#include "gameobject/Camera.hpp"

Matrix4x4f Camera::GetViewMatrix()
{
    Vector3f gazeDir = GetComponent<Transform>()->rotationMatrix * Vector3f(0.0f, 0.0f, 1.0f);
    Vector3f viewUpDir = GetComponent<Transform>()->rotationMatrix * Vector3f(0.0f, 1.0f, 0.0f);
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

    Vector3f position = GetComponent<Transform>()->position;
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

Matrix4x4f Camera::GetProjectionMatrix()
{
    if(projection == Projection::Orthographic)
    {
        // float l = -1;
        // float r = 1;
        // float n = 1;
        // float f = 100;
        // float b = -1;
        // float t = 1;
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
    else if(projection == Projection::Perspective)
    {
        // float l = -0.173f;
        // float r = 0.173f;
        // float n = 0.3f;
        // float f = 100;
        // float b = -0.173f;
        // float t = 0.173f;
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
    else
    {
        return Matrix4x4f();
    }
}

Frustum Camera::GetFrustum()
{
    Frustum frustum;
    frustum.SetPlane(0, 0.0f, 0.0f, -1.0f, 0.3f);  //n
    frustum.SetPlane(1, 0.0f, 0.0f, 1.0f, -100.0f);  //f
    frustum.SetPlane(2, -n, 0.0f, -l, 0.0f);  //l
    frustum.SetPlane(3, n, 0.0f, -r, 0.0f);  //r
    frustum.SetPlane(4, 0.0f, -n, -b, 0.0f);  //b
    frustum.SetPlane(5, 0.0f, n, -t, 0.0f);  //t
    return frustum;
}

//Camera* Camera::_mainCamera = nullptr;
//
//Camera::Camera()
//{
//
//}
//
//Camera* Camera::GetMainCamera()
//{
//    if (_mainCamera == nullptr)
//    {
//        _mainCamera = new Camera();
//    }
//
//    return _mainCamera;
//}
//
///// <summary>
///// M(view) = R(view)*T(view)
///// </summary>
///// <returns></returns>
//void Camera::CalculateViewMatrix()
//{
//    float translateArray[4][4] =
//    {
//        {1, 0, 0, -position._x},
//        {0, 1, 0, -position._y},
//        {0, 0, 1, -position._z},
//        {0, 0, 0, 1}
//    };
//    Matrix translateMatrix(translateArray);
//    //Rotate gazeDir to -Z, viewUpDir to Y, gazeDir x viewUpdir to X
//    //Consider its inverse rotation
//    //对于旋转矩阵来说，逆矩阵等于转置矩阵
//    //将XYZ轴旋转到UVW轴的旋转矩阵为
//    //{ u_x, u_y, u_z, 0 },    u_x为u轴在x轴上的分量
//    //{ v_x, v_y, v_z, 0 },    表示向量在x轴的方向上的投影
//    //{ w_x, w_y, w_z, 0 },
//    //{ 0, 0, 0, 1 }
//    //将点(x, y, z, 1)与旋转矩阵相乘得到坐标系的变换，得到(u, v, w, 1)
//    //Z -> -gazeDir  Y -> viewUpDir  X -> viewUpDir X -gazeDir == gazeDir X viewUpDir
//
//    Vector3f gazeMutiView = gazeDir.Cross(viewUpDir);
//    
//    viewUpDir.Standardization();
//    viewUpDir.Print();
//    gazeDir.Standardization();
//    gazeDir.Print();
//    gazeMutiView.Standardization();
//    gazeMutiView.Print();
//    float rotateArray[4][4] =
//    {
//        {gazeMutiView._x, gazeMutiView._y, gazeMutiView._z, 0},
//        {viewUpDir._x  , viewUpDir._y  , viewUpDir._z  , 0},
//        {-gazeDir._x   , -gazeDir._y   , -gazeDir._z   , 0},
//        {0, 0, 0, 1}
//    };
//    Matrix rotateMatrix(rotateArray);
//    std::cout << "translateMatrix" << std::endl;
//    translateMatrix.Print();
//    std::cout << "rotateMatrix" << std::endl;
//    rotateMatrix.Print();
//    Matrix viewMatrix =  rotateMatrix * translateMatrix;
//    _viewMatrix = viewMatrix;
//    std::cout << "_viewMatrix" << std::endl;
//    _viewMatrix.Print();
//}
//
//void Camera::CalculateOrthographicMatrix(float l, float r, float n, float f, float b, float t)
//{
//    float translationArray[4][4] =
//    {
//        {1, 0, 0, -(l + r) / 2},
//        {0, 1, 0, -(b + t) / 2},
//        {0, 0, 1, -(n + f) / 2},
//        {0, 0 ,0, 1}
//    };
//    Matrix translationMatrix(translationArray);
//
//    float scaleArray[4][4] =
//    {
//        {2 / (r - l), 0, 0, 0},
//        {0, 2 / (t - b), 0, 0},
//        {0, 0, 2 / (n - f), 0},
//        {0, 0, 0, 1}
//    };
//    Matrix scaleMatrix(scaleArray);
//
//    Matrix orthographicMatrix = scaleMatrix * translationMatrix;
//    _orthographicMatrix = orthographicMatrix;
//    std::cout << "_orthographicMatrix" << std::endl;
//    _orthographicMatrix.Print();
//}
//
//void Camera::CalculatePerspectiveMatrix(float l, float r, float n, float f, float b, float t)
//{
//    //float persepToNegOrthoArray[4][4] =
//    //{
//    //    {n, 0, 0, 0},
//    //    {0, n, 0, 0},
//    //    {0, 0, n + f, -n * f},
//    //    {0, 0, 1, 0}
//    //};
//    //Matrix persepToNegOrthoMatrix(persepToNegOrthoArray);
//    //CalculateOrthographicMatrix(l, r, n, f, b, t);
//
//    //_perspectiveMatrix = persepToNegOrthoMatrix * _orthographicMatrix;
//    //std::cout << "_perspectiveMatrix" << std::endl;
//    //_perspectiveMatrix.Print();
//    float perspectiveArray[4][4] =
//    {
//        {2 * n / (r - l), 0, (r + l) / (r - l), 0},
//        {0, 2 * n / (t - b), (t + b) / (t - b), 0},
//        {0, 0, -(f + n) / (f - n), -2 * f * n / (f - n)},
//        {0, 0, -1, 0}
//    };
//    Matrix perspectiveMatrix(perspectiveArray);
//
//    _perspectiveMatrix = perspectiveMatrix;
//    std::cout << "_perspectiveMatrix" << std::endl;
//    _perspectiveMatrix.Print();
//}
