#pragma once

#include "math/Vector.hpp"

class Color
{
public:
	union
	{
		float data[4];
		struct
		{
			float r, g, b, a;
		};
	};

	Color();
	Color(float r, float g, float b, float a = 1.0f);

	Color operator+(const Color& color) const;

	Color operator*(const Color& color) const;
	Color operator*(float scalar) const;
};