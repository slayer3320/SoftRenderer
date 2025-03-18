#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

#include "../math/Vector.hpp"
#include "../math/Matrix.hpp"
#include "../math/MathUtil.hpp"
#include "../math/MinHeap.hpp"
#include "Component.hpp"
//包括顶点，法线，uv坐标

struct Vertex
{
	Vector3f pos;
	Vector2f texcoord;
	Vector3f normal;
};

struct Edge
{
	unsigned int v1, v2;

	Edge(unsigned int v1, unsigned int v2) : v1(v1), v2(v2) {}
};

struct Face
{
	unsigned int v1, v2, v3;

	Face(unsigned int v1, unsigned int v2, unsigned int v3) : v1(v1), v2(v2), v3(v3) {}
};

class Mesh : public Component
{
public:
	//原始数据 original data
	std::vector<Vertex> originalVertices;
	std::vector<Edge> originalEdges;
	std::vector<Face> originalFaces;
	std::unordered_map<unsigned int, std::vector<unsigned int>> originalvertex2face;
	std::unordered_map<unsigned int, std::vector<unsigned int>> originalvertex2edge;
	//渲染数据 render data
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	std::vector<Face> faces;
	std::unordered_map<unsigned int, std::vector<unsigned int>> vertex2face;
	std::unordered_map<unsigned int, std::vector<unsigned int>> vertex2edge;


	bool isSimplification = true;

	Mesh();
	~Mesh();
	void LoadMesh(std::string path);
	void Update() override;

	void Subdivision();
	void Simplification();

	void Print();
};