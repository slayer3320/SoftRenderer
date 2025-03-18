#pragma once

#include "vector"

#include "Singleton.hpp"
#include "RasterizationStage.hpp"
#include "Buffer.hpp"
#include "../shader/ShaderData.hpp"
#include "../shader/Shader.hpp"

//顶点着色器   将模型obj文件中的顶点 从 模型空间/对象空间/局部空间->世界空间->观察空间/摄像机空间->裁剪空间/齐次裁切空间
//曲面细分着色器
//几何着色器
//裁剪   裁剪空间中坐标的范围是[-1,1]，超出这个范围的顶点会被裁剪掉
//屏幕映射   将裁剪空间正方向小盒子拉伸成屏幕大小的正方形
class GeometricStage : public Singleton<GeometricStage>
{
public:
	GeometricStage() = default;
	~GeometricStage() = default;
	void VertexShader(IShader* shader);
	void TessellationShader();
	void GeometryShader();
	void Clipping();
	void ScreenMapping();

	void NextStage();

	std::vector<Shader_Struct_a2v*>* a2v_data;
	std::vector<Shader_Struct_v2f*>* v2f_data;
	std::vector<bool>* clip_flag;
};