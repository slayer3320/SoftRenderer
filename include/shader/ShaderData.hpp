#pragma once

#include "../Color.hpp"
#include "../math/Matrix.hpp"

//struct ShaderData {
//	Material* matrial;
//	RenderBuffer* targetBuffer;
//	RenderBuffer* shadow_map;
//	//bool enable_shadow;
//	Vector3f view_Pos;
//	Vector3f light_dir;
//	Color light_color;
//	Color ambient;
//	Matrix4x4 model_matrix;
//	Matrix4x4 model_matrix_I;
//	Matrix4x4 view_matrix;
//	Matrix4x4 projection_matrix;
//	Matrix4x4 light_vp_matrix;
//	Matrix4x4 camera_vp_matrix;
//};

struct ShaderData
{
	Matrix4x4f model_matrix;
	Matrix4x4f view_matrix;
	Matrix4x4f projection_matrix;

	Vector3f lightDir0;
	Color lightColor0;

};

struct Shader_Struct_a2v
{
	Vector3f pos;
	Vector3f normal;
	Vector2f texcoord;
	Color color;

	Shader_Struct_a2v()
	{
		pos = Vector3f();
		normal = Vector3f();
		texcoord = Vector2f();
		color = Color();
	}
};

struct Shader_Struct_v2f
{
	Vector3f pos;
	Vector3f normal;
	Vector2f texcoord;
	Color color;

	Shader_Struct_v2f()
	{
		pos = Vector3f();
		normal = Vector3f();
		texcoord = Vector2f();
		color = Color();
	}
}; 

struct Fragment
{
    unsigned int x;
    unsigned int y;
    float depth;
    Vector3f normal;
	Color color;
    //等等

	Fragment()
	{
		x = 0;
		y = 0;
		depth = 0.0f;
		normal = Vector3f();
		color = Color();
	}
};