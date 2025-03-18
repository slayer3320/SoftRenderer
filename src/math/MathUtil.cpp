#include "math/MathUtil.hpp"

namespace MATH
{
    int Max(int a, int b)
    {
        return a > b ? a : b;
    }

    int Max(int a, int b, int c)
    {
        return Max(Max(a, b), c);
    }

    int Min(int a, int b)
    {
        return a < b ? a : b;
    }

    int Min(int a, int b, int c)
    {
        return Min(Min(a, b), c);
    }

    bool IsSameSign(const float f1, const float f2, const float f3)
    {
        return (f1 > 0 && f2 > 0 && f3 > 0) || (f1 < 0 && f2 < 0 && f3 < 0);
    }

    bool IsInsideTriangle(float x, float y, const Vector3f& v1, const Vector3f& v2, const Vector3f& v3)
    {
        // Vector2f v12 = Vector2f(v2.x - v1.x, v2.y - v1.y);
        // Vector2f v23 = Vector2f(v3.x - v2.x, v3.y - v2.y);
        // Vector2f v31 = Vector2f(v1.x - v3.x, v1.y - v3.y);

        // Vector2f v1p = Vector2f(x - v1.x, y - v1.y);
        // Vector2f v2p = Vector2f(x - v2.x, y - v2.y);
        // Vector2f v3p = Vector2f(x - v3.x, y - v3.y);

        // return IsSameSign(v12.x * v1p.y - v12.y * v1p.x, v23.x * v2p.y - v23.y * v2p.x, v31.x * v3p.y - v31.y * v3p.x);

        //不声明变量的写法
        return IsSameSign((v2.x - v1.x) * (y - v1.y) - (v2.y - v1.y) * (x - v1.x), 
                          (v3.x - v2.x) * (y - v2.y) - (v3.y - v2.y) * (x - v2.x), 
                          (v1.x - v3.x) * (y - v3.y) - (v1.y - v3.y) * (x - v3.x));
    }

    Vector3f GetBarycentricCoord(float x, float y, const Vector3f& v1, const Vector3f& v2, const Vector3f& v3)
    {
        Vector3f v0(x, y, 0.0f);
        Vector3f e1 = v2 - v1;
        Vector3f e2 = v3 - v2;
        Vector3f e3 = v1 - v3;

        float area = Cross(e1, e2).z;

        Vector3f cross1 = Cross(e1, v0 - v1);
        Vector3f cross2 = Cross(e2, v0 - v2);
        Vector3f cross3 = Cross(e3, v0 - v3);

        float alpha = cross2.z / area;
        float beta = cross3.z / area;
        float gamma = cross1.z / area;

        return Vector3f(alpha, beta, gamma);
    }

    Vector4f GetPlaneEquationParameters(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3)
    {

        Vector3f normal = Cross(v2 - v1, v3 - v1);
        normal.Normalize();
        float d = -Dot(normal, v1);
        return Vector4f(normal.x, normal.y, normal.z, d);
    }
}