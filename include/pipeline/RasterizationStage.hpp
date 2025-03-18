#pragma once

#include "vector"

#include "Singleton.hpp"
#include "../shader/ShaderData.hpp"
#include "../shader/Shader.hpp"
//三角形设置
//三角形遍历
//片元着色器 
//逐片元操作



class RasterizationStage : public Singleton<RasterizationStage>
{
public:
	RasterizationStage() = default;
    ~RasterizationStage() = default;
    void TriangleSetup();
    void TriangleTraversal();
    void FragmentShader(IShader* shader);
    void FragmentOperation();

    void NextStage();

    std::vector<Shader_Struct_v2f*>* v2f_data;
};