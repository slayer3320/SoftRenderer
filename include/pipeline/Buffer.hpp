#pragma once

#include <fstream>
#include <cstdint>
#include <algorithm>

#include "../Singleton.hpp"
#include "../Color.hpp"

class Buffer : public Singleton<Buffer>
{
public:
	int height, width;
	float* depthBuffer;
	Color* colorBuffer;

	Buffer();
	~Buffer();
	
	void Refresh();

	int GetHeight();
	int GetWidth();

	void SetDepth(int x, int y, float depth);
	float GetDepth(int x, int y);
	void SetColor(int x, int y, Color color);
	Color GetColor(int x, int y);

	unsigned char* GetColorData(unsigned char* color_data);

	void WriteToBMP(const std::string& filename);
};