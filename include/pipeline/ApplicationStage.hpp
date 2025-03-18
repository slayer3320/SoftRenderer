#pragma once

#include "vector"
#include "Singleton.hpp"
#include "GeometricStage.hpp"
#include "../shader/ShaderData.hpp"
#include "../component/Mesh.hpp"

//应用阶段会提供大致以下数据：
//顶点（网格）
//颜色
//纹理的UV坐标
//MVP矩阵
//光源位置
class ApplicationStage : public Singleton<ApplicationStage>
{
public:
    ApplicationStage() = default;
    ~ApplicationStage() = default;

    void PepareData(Mesh* mesh);
    void NextStage();

    std::vector<Shader_Struct_a2v*>* a2v_data;
};