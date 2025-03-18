#pragma once

#include "GameObject.hpp"
#include "../math/OcTree.hpp"

enum Projection
{
	Perspective,
	Orthographic
};

//摄像机看向z轴正方向
class Camera : public GameObject
{
public:
	Projection projection = Projection::Perspective;

	Matrix4x4f GetViewMatrix();
	Matrix4x4f GetProjectionMatrix();

	Frustum GetFrustum();

private:
	float fov = 60;
	float l = -0.173f;
	float r = 0.173f;
	float n = 0.3f;
	float f = 100;
	float b = -0.173f;
	float t = 0.173f;
};

