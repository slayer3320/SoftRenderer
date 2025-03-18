#pragma once

#include "ShaderData.hpp"

#include "../component/Transform.hpp"
#include "../Scene.hpp"

class IShader
{
public:
	ShaderData* shaderData;

	virtual Shader_Struct_v2f* VertexShader(Shader_Struct_a2v* a2v) = 0;
	virtual Color FragmentShader(Shader_Struct_v2f* v2f) = 0;

	void UpdateShaderData(Transform* modelTransform);

	Matrix4x4f GetModelMatrix(Transform* modelTransform);
	Matrix4x4f GetViewMatrix(Camera* camera);
	Matrix4x4f GetProjectionMatrix(Camera* camera);

	Vector3f ObjectToClipPos(Vector3f pos);
	Vector3f ObjectToWorldPos(Vector3f pos);
	Vector3f ObjectToWorldNormal(Vector3f normal);
};

class DefaultShader : public IShader
{
public:
	Shader_Struct_v2f* VertexShader(Shader_Struct_a2v* a2v) override;
	Color FragmentShader(Shader_Struct_v2f* v2f) override;
};

class BlinnPhongShader : public IShader
{
public:
	Shader_Struct_v2f* VertexShader(Shader_Struct_a2v* a2v) override;
	Color FragmentShader(Shader_Struct_v2f* v2f) override;
};