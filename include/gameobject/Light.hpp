#pragma once

#include"../math/Vector.hpp"
#include"../Color.hpp"
#include"GameObject.hpp"

class Light : public GameObject
{
public:
	Color color;
	float intensity;
};

class PointLight : public Light
{
public:

};

class DirectionalLight : public Light
{
public:
	Vector3f direction;
};