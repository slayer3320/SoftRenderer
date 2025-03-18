#include "component/Mesh.hpp"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::LoadMesh(std::string path) 
{
    std::ifstream file(path);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::string line;
    std::vector<Vector3f> temp_positions;
    std::vector<Vector2f> temp_texcoords;
    std::vector<Vector3f> temp_normals;

    struct VIdx
    {
        unsigned int pidx, tidx, nidx;
    };

    std::vector<VIdx> temp_vidx;

    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") 
        {
            Vector3f position;
            iss >> position.x >> position.y >> position.z;
            temp_positions.emplace_back(position);
        } 
        else if (prefix == "vt") 
        {
            Vector2f texcoord;
            iss >> texcoord.x >> texcoord.y;
            temp_texcoords.emplace_back(texcoord);
        } 
        else if (prefix == "vn") 
        {
            Vector3f normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.emplace_back(normal);
        } 
        else if (prefix == "f") 
        {
            unsigned int vIdx[3], tIdx[3], nIdx[3];
            char slash;
            for (int i = 0; i < 3; i++) 
            {
                iss >> vIdx[i] >> slash >> tIdx[i] >> slash >> nIdx[i];
                vIdx[i]--;
                tIdx[i]--;
                nIdx[i]--;
                temp_vidx.push_back({vIdx[i], tIdx[i], nIdx[i]});
            }
            originalEdges.emplace_back(Edge(vIdx[0], vIdx[1]));
            originalEdges.emplace_back(Edge(vIdx[1], vIdx[2]));
            originalEdges.emplace_back(Edge(vIdx[2], vIdx[0]));

            originalFaces.emplace_back(Face(vIdx[0], vIdx[1], vIdx[2]));

            originalvertex2edge[vIdx[0]].emplace_back(originalEdges.size() - 3);
            originalvertex2edge[vIdx[1]].emplace_back(originalEdges.size() - 2);
            originalvertex2edge[vIdx[2]].emplace_back(originalEdges.size() - 1);
            originalvertex2edge[vIdx[0]].emplace_back(originalEdges.size() - 1);
            originalvertex2edge[vIdx[1]].emplace_back(originalEdges.size() - 3);
            originalvertex2edge[vIdx[2]].emplace_back(originalEdges.size() - 2);

            originalvertex2face[vIdx[0]].emplace_back(originalFaces.size() - 1);
            originalvertex2face[vIdx[1]].emplace_back(originalFaces.size() - 1);
            originalvertex2face[vIdx[2]].emplace_back(originalFaces.size() - 1);
        }
    }

    originalVertices = std::vector<Vertex>(temp_positions.size());

    for(auto idx : temp_vidx)
    {
        originalVertices[idx.pidx].pos = temp_positions[idx.pidx];
        originalVertices[idx.pidx].texcoord = temp_texcoords[idx.tidx];
        originalVertices[idx.pidx].normal = temp_normals[idx.nidx];
    }

    file.close();

    vertices = originalVertices;
    edges = originalEdges;
    faces = originalFaces;
    vertex2face = originalvertex2face;
    vertex2edge = originalvertex2edge;
}


void Mesh::Update()
{
}

void Mesh::Subdivision()
{
}

void Mesh::Simplification()
{
    if(isSimplification) return;
    isSimplification = true;

    vertices = originalVertices;
    edges = originalEdges;
    faces = originalFaces;
    vertex2face = originalvertex2face;
    vertex2edge = originalvertex2edge;
    
    //计算面的二次误差矩阵
    std::vector<Matrix4x4f> vertexErrorMatrix;
    std::vector<Matrix4x4f> edgeErrorMatrix;
    std::vector<Matrix4x4f> faceErrorMatrix;
    std::vector<int> mergerConsideration;

    for(int i = 0; i < faces.size(); i ++)
    {
        auto face = faces[i];
        Vector4f planeEquationParameters = MATH::GetPlaneEquationParameters(vertices[face.v1].pos, vertices[face.v2].pos, vertices[face.v3].pos);
        
        float errorMatrixArray[4][4] = 
        {
            {planeEquationParameters.x * planeEquationParameters.x, planeEquationParameters.x * planeEquationParameters.y, planeEquationParameters.x * planeEquationParameters.z, planeEquationParameters.x * planeEquationParameters.w},
            {planeEquationParameters.y * planeEquationParameters.x, planeEquationParameters.y * planeEquationParameters.y, planeEquationParameters.y * planeEquationParameters.z, planeEquationParameters.y * planeEquationParameters.w},
            {planeEquationParameters.z * planeEquationParameters.x, planeEquationParameters.z * planeEquationParameters.y, planeEquationParameters.z * planeEquationParameters.z, planeEquationParameters.z * planeEquationParameters.w},
            {planeEquationParameters.w * planeEquationParameters.x, planeEquationParameters.w * planeEquationParameters.y, planeEquationParameters.w * planeEquationParameters.z, planeEquationParameters.w * planeEquationParameters.w}
        };
        Matrix4x4f errorMatrix(errorMatrixArray);

        faceErrorMatrix.emplace_back(errorMatrix);
    }

    for(int i = 0; i < vertices.size(); i ++)
    {
        auto vertex = vertices[i];

        Matrix4x4f errorMatrix = Matrix4x4f();
        for(auto j : vertex2face[i])
        {
            errorMatrix = errorMatrix + faceErrorMatrix[j];
        }

        vertexErrorMatrix.emplace_back(errorMatrix);
    }
    
    //<合并代价，边idx>
    // auto compare = [](const std::pair<float, int>& a, const std::pair<float, int>& b) 
    // {
    //     return a.first > b.first; // 使得小的元素优先级更高
    // };

    // std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, decltype(compare)> mergeCostQueue(compare);
    
    //param: first: mergeCost, second: edgeIdx
    MinHeap mergeCostQueue;

    // 更新顶点列表：合并两个顶点后，新顶点取代原来的两个顶点。
    // 更新边信息：删除与合并顶点相关的所有边，添加新的边连接新顶点与其他顶点。
    // 重新计算二次误差矩阵：新顶点的二次误差矩阵是原来两个顶点矩阵的和。
    // 重新计算边的合并代价：对所有与新顶点相连的边，重新计算合并代价。
    for(int i = 0; i < edges.size(); i ++)
    {
        Matrix4x4f errorMatrix = vertexErrorMatrix[edges[i].v1] + vertexErrorMatrix[edges[i].v2];
        //edgeErrorMatrix.emplace_back(vertexErrorMatrix[edges[i].v1] + vertexErrorMatrix[edges[i].v2]);

        Vector3f edgeMidPoint = (vertices[edges[i].v1].pos + vertices[edges[i].v2].pos) / 2;
        Vector4f homogenEdgeMidPoint = Vector4f(edgeMidPoint.x, edgeMidPoint.y, edgeMidPoint.z, 1.0f);
        float mergeCost = TransposeMutiply(homogenEdgeMidPoint, errorMatrix * homogenEdgeMidPoint);

        mergeCostQueue.push(std::make_pair(mergeCost, i));
    }

    std::vector<bool> isVertexMerged(vertices.size(), false);
    std::vector<bool> isEdgeMerged(edges.size(), false);
    std::vector<bool> isFaceMerged(faces.size(), false);
    int targetEdgeCount = edges.size() / 2;

    while(mergeCostQueue.size() > targetEdgeCount)
    {
        int edgeIdx = mergeCostQueue.top().second;
        Edge delEdge = edges[edgeIdx];
        mergeCostQueue.pop();

        if(isEdgeMerged[edgeIdx]) continue;
        isEdgeMerged[edgeIdx] = true;

        isVertexMerged[delEdge.v1] = true;
        isVertexMerged[delEdge.v2] = true;
        
        //更新点
        Vector3f newVertexPos = (vertices[delEdge.v1].pos + vertices[delEdge.v2].pos) / 2;
        Vector2f newVertexTexcoord = (vertices[delEdge.v1].texcoord + vertices[delEdge.v2].texcoord) / 2;
        Vector3f newVertexNormal = (vertices[delEdge.v1].normal + vertices[delEdge.v2].normal) / 2;
        Vertex newVertex = Vertex{newVertexPos, newVertexTexcoord, newVertexNormal};
        vertices.emplace_back(newVertex);
        
        isVertexMerged.emplace_back(false);

        int newVertexIdx = vertices.size() - 1;

        unsigned int delVertices[2] = {delEdge.v1, delEdge.v2};

        for(int i = 0; i < 2; i ++)
        {
            unsigned int delVertexIdx = delVertices[i];
            for(auto delFaceIdx : vertex2face[delVertexIdx])
            {
                //这tm不是一点用没有
                //if(isVertexMerged[faces[delFaceIdx].v1] || isVertexMerged[faces[delFaceIdx].v2] || isVertexMerged[faces[delFaceIdx].v3]) continue; 
                if(isFaceMerged[delFaceIdx]) continue;
                //更新面
                Face delFace = faces[delFaceIdx];
                Face newFace = delFace;

                //如果delFace的三个顶点中两个要删除则跳过
                //是否跳过这个面
                bool isSkip = false;
                unsigned int delFaceVertex[3] = {delFace.v1, delFace.v2, delFace.v3};
                for(int j = 0; j < 3; j ++)
                {
                    if(delFaceVertex[j] == delVertices[0] && delFaceVertex[(j + 1) % 3] == delVertices[1] ||
                       delFaceVertex[j] == delVertices[1] && delFaceVertex[(j + 1) % 3] == delVertices[0])
                    {
                        isSkip = true;
                    }
                }
                
                if(isSkip)
                {
                    isFaceMerged[delFaceIdx] = true;
                    continue;
                }

                if(delFace.v1 == delVertexIdx)
                {
                    newFace.v1 = newVertexIdx;
                }
                else if(delFace.v2 == delVertexIdx)
                {
                    newFace.v2 = newVertexIdx;
                }
                else if(delFace.v3 == delVertexIdx)
                {
                    newFace.v3 = newVertexIdx;
                }
                else
                {
                    std::cout << "Error: face vertex not match" << std::endl;
                    continue;
                }

                faces[delFaceIdx] = newFace;

                //更新vertex2face
                vertex2face[newVertexIdx].emplace_back(delFaceIdx);

                //更新面的二次误差矩阵
                Vector4f planeEquationParameters = MATH::GetPlaneEquationParameters(vertices[newFace.v1].pos, vertices[newFace.v2].pos, vertices[newFace.v3].pos);
                float errorMatrixArray[4][4] = 
                {
                    {planeEquationParameters.x * planeEquationParameters.x, planeEquationParameters.x * planeEquationParameters.y, planeEquationParameters.x * planeEquationParameters.z, planeEquationParameters.x * planeEquationParameters.w},
                    {planeEquationParameters.y * planeEquationParameters.x, planeEquationParameters.y * planeEquationParameters.y, planeEquationParameters.y * planeEquationParameters.z, planeEquationParameters.y * planeEquationParameters.w},
                    {planeEquationParameters.z * planeEquationParameters.x, planeEquationParameters.z * planeEquationParameters.y, planeEquationParameters.z * planeEquationParameters.z, planeEquationParameters.z * planeEquationParameters.w},
                    {planeEquationParameters.w * planeEquationParameters.x, planeEquationParameters.w * planeEquationParameters.y, planeEquationParameters.w * planeEquationParameters.z, planeEquationParameters.w * planeEquationParameters.w}
                };

                faceErrorMatrix[delFaceIdx] = Matrix4x4f(errorMatrixArray);
            }
        }

        //更新点的二次误差矩阵
        //找到所有与删除点相关的面，找到这些面的三个顶点
        for(int i = 0; i < 2; i ++)
        {
            for(auto updateFaceIdx : vertex2face[newVertexIdx])
            {
                //if(isVertexMerged[faces[delFaceIdx].v1] || isVertexMerged[faces[delFaceIdx].v2] || isVertexMerged[faces[delFaceIdx].v3]) continue; 
                if(isFaceMerged[updateFaceIdx]) continue;

                unsigned int updateVertexIdxs[3] = {faces[updateFaceIdx].v1, faces[updateFaceIdx].v2, faces[updateFaceIdx].v3};

                for(auto updateVertexIdx : updateVertexIdxs)
                {
                    if(updateVertexIdx == newVertexIdx) continue;

                    Matrix4x4f errorMatrix = Matrix4x4f();
                    for(auto j : vertex2face[updateVertexIdx])
                    {
                        if(isFaceMerged[j]) continue;
                        errorMatrix = errorMatrix + faceErrorMatrix[j];
                    }

                    vertexErrorMatrix[updateVertexIdx] = errorMatrix;
                }
            }
        }

        Matrix4x4f newVertexErrorMatrix = Matrix4x4f();
        for(auto j : vertex2face[newVertexIdx])
        {
            if(isFaceMerged[j]) continue;
            newVertexErrorMatrix = newVertexErrorMatrix + faceErrorMatrix[j];
        }
        vertexErrorMatrix.push_back(newVertexErrorMatrix);

     
        //更新边
        //只包含一个删除点的面中与删除点有关的边，需要更新edge和mergeCostQueue
        //包含两个删除点的面中与删除点有关的边，需要删除edge和mergeCostQueue
        //新产生的边加到edge和mergeCostQueue
        for(int i = 0; i < 2; i ++)
        {
            unsigned int delVertexIdx = delVertices[i];
            for(auto delFaceIdx : vertex2face[delVertexIdx])
            {
                //不需要，因为不管这个面有没有合并，都需要对面进行操作
                //if(isFaceMerged[delFaceIdx]) continue;
                
                Face delFace = faces[delFaceIdx];

                //如果delFace的三个顶点中两个要删除
                unsigned int delFaceVertex[3] = {delFace.v1, delFace.v2, delFace.v3};
                bool isDelEdge = false;
                unsigned int extraVertexIdx = 0;
                for(int j = 0; j < 3; j ++)
                {
                    if(delFaceVertex[j] == delVertices[0] && delFaceVertex[(j + 1) % 3] == delVertices[1] ||
                       delFaceVertex[j] == delVertices[1] && delFaceVertex[(j + 1) % 3] == delVertices[0])
                    {
                        isDelEdge = true;
                        extraVertexIdx = delFaceVertex[(j + 2) % 3];
                    }
                }
                
                if(isDelEdge)
                {
                    
                }
                else
                {
                    for(int j = 0; j < 3; j ++)
                    {
                        if(delFaceVertex[j] == delVertexIdx)
                        {
                            unsigned int v1 = delFaceVertex[(j + 1) % 3];
                            unsigned int v2 = delFaceVertex[(j + 2) % 3];

                            Edge updateEdge1 = Edge(v1, newVertexIdx);
                            Edge updateEdge2 = Edge(v2, newVertexIdx);

                            //找到updateEdge1和updateEdge2在edges中的位置
                            int updateEdgeIdx1 = -1;
                            int updateEdgeIdx2 = -1;

                            for(auto tmpEdge : vertex2edge[v1])
                            {
                                if(edges[tmpEdge].v1 == v1 && edges[tmpEdge].v2 == delVertexIdx || edges[tmpEdge].v1 == delVertexIdx && edges[tmpEdge].v2 == v1)
                                {
                                    updateEdgeIdx1 = tmpEdge;
                                }
                            }
                            for(auto tmpEdge : vertex2edge[v2])
                            {
                                if(edges[tmpEdge].v1 == v2 && edges[tmpEdge].v2 == delVertexIdx || edges[tmpEdge].v1 == delVertexIdx && edges[tmpEdge].v2 == v2)
                                {
                                    updateEdgeIdx2 = tmpEdge;
                                }
                            }

                            edges[updateEdgeIdx1] = updateEdge1;
                            edges[updateEdgeIdx2] = updateEdge2;

                            
                            Matrix4x4f errorMatrix = vertexErrorMatrix[v1] + vertexErrorMatrix[newVertexIdx];
                            Vector3f edgeMidPoint = (vertices[v1].pos + vertices[newVertexIdx].pos) / 2;
                            Vector4f homogenEdgeMidPoint = Vector4f(edgeMidPoint.x, edgeMidPoint.y, edgeMidPoint.z, 1.0f);
                            float mergeCost = TransposeMutiply(homogenEdgeMidPoint, errorMatrix * homogenEdgeMidPoint);
                            mergeCostQueue.modify(updateEdgeIdx1, mergeCost);

                            errorMatrix = vertexErrorMatrix[v2] + vertexErrorMatrix[newVertexIdx];
                            edgeMidPoint = (vertices[v2].pos + vertices[newVertexIdx].pos) / 2;
                            homogenEdgeMidPoint = Vector4f(edgeMidPoint.x, edgeMidPoint.y, edgeMidPoint.z, 1.0f);
                            mergeCost = TransposeMutiply(homogenEdgeMidPoint, errorMatrix * homogenEdgeMidPoint);
                            mergeCostQueue.modify(updateEdgeIdx2, mergeCost);
                        }
                    }
                }

            }
        }
    }


    //之后修改成remove_if
    //根据bool数组删除元素
    for (int i = 0; i < isEdgeMerged.size(); i++) 
    {
        if (isEdgeMerged[i]) 
        {
            edges.erase(edges.begin() + i);
        }
    }

    for (int i = 0; i < isVertexMerged.size(); i++) 
    {
        if (isVertexMerged[i]) 
        {
            vertices.erase(vertices.begin() + i);
        }
    }

    for (int i = 0; i < faces.size(); i++) 
    {
        Face face = faces[i];
        if (isVertexMerged[face.v1] || isVertexMerged[face.v2] || isVertexMerged[face.v3]) 
        {
            faces.erase(faces.begin() + i);
        }
    }
}

void Mesh::Print()
{
    for (auto vertex : vertices)
    {
        std::cout << "Vertex: " << vertex.pos << std::endl;
    }
    for (auto face : faces)
    {
        std::cout << "Face: " << face.v1 << ", " << face.v2 << ", " << face.v3 << std::endl;
    }
}
