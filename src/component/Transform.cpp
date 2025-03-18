#include "component/Transform.hpp"

Transform::Transform()
{
	position = Vector3f(0.0f, 0.0f, 0.0f);
	rotation = Vector3f(0.0f, 0.0f, 0.0f);
	scale = Vector3f(1.0f, 1.0f, 1.0f);

	float rotateArray[3][3] = 
	{
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1}
	};
	rotationMatrix = Matrix3x3f(rotateArray);
}

Transform::~Transform()
{
}

void Transform::Update()
{
	
}
