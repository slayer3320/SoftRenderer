#include "pipeline/RenderPipeline.hpp"

RenderPipeline::RenderPipeline()
{
}

RenderPipeline::~RenderPipeline()
{
}

void RenderPipeline::Render()
{
    
}

void RenderPipeline::Preprocess(Mesh* mesh)
{
    //mesh->Simplification();
}

void RenderPipeline::PepareData(Mesh* mesh)
{
    int meshVertexCount = mesh->vertices.size();
    int meshFaceCount = mesh->faces.size();

    a2v_data->resize(meshVertexCount);

    
    if(isCulling)
    {
        std::vector<bool> isCulled = FrustumCulling(mesh);
        //std::vector<bool> isCulled = std::vector<bool>(vertex_count, false);
        std::unordered_map<int, int> indexDict;

        Shader_Struct_a2v a2v = Shader_Struct_a2v();
        int current_index = 0;
        for(int i = 0; i < meshVertexCount; i++)
        {
            if(isCulled[i])
            {
                continue;
            }   

            a2v.pos = mesh->vertices[i].pos;
            a2v.normal = mesh->vertices[i].normal;
            a2v.texcoord = mesh->vertices[i].texcoord;
            a2v_data->at(current_index) = a2v;
            indexDict[i] = current_index;
            current_index++;
        }

        for(int i = 0; i < meshFaceCount; i++)
        {
            if(isCulled[mesh->faces[i].v1] || isCulled[mesh->faces[i].v2] || isCulled[mesh->faces[i].v3])
            {
                continue;
            }

            face_data->emplace_back(Face(indexDict[mesh->faces[i].v1], indexDict[mesh->faces[i].v2], indexDict[mesh->faces[i].v3]));
        }

        vertex_count = a2v_data->size();
        face_count = face_data->size();

        // std::cout<<"vertex_count: "<<vertex_count<<std::endl;   
        // std::cout<<"face_count: "<<face_count<<std::endl;
    }
    else
    {
        Shader_Struct_a2v a2v = Shader_Struct_a2v();
        for(int i = 0; i < meshVertexCount; i++)
        {

            a2v.pos = mesh->vertices[i].pos;
            a2v.normal = mesh->vertices[i].normal;
            a2v.texcoord = mesh->vertices[i].texcoord;
            a2v_data->at(i) = a2v;
        }

        face_data = &mesh->faces;

        vertex_count = a2v_data->size();
        face_count = face_data->size();
    }
}

std::vector<bool> RenderPipeline::FrustumCulling(Mesh* mesh)
{

    int meshVertexCount = mesh->vertices.size();

    std::vector<bool> isCulled = std::vector<bool>(meshVertexCount, false);

    OcTreeNode* root = new OcTreeNode(Vector3f(0.0f, 0.0f, 0.0f), 1.0f);

    for(int i = 0; i < meshVertexCount; i++)
    {
        root->Insert(mesh->vertices[i].pos);
    }

    Frustum frustum = Scene::GetInstance()->GetCamera()->GetFrustum();

    int ct = 0;

    std::vector<Vector3f> visiblePoints;
    root->Cull(frustum, visiblePoints);
    std::cout<<visiblePoints.size()<<std::endl;
    for(int i = 0; i < meshVertexCount; i++)
    {
        if(std::find(visiblePoints.begin(), visiblePoints.end(), mesh->vertices[i].pos) == visiblePoints.end())
        {
            isCulled[i] = true;
            ct ++;
            //std::cout<<"isCulled["<< i << "] = true;" <<std::endl;
        }
    }

    std::cout<<"culled count:" << ct <<std::endl;
    delete root;
    return isCulled;
}

void RenderPipeline::VertexShader(IShader* shader)
{
    bool isMutiThread = true;

    if(isMutiThread)
    {
        v2f_data->resize(vertex_count);

        int num_threads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads(num_threads);

        for(int t = 0; t < num_threads; t++)
        {
            threads[t] = std::thread([&, t]()
            {
                for(int i = t; i < vertex_count; i += num_threads)
                {
                    v2f_data->at(i) = *shader->VertexShader(&a2v_data->at(i));
                }
            });
        }

        for(auto& thread : threads)
        {
            thread.join();
        }
    }
    else
    {
        v2f_data->resize(vertex_count);

        for(int i = 0; i < vertex_count; i++)
        {
            v2f_data->at(i) = *shader->VertexShader(&a2v_data->at(i));
        }
    }

    
}

void RenderPipeline::TessellationShader()
{
    
}

void RenderPipeline::GeometryShader()
{
    
}

void RenderPipeline::Clipping()
{
    clip_flag->resize(vertex_count);

    for(int i = 0; i < vertex_count; i ++)
    {
        if((*v2f_data)[i].pos.x < -1 || (*v2f_data)[i].pos.x > 1 || (*v2f_data)[i].pos.y < -1 || (*v2f_data)[i].pos.y > 1 || (*v2f_data)[i].pos.z < -1 || (*v2f_data)[i].pos.z > 1)
        {
            (*clip_flag)[i] = true;
        }
        else
        {
            (*clip_flag)[i] = false;
        }
    }
}

// void RenderPipeline::ScreenMapping()
// {
//     for(int i = 0; i < vertex_count; i ++)
//     {
//         std::cout<<(*v2f_data)[i]->pos.x<<" "<<(*v2f_data)[i]->pos.y<<"  ->  ";
//         (*v2f_data)[i]->pos.x = ((*v2f_data)[i]->pos.x + 1) * 0.5 * Buffer::GetInstance()->GetWidth();
//         (*v2f_data)[i]->pos.y = ((*v2f_data)[i]->pos.y + 1) * 0.5 * Buffer::GetInstance()->GetHeight();
//         std::cout<<(*v2f_data)[i]->pos.x<<" "<<(*v2f_data)[i]->pos.y<<std::endl;
//     }
// }

void RenderPipeline::ScreenMapping()
{
    float width = Buffer::GetInstance()->GetWidth();
    float height = Buffer::GetInstance()->GetHeight();
    float aspect_ratio = width / height;

    for(int i = 0; i < vertex_count; i++)
    {   
        //aspect_ratio是解决图像随屏幕宽高比例变化而变形的问题
        //负号是解决图像上下左右颠倒的问题
        if (width >= height) 
        {
            (*v2f_data)[i].pos.x = ((*v2f_data)[i].pos.x / aspect_ratio + 1) * 0.5 * width;
            (*v2f_data)[i].pos.y = (-(*v2f_data)[i].pos.y + 1) * 0.5 * height;
        }
    }
}


void RenderPipeline::TriangleSetup()
{
    
}



void RenderPipeline::TriangleTraversal(IShader* shader)
{
    bool isMultiThread = true;
    bool isRetainedFragment = false;

    if(isMultiThread)
    {
        if(isRetainedFragment)
        {
            fragment_data->clear();

            int num_threads = std::thread::hardware_concurrency();
            std::vector<std::thread> threads(num_threads);
            std::mutex mutex;

            for (int t = 0; t < num_threads; ++t) 
            {
                threads[t] = std::thread([&, t]() 
                {
                    std::vector<Fragment> thread_fragment_data;
                    thread_fragment_data.clear();

                    for (int i = t; i < face_count; i += num_threads) 
                    {
                        if ((*clip_flag)[(*face_data)[i].v1] || (*clip_flag)[(*face_data)[i].v2] || (*clip_flag)[(*face_data)[i].v3]) 
                        {
                            continue;
                        }

                        Shader_Struct_v2f* v1 = &(*v2f_data)[(*face_data)[i].v1];
                        Shader_Struct_v2f* v2 = &(*v2f_data)[(*face_data)[i].v2];
                        Shader_Struct_v2f* v3 = &(*v2f_data)[(*face_data)[i].v3];

                        int x_min = std::min(std::min(v1->pos.x, v2->pos.x), v3->pos.x);
                        int x_max = std::max(std::max(v1->pos.x, v2->pos.x), v3->pos.x);
                        int y_min = std::min(std::min(v1->pos.y, v2->pos.y), v3->pos.y);
                        int y_max = std::max(std::max(v1->pos.y, v2->pos.y), v3->pos.y);

                        for (int x = x_min; x <= x_max; x++) 
                        {
                            for (int y = y_min; y <= y_max; y++) 
                            {
                                if (MATH::IsInsideTriangle(x, y, v1->pos, v2->pos, v3->pos) == false) 
                                {
                                    continue;
                                }

                                Vector3f barycentricCoord = MATH::GetBarycentricCoord(x, y, v1->pos, v2->pos, v3->pos);
                                Shader_Struct_v2f v2f;
                                v2f.pos = v1->pos * barycentricCoord.x + v2->pos * barycentricCoord.y + v3->pos * barycentricCoord.z;
                                v2f.normal = v1->normal * barycentricCoord.x + v2->normal * barycentricCoord.y + v3->normal * barycentricCoord.z;
                                v2f.texcoord = v1->texcoord * barycentricCoord.x + v2->texcoord * barycentricCoord.y + v3->texcoord * barycentricCoord.z;
                                v2f.color = v1->color * barycentricCoord.x + v2->color * barycentricCoord.y + v3->color * barycentricCoord.z;

                                Fragment fragment;
                                fragment.x = x;
                                fragment.y = y;
                                fragment.depth = v2f.pos.z;
                                fragment.color = RenderPipeline::FragmentShader(shader, &v2f);

                                thread_fragment_data.emplace_back(fragment);
                            }
                        }
                    }

                    std::lock_guard<std::mutex> lock(mutex);
                    fragment_data->insert(fragment_data->end(), thread_fragment_data.begin(), thread_fragment_data.end());
                });
            }

            for (auto& thread : threads) 
            {
                thread.join(); 
            }

            fragment_count = fragment_data->size();
        }
        else
        {
            fragment_data->clear();

            int num_threads = std::thread::hardware_concurrency();
            std::vector<std::thread> threads(num_threads);
            std::mutex mutex;

            for (int t = 0; t < num_threads; ++t) 
            {
                threads[t] = std::thread([&, t]() 
                {
                    Shader_Struct_v2f* v1;
                    Shader_Struct_v2f* v2;
                    Shader_Struct_v2f* v3;
                    
                    for (int i = t; i < face_count; i += num_threads) 
                    {
                        if ((*clip_flag)[(*face_data)[i].v1] || (*clip_flag)[(*face_data)[i].v2] || (*clip_flag)[(*face_data)[i].v3]) 
                        {
                            continue;
                        }

                        v1 = &(*v2f_data)[(*face_data)[i].v1];
                        v2 = &(*v2f_data)[(*face_data)[i].v2];
                        v3 = &(*v2f_data)[(*face_data)[i].v3];

                        int x_min = std::min(std::min(v1->pos.x, v2->pos.x), v3->pos.x);
                        int x_max = std::max(std::max(v1->pos.x, v2->pos.x), v3->pos.x);
                        int y_min = std::min(std::min(v1->pos.y, v2->pos.y), v3->pos.y);
                        int y_max = std::max(std::max(v1->pos.y, v2->pos.y), v3->pos.y);

                        for (int x = x_min; x <= x_max; x++) 
                        {
                            for (int y = y_min; y <= y_max; y++) 
                            {
                                if (MATH::IsInsideTriangle(x, y, v1->pos, v2->pos, v3->pos) == false) 
                                {
                                    continue;
                                }

                                Vector3f barycentricCoord = MATH::GetBarycentricCoord(x, y, v1->pos, v2->pos, v3->pos);
                                Shader_Struct_v2f v2f;
                                v2f.pos = v1->pos * barycentricCoord.x + v2->pos * barycentricCoord.y + v3->pos * barycentricCoord.z;
                                v2f.normal = v1->normal * barycentricCoord.x + v2->normal * barycentricCoord.y + v3->normal * barycentricCoord.z;
                                v2f.texcoord = v1->texcoord * barycentricCoord.x + v2->texcoord * barycentricCoord.y + v3->texcoord * barycentricCoord.z;
                                v2f.color = v1->color * barycentricCoord.x + v2->color * barycentricCoord.y + v3->color * barycentricCoord.z;

                                Fragment fragment;
                                fragment.x = x;
                                fragment.y = y;
                                fragment.depth = v2f.pos.z;
                                fragment.color = RenderPipeline::FragmentShader(shader, &v2f);

                                // if(barycentricCoord.x + barycentricCoord.y > 0.99f ||
                                // barycentricCoord.y + barycentricCoord.z > 0.99f ||
                                // barycentricCoord.x + barycentricCoord.z > 0.99f)
                                // {
                                //     fragment.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
                                // }
                                // else
                                // {
                                //     fragment.color = Color(0.0f, 0.0f, 0.0f, 1.0f);
                                // }

                                if(fragment.depth < Buffer::GetInstance()->GetDepth(x, y))
                                {
                                    
                                    Buffer::GetInstance()->SetDepth(x, y, fragment.depth);
                                    Buffer::GetInstance()->SetColor(x, y, fragment.color);
                                }
                            }
                        }
                    }
                });
            }

            for (auto& thread : threads) 
            {
                thread.join(); 
            }

            fragment_count = fragment_data->size();
        }
    }
    else
    {
        fragment_data->clear();

        Shader_Struct_v2f* v2f = new Shader_Struct_v2f();
        Shader_Struct_v2f* v1;
        Shader_Struct_v2f* v2;
        Shader_Struct_v2f* v3;
        int x_min, x_max, y_min, y_max;
        Fragment fragment = Fragment();
        Vector3f barycentricCoord;

        for(int i = 0; i < face_count; i++)
        {
            if((*clip_flag)[(*face_data)[i].v1] || (*clip_flag)[(*face_data)[i].v2] || (*clip_flag)[(*face_data)[i].v3])
            {
                continue;
            }

            v1 = &(*v2f_data)[(*face_data)[i].v1];
            v2 = &(*v2f_data)[(*face_data)[i].v2];
            v3 = &(*v2f_data)[(*face_data)[i].v3];

            x_min = std::min(std::min(v1->pos.x, v2->pos.x), v3->pos.x);
            x_max = std::max(std::max(v1->pos.x, v2->pos.x), v3->pos.x);
            y_min = std::min(std::min(v1->pos.y, v2->pos.y), v3->pos.y);
            y_max = std::max(std::max(v1->pos.y, v2->pos.y), v3->pos.y);

            for(int x = x_min; x <= x_max; x++)
            {
                for(int y = y_min; y <= y_max; y++)
                {
                    if(MATH::IsInsideTriangle(x, y, v1->pos, v2->pos, v3->pos) == false)
                    {
                        continue;
                    }

                    barycentricCoord = MATH::GetBarycentricCoord(x, y, v1->pos, v2->pos, v3->pos);
                    v2f->pos = v1->pos * barycentricCoord.x + v2->pos * barycentricCoord.y + v3->pos * barycentricCoord.z;
                    v2f->normal = v1->normal * barycentricCoord.x + v2->normal * barycentricCoord.y + v3->normal * barycentricCoord.z;
                    v2f->texcoord = v1->texcoord * barycentricCoord.x + v2->texcoord * barycentricCoord.y + v3->texcoord * barycentricCoord.z;
                    v2f->color = v1->color * barycentricCoord.x + v2->color * barycentricCoord.y + v3->color * barycentricCoord.z;

                    fragment.x = x;
                    fragment.y = y;
                    fragment.depth = v2f->pos.z;
                    if(barycentricCoord.x + barycentricCoord.y > 0.99f ||
                       barycentricCoord.y + barycentricCoord.z > 0.99f ||
                       barycentricCoord.x + barycentricCoord.z > 0.99f)
                    {
                        fragment.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
                    }
                    else
                    {
                        fragment.color = Color(0.0f, 0.0f, 0.0f, 1.0f);
                    }
                    //fragment.color = RenderPipeline::FragmentShader(shader, v2f);

                    //fragment_data->at(current_count) = fragment;
                    fragment_data->emplace_back(fragment);
                }
            }
        }
        
        fragment_count = fragment_data->size();
    }

    
}

Color RenderPipeline::FragmentShader(IShader* shader, Shader_Struct_v2f* v2f)
{
    return shader->FragmentShader(v2f);
}


void RenderPipeline::FragmentOperation()
{
    //int fragment_count = fragment_data->size();

    for(int i = 0; i < fragment_count; i++)
    {
        int x = fragment_data->at(i).x, y = fragment_data->at(i).y;

        if(x >= Buffer::GetInstance()->GetWidth() || x < 0 || y >= Buffer::GetInstance()->GetHeight() || y < 0)
        {
            continue;
        }   

        if(fragment_data->at(i).depth < Buffer::GetInstance()->GetDepth(x, y))
        {
            
            Buffer::GetInstance()->SetDepth(x, y, fragment_data->at(i).depth);
            Buffer::GetInstance()->SetColor(x, y, fragment_data->at(i).color);
        }
    }
}

void RenderPipeline::RefreshData()
{
    

}

