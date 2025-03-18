#include "shader/Shader.hpp"

#include <iostream>
#include <random>
void IShader::UpdateShaderData(Transform* modelTransform)
{
    if(shaderData == nullptr)
    {
        shaderData = new ShaderData();
    }
    Scene* scene = Scene::GetInstance();
    Camera* camera = scene->GetCamera();
    Light* light = scene->GetLight();

	shaderData->model_matrix = GetModelMatrix(modelTransform);
    shaderData->view_matrix = GetViewMatrix(camera);
    shaderData->projection_matrix = GetProjectionMatrix(camera);

    shaderData->lightDir0 = (light->GetComponent<Transform>()->position - modelTransform->position).Normalize();
    shaderData->lightColor0 = light->color;
}

Matrix4x4f IShader::GetModelMatrix(Transform* modelTransform)
{
	float scaleArray[4][4] = 
	{
		{modelTransform->scale.x, 0, 0, 0},
		{0, modelTransform->scale.y, 0, 0},
		{0, 0, modelTransform->scale.z, 0},
		{0, 0, 0, 1}
	};
	Matrix4x4f scaleMatrix(scaleArray);

	float rotateArray[4][4] = 
	{
		{modelTransform->rotationMatrix.value[0][0], modelTransform->rotationMatrix.value[0][1], modelTransform->rotationMatrix.value[0][2], 0},
		{modelTransform->rotationMatrix.value[1][0], modelTransform->rotationMatrix.value[1][1], modelTransform->rotationMatrix.value[1][2], 0},
		{modelTransform->rotationMatrix.value[2][0], modelTransform->rotationMatrix.value[2][1], modelTransform->rotationMatrix.value[2][2], 0},
		{0, 0, 0, 1}
	};
	Matrix4x4f rotationMatrix(rotateArray);

	float translationArray[4][4] = 
	{
		{1, 0, 0, modelTransform->position.x},
		{0, 1, 0, modelTransform->position.y},
		{0, 0, 1, modelTransform->position.z},
		{0, 0, 0, 1}
	};
	Matrix4x4f translationMatrix(translationArray);

	return scaleMatrix * rotationMatrix * translationMatrix;
}

Matrix4x4f IShader::GetViewMatrix(Camera* camera)
{
	Vector3f gazeDir = camera->GetComponent<Transform>()->rotationMatrix * Vector3f(0.0f, 0.0f, 1.0f);
    Vector3f viewUpDir = camera->GetComponent<Transform>()->rotationMatrix * Vector3f(0.0f, 1.0f, 0.0f);
    Vector3f gazeMutiView = Cross(gazeDir, viewUpDir);
    viewUpDir.Normalize();
    gazeDir.Normalize();
    gazeMutiView.Normalize();
    float rotateArray[4][4] =
    {
        {gazeMutiView.x, gazeMutiView.y, gazeMutiView.z, 0},
        {viewUpDir.x  , viewUpDir.y  , viewUpDir.z  , 0},
        {-gazeDir.x   , -gazeDir.y   , -gazeDir.z   , 0},
        {0, 0, 0, 1}
    };
    Matrix4x4f rotateMatrix(rotateArray);

    Vector3f position = camera->GetComponent<Transform>()->position;
    float translateArray[4][4] =
    {
        {1, 0, 0, -position.x},
        {0, 1, 0, -position.y},
        {0, 0, 1, -position.z},
        {0, 0, 0, 1}
    };
    Matrix4x4f translateMatrix(translateArray);

    return rotateMatrix * translateMatrix;
}

Matrix4x4f IShader::GetProjectionMatrix(Camera* camera)
{
	if(camera->projection == Projection::Orthographic)
    {
        float l = -1;
        float r = 1;
        float n = 1;
        float f = 100;
        float b = -1;
        float t = 1;
        float translationArray[4][4] =
        {
            {1, 0, 0, -(l + r) / 2},
            {0, 1, 0, -(b + t) / 2},
            {0, 0, 1, -(n + f) / 2},
            {0, 0 ,0, 1}
        };
        Matrix4x4f translationMatrix(translationArray);

        float scaleArray[4][4] =
        {
            {2 / (r - l), 0, 0, 0},
            {0, 2 / (t - b), 0, 0},
            {0, 0, 2 / (n - f), 0},
            {0, 0, 0, 1}
        };
        Matrix4x4f scaleMatrix(scaleArray);

        return scaleMatrix * translationMatrix;
    }
    else if(camera->projection == Projection::Perspective)
    {
        float l = -1;
        float r = 1;
        float n = 1.0f;
        float f = 100;
        float b = -1;
        float t = 1;
        float perspectiveArray[4][4] =
        {
            {2 * n / (r - l), 0, (r + l) / (r - l), 0},
            {0, 2 * n / (t - b), (t + b) / (t - b), 0},
            {0, 0, -(f + n) / (f - n), -2 * f * n / (f - n)},
            {0, 0, -1, 0}
        };
        Matrix4x4f perspectiveMatrix(perspectiveArray);

        return perspectiveMatrix;
    }
    else return Matrix4x4f();
}

Vector3f IShader::ObjectToClipPos(Vector3f pos)
{
	Vector4f HC_pos = Vector4f(pos.x, pos.y, pos.z, 1.0f);
	Vector4f clip_pos = shaderData->projection_matrix * shaderData->view_matrix * shaderData->model_matrix * HC_pos;
	return Vector3f(clip_pos.x / clip_pos.w, clip_pos.y / clip_pos.w, clip_pos.z / clip_pos.w);
}

Vector3f IShader::ObjectToWorldPos(Vector3f pos)
{
	return Vector3f();
}

Vector3f IShader::ObjectToWorldNormal(Vector3f normal)
{
    Vector4f temp = (shaderData->model_matrix * Vector4f(normal.x, normal.y, normal.z, 0.0f));
    return Vector3f(temp.x, temp.y, temp.z);
}


Shader_Struct_v2f* DefaultShader::VertexShader(Shader_Struct_a2v* a2v)
{
	Shader_Struct_v2f* v2f = new Shader_Struct_v2f();
	v2f->pos = ObjectToClipPos(a2v->pos);
	return v2f;
}

Color DefaultShader::FragmentShader(Shader_Struct_v2f* v2f)
{
	return Color(1.0f, 1.0f, 1.0f, 1.0f);
}

//------------------------------BlinnPhongShader------------------------------

Shader_Struct_v2f* BlinnPhongShader::VertexShader(Shader_Struct_a2v* a2v)
{
    Shader_Struct_v2f* v2f = new Shader_Struct_v2f();
    v2f->pos = ObjectToClipPos(a2v->pos);
    Vector3f normal = ObjectToWorldNormal(a2v->normal);
    v2f->normal = normal;
    v2f->color = shaderData->lightColor0 * Color(1.0f, 1.0f, 1.0f) * std::max(0.0f, Dot(normal, shaderData->lightDir0)) + Color(0.1f, 0.1f, 0.1f, 1.0f);

    return v2f;
}

Color BlinnPhongShader::FragmentShader(Shader_Struct_v2f* v2f)
{
    return v2f->color;
}

//------------------------------BlinnPhongShader------------------------------

//BlinnPhongShader::BlinnPhongShader()
//{
//	_pointLight = nullptr;
//	_texture = nullptr;
//	_camera = Camera::GetMainCamera();
//}
//
//void BlinnPhongShader::Init(PointLight *pointLight, Texture *texture)
//{
//	_pointLight = pointLight;
//	_texture = texture;
//	_camera = Camera::GetMainCamera();
//}
//
//Color BlinnPhongShader::CalcuAmbientLighting(Vector3f point)
//{
//	float r = Vector3f::GetDistance(point, _pointLight->position);
//	return _texture->_ambientReflectionCoefficient * _pointLight->_intensity / (r * r);
//}
//
//Color BlinnPhongShader::CalcuDiffuseRelection(Vector3f point, Vector3f normal)
//{
//	float r = Vector3f::GetDistance(point, _pointLight->position);
//
//	Vector3f l = _pointLight->position - point;
//	
//	float enegyReceived = normal.ReturnStandardization().Dot(l.ReturnStandardization());
//	enegyReceived = (enegyReceived > 0 ? enegyReceived : 0);
//
//	return _texture->_diffuseReflectionCoefficient * _pointLight->_intensity / (r * r) * enegyReceived;
//}

