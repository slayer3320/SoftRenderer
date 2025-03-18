#include "component/Renderer.hpp"




//Renderer::Renderer(int screenWidth, int screenHeight)
//{
//	_screenWidth = screenWidth;
//	_screenHeight = screenHeight;
//	_imageColor = std::vector<uint8_t>(_screenWidth * _screenHeight * 3, 0);
//	_bmp = BMP(_screenWidth, _screenHeight, _imageColor);
//}
//
//void Renderer::DoRender(const std::string& filename)
//{
//	_bmp.saveBMP(filename, _imageColor);
//}
//
////屏幕坐标系和正则BOX的Y轴是反转的
////屏幕坐标系向后为X轴正方向，向下为Y轴正方向
////屏幕坐标X轴(0~_screenHeight-1)  Y轴(0~_screenWidth-1)
//Vector3f Renderer::CanonicalCubeToScreenCoord(const Vector3f& point)
//{
//	if (point._w == 0)
//	{
//		std::cout << "Parameter Not Point ERROR" << std::endl;
//		return Vector3f();
//	}
//	Vector3f standardPoint(point._x / point._w, point._y / point._w, point._z / point._w, point._w / point._w);
//	Vector3f screenCoord;
//	screenCoord._x = (standardPoint._x + 1) * 0.5f * (_screenHeight - 1);
//	screenCoord._y = (_screenWidth - 1) - (standardPoint._y + 1) * 0.5f * (_screenWidth - 1);
//	//screenCoord._z = 1 / point._z; //我也不知道为什么,但是这个地方肯定有问题
//	screenCoord._z = (point._z >= 0 ? 0x7fffffff : point._z);
//	screenCoord._w = 1;
//	return screenCoord;
//}
//
//void Renderer::DrawPoint(const Vector3f& point)
//{
//	DrawPoint(point, Color::White());
//}
//
///// <param name="point">正则箱坐标系下x坐标</param>
///// <param name="color">正则箱坐标系下y坐标</param>
///// <returns></returns>
//void Renderer::DrawPoint(const Vector3f& point, const Color& color)
//{
//	Vector3f screenCoord = CanonicalCubeToScreenCoord(point);
//	point.Print();
//	screenCoord.Print();
//	SetPixel(screenCoord._x, screenCoord._y, color);
//}
//
//void Renderer::DrawLine1(const Vector3f& point1, const Vector3f& point2, const Color& color)
//{
//	int posX1 = floor(CanonicalCubeToScreenCoord(point1)._x), posY1 = floor(CanonicalCubeToScreenCoord(point1)._y);
//	int posX2 = floor(CanonicalCubeToScreenCoord(point2)._x), posY2 = floor(CanonicalCubeToScreenCoord(point2)._y);
//	DrawLine1(posX1, posY1, posX2, posY2, color);
//}
//
//void Renderer::DrawLine1(const int x1, const int y1, const int x2, const int y2, const Color& color)
//{
//	//如果是同一个点的话会报错不知道为什么
//	int posX1 = x1, posY1 = y1;
//	int posX2 = x2, posY2 = y2;
//	int stPosX = 0, stPosY = 0, endPosX = 0, endPosY = 0;
//
//	std::cout << posX1 << " " << posY1 << " " << posX2 << " " << posY2 << std::endl;
//
//	float k;
//
//	if (posX1 > posX2)
//	{
//		k = float(posY1 - posY2) / float(posX1 - posX2);
//		stPosX = posX2, stPosY = posY2, endPosX = posX1, endPosY = posY1;
//		//if (k < 0) stPosX = posX2, stPosY = posY2, endPosX = posX1, endPosY = posY1;
//	}
//	else if (posX1 < posX2)
//	{
//		k = float(posY2 - posY1) / float(posX2 - posX1);
//		stPosX = posX1, stPosY = posY1, endPosX = posX2, endPosY = posY2;
//	}
//	else
//	{
//		stPosY = std::min(posY1, posY2);
//		endPosY = std::max(posY1, posY2);
//
//		do
//		{
//			SetPixel(posX1, stPosY, color);
//			stPosY++;
//		} while (stPosY != endPosY);
//
//		return;
//	}
//
//	std::cout << k << std::endl;
//	float posX = (float)stPosX, posY = (float)stPosY;
//	do
//	{
//		std::cout << stPosX << " " << stPosY << " " << endPosX << " " << endPosY << std::endl;
//		SetPixel(stPosX, stPosY, color);
//		if (k > 0)
//		{
//			if (k <= 1)
//			{
//				stPosX++;
//				posY += k;
//				stPosY = round(posY);
//			}
//			if (k > 1)
//			{
//				stPosY++;
//				posX += 1 / k;
//				stPosX = round(posX);
//			}
//		}
//		if (k < 0)
//		{
//			if (k <= -1)
//			{
//				stPosY--;
//				posX -= 1 / k;
//				stPosX = round(posX);
//			}
//			if (k > -1)
//			{
//				stPosX++;
//				posY += k;
//				stPosY = round(posY);
//			}
//		}
//	} while (stPosX != endPosX && stPosY != endPosY);
//
//	SetPixel(stPosX, stPosY, Color::Red());
//}
//
//void Renderer::DrawLine2(const Vector3f& point1, const Vector3f& point2, const Color& color)
//{
//	return void();
//}
//
//void Renderer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
//{
//	DrawLine1(x1, y1, x2, y2, Color::White());
//	DrawLine1(x1, y1, x3, y3, Color::White());
//	DrawLine1(x2, y2, x3, y3, Color::White());
//}
//
//void Renderer::RasterizeTriangle(Vertex v1, Vertex v2, Vertex v3, BlinnPhongShader* shader, Buffer* zbuffer)
//{
//	//TODO:这里应该要先进行模型变换
//
//	Vertex vv1(Camera::GetMainCamera()->_perspectiveMatrix * Camera::GetMainCamera()->_viewMatrix * v1._pos, v1._normal, v1._color, v1._uv);
//	Vertex vv2(Camera::GetMainCamera()->_perspectiveMatrix * Camera::GetMainCamera()->_viewMatrix * v2._pos, v2._normal, v2._color, v2._uv);
//	Vertex vv3(Camera::GetMainCamera()->_perspectiveMatrix * Camera::GetMainCamera()->_viewMatrix * v3._pos, v3._normal, v3._color, v3._uv);
//
//	vv1._pos.DotNormalization();
//	vv2._pos.DotNormalization();
//	vv3._pos.DotNormalization();
//
//	vv1._pos = CanonicalCubeToScreenCoord(vv1._pos);
//	vv2._pos = CanonicalCubeToScreenCoord(vv2._pos);  
//	vv3._pos = CanonicalCubeToScreenCoord(vv3._pos);
//
//	//现在是屏幕坐标
//	//注意这里是屏幕坐标系，top在下方,y数值较大
//	int bottom = MATH::Min(ScreenCoordToPixel(vv1._pos)._y, ScreenCoordToPixel(vv2._pos)._y, ScreenCoordToPixel(vv3._pos)._y);
//	int top = MATH::Max(ScreenCoordToPixel(vv1._pos)._y, ScreenCoordToPixel(vv2._pos)._y, ScreenCoordToPixel(vv3._pos)._y);
//	int left = MATH::Min(ScreenCoordToPixel(vv1._pos)._x, ScreenCoordToPixel(vv2._pos)._x, ScreenCoordToPixel(vv3._pos)._x);
//	int right = MATH::Max(ScreenCoordToPixel(vv1._pos)._x, ScreenCoordToPixel(vv2._pos)._x, ScreenCoordToPixel(vv3._pos)._x);
//
//	//std::cout << top << " " << bottom << " " << left << " " << right << std::endl;
//
//	//没考虑三个点在一个像素内的情况
//	//像素坐标转换成像素中心坐标，用中心来判断是否在三角形内部
//
//	for (int x = left; x <= right; ++x)
//	{
//		for (int y = bottom; y <= top; ++y)
//		{
//			float i = static_cast<float>(x) + 0.5f;
//			float j = static_cast<float>(y) + 0.5f;
//
//			if (MATH::IsInsideTriangle(i, j, vv1._pos, vv2._pos, vv3._pos))
//			{
//				Vector3f barycentricCoord = MATH::GetBarycentricCoord(i, j, vv1._pos, vv2._pos, vv3._pos);
//
//				//float zDepth = (vv1._pos * barycentricCoord._x + vv2._pos * barycentricCoord._y + vv3._pos * barycentricCoord._z)._z;
//				float zDepth = vv1._pos._z * barycentricCoord._x + vv2._pos._z * barycentricCoord._y + vv3._pos._z * barycentricCoord._z;
//
//				// TODO: 如果三角形过大超出屏幕范围需要处理
//				int bufferX = static_cast<int>(i - 0.5f);
//				int bufferY = static_cast<int>(j - 0.5f);
//
//				if (zbuffer->_depthBuffer[bufferX][bufferY] > zDepth)
//				{
//					zbuffer->_depthBuffer[bufferX][bufferY] = zDepth;
//					Vector3f colorVector(v1._color._RGBA * barycentricCoord._x + v2._color._RGBA * barycentricCoord._y + v3._color._RGBA * barycentricCoord._z);
//					Color color(colorVector._x, colorVector._y, colorVector._z, colorVector._w);
//					SetPixel(bufferX, bufferY, color);
//					//SetPixel(x, y, color);
//				}
//			}
//		}
//	}
//}
//
//void Renderer::DrawByIndex(Mesh* m, BlinnPhongShader* shader, Buffer* zbuffer)
//{
//	if (m->_indexBuffer.size() % 3 != 0)
//	{
//		std::cerr << "Error: Invalid number of indices in DrawByIndex." << std::endl;
//		return;
//	}
//
//	for (int i = 0; i < m->_indexBuffer.size(); i = i + 3)
//	{
//		Vertex p1;
//		p1._pos = m->positionBuffer[m->_indexBuffer[i]._x - 1];
//		p1._uv = m->_uvBuffer[m->_indexBuffer[i]._y - 1];
//		p1._normal = m->_normalBuffer[m->_indexBuffer[i]._z - 1];
//		//颜色连续相乘看起来才是对的，但是理论上应该系数相加再乘颜色
//		p1._color = Color(0.5f, 0.5f, 0.5f, 1) * (shader->CalcuAmbientLighting(p1._pos) + shader->CalcuDiffuseRelection(p1._pos, p1._normal));
//
//		Vertex p2;
//		p2._pos = m->positionBuffer[m->_indexBuffer[i + 1]._x - 1];
//		p2._uv = m->_uvBuffer[m->_indexBuffer[i + 1]._y - 1];
//		p2._normal = m->_normalBuffer[m->_indexBuffer[i + 1]._z - 1];
//		p2._color = Color(0.5f, 0.5f, 0.5f, 1) * (shader->CalcuAmbientLighting(p2._pos) + shader->CalcuDiffuseRelection(p2._pos, p2._normal));
//
//		Vertex p3;
//		p3._pos = m->positionBuffer[m->_indexBuffer[i + 2]._x - 1];
//		p3._uv = m->_uvBuffer[m->_indexBuffer[i + 2]._y - 1];
//		p3._normal = m->_normalBuffer[m->_indexBuffer[i + 2]._z - 1];
//		p3._color = Color(0.5f, 0.5f, 0.5f, 1) * (shader->CalcuAmbientLighting(p3._pos) + shader->CalcuDiffuseRelection(p3._pos, p3._normal));
//
//
//		//shader->CalcuAmbientLighting().Print();
//		//shader->CalcuDiffuseRelection(p3._pos, p3._normal).Print();
//		//std::cout << std::endl;
//		//p1._color.Print();
//		//p2._color.Print();
//		//p3._color.Print();
//		//std::cout << "---------------------------------" << std::endl;
//
//		RasterizeTriangle(p1, p2, p3, shader, zbuffer);
//	}
//}
//
//Vector3i Renderer::ScreenCoordToPixel(const Vector3f& screenCoord)
//{
//	return Vector3i((int)floor(screenCoord._x), (int)floor(screenCoord._y), (int)floor(screenCoord._z), (int)floor(screenCoord._w));
//}
//
///// <param name="x">屏幕坐标系下的x坐标</param>
///// <param name="y">屏幕坐标系下的y坐标</param>
///// <param name="color"></param>
///// <returns></returns>
//void Renderer::SetPixel(const float x, const float y, const Color& color)
//{
//	Vector3i intColor = color.Translate();
//	int ix = floor(x), iy = _screenWidth - 1 - floor(y);
//	_imageColor[(iy * _screenHeight + ix) * 3] = static_cast<uint8_t>(intColor._z);
//	_imageColor[(iy * _screenHeight + ix) * 3 + 1] = static_cast<uint8_t>(intColor._y);
//	_imageColor[(iy * _screenHeight + ix) * 3 + 2] = static_cast<uint8_t>(intColor._x);
//}
//

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Update()
{
}
