#include "Vector.hpp"

namespace MATH
{
	int Max(int a, int b);
	int Max(int a, int b, int c);

	int Min(int a, int b);
	int Min(int a, int b, int c);

	bool IsSameSign(const float f1, const float f2, const float f3);

	bool IsInsideTriangle(float x, float y, const Vector3f& v1,const Vector3f& v2, const Vector3f& v3);

	Vector3f GetBarycentricCoord(float x, float y, const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);

	Vector4f GetPlaneEquationParameters(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3);


}
