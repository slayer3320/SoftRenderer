#pragma once

#include "Component.hpp"
#include "../math/Vector.hpp"
#include "../math/Matrix.hpp"

class Transform : public Component
{
public:
	Vector3f position;
	Vector3f rotation;
	Matrix3x3f rotationMatrix;
	Vector3f scale;

	Transform();
	~Transform();
	void Update() override;

};