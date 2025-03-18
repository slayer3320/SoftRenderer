#pragma once

#include "Component.hpp"

class Renderer : public Component
{
public:
	Renderer();
	~Renderer();
	void Update() override;
};

//class Renderer
//{
//public:
//	BMP _bmp;
//	int _screenWidth;
//	int _screenHeight;
//	/// <summary>
//	/// 注意！BMP的像素顺序的BGR
//	/// </summary>
//	std::vector<uint8_t> _imageColor;
//
//	Renderer(int screenWidth, int screenHeight);
//	void DoRender(const std::string& filename);
//	Vector3f CanonicalCubeToScreenCoord(const Vector3f& point);
//
//	void DrawPoint(const Vector3f& point);
//	void DrawPoint(const Vector3f& point, const Color& color);
//
//	//DDA数值微分算法
//	void DrawLine1(const Vector3f& point1, const Vector3f& point2, const Color& color);
//	void DrawLine1(const int x1, const int y1, const int x2, const int y2, const Color& color);
//	//中点Bresenham算法
//	void DrawLine2(const Vector3f& point1, const Vector3f& point2, const Color& color);
//	
//	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
//	void RasterizeTriangle(Vertex v1, Vertex v2, Vertex v3, BlinnPhongShader* shader, Buffer* zbuffer);
//
//	void DrawByIndex(Mesh* m, BlinnPhongShader* shader, Buffer* zbuffer);
//
//	Vector3i ScreenCoordToPixel(const Vector3f& screenCoord);
//	void SetPixel(const float x, const float y, const Color& color);
//};
