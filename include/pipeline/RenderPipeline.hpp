#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <unordered_map>

#include "../Singleton.hpp"
#include "Buffer.hpp"
#include "../math/MathUtil.hpp"
#include "../math/OcTree.hpp"
#include "../shader/Shader.hpp"
#include "../component/Mesh.hpp"



class RenderPipeline : public Singleton<RenderPipeline>
{
public:
    RenderPipeline();
    ~RenderPipeline();
    void Render();

    void Preprocess(Mesh* mesh);
    void PepareData(Mesh* mesh);
    std::vector<bool> FrustumCulling(Mesh* mesh);

	void VertexShader(IShader* shader);
	void TessellationShader();
	void GeometryShader();
	void Clipping();
	void ScreenMapping();

	void TriangleSetup();
    void TriangleTraversal(IShader* shader);
    Color FragmentShader(IShader* shader, Shader_Struct_v2f* v2f);
    void FragmentOperation();

    void RefreshData();

    unsigned int vertex_count = 0;
    unsigned int face_count = 0;
    unsigned int fragment_count = 0;

	std::vector<Shader_Struct_a2v>* a2v_data = new std::vector<Shader_Struct_a2v>();
	std::vector<Shader_Struct_v2f>* v2f_data = new std::vector<Shader_Struct_v2f>();
	std::vector<Fragment>* fragment_data = new std::vector<Fragment>();


    std::vector<Face>* face_data = new std::vector<Face>();
    std::vector<bool>* clip_flag = new std::vector<bool>();
    bool isCulling = false;
};
