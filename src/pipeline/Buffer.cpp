#include "pipeline/Buffer.hpp"

#include<string.h>
#include<iostream>

Buffer::Buffer()
{
	// this->height = 720;
	// this->width = 1280;
    this->height = 1080;
	this->width = 1920;
	this->depthBuffer = new float[height * width];
    for(int i = 0; i < height * width; i++)
    {
        depthBuffer[i] = 999999.0f;
    }
    //std::fill_n(depthBuffer, sizeof depthBuffer, std::numeric_limits<float>::max());
	this->colorBuffer = new Color[height * width];
}

Buffer::~Buffer()
{
	delete[] depthBuffer;
	delete[] colorBuffer;
}

void Buffer::Refresh()
{
    Color clearColor = Color(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < height * width; i++)
    {
        depthBuffer[i] = 999999.0f;
        colorBuffer[i] = clearColor;
    }
    //std::fill_n(depthBuffer, sizeof depthBuffer, std::numeric_limits<float>::max());
}


int Buffer::GetHeight()
{
	return height;
}

int Buffer::GetWidth()
{
	return width;
}

void Buffer::SetDepth(int x, int y, float depth)
{
	depthBuffer[y * width + x] = depth;
}

float Buffer::GetDepth(int x, int y)
{
	return depthBuffer[y * width + x];
}

void Buffer::SetColor(int x, int y, Color color)
{
	colorBuffer[y * width + x] = color;
}

Color Buffer::GetColor(int x, int y)
{
	return colorBuffer[y * width + x];
}

unsigned char* Buffer::GetColorData(unsigned char* color_data)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Color color = colorBuffer[i * width + j];
            color_data[(i * width + j) * 4] = (unsigned char)(color.r * 255);
            color_data[(i * width + j) * 4 + 1] = (unsigned char)(color.g * 255);
            color_data[(i * width + j) * 4 + 2] = (unsigned char)(color.b * 255);
            color_data[(i * width + j) * 4 + 3] = (unsigned char)(color.a * 255);
        }
    }
    return color_data;
}

void Buffer::WriteToBMP(const std::string& filename)
{
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file) 
	{
        std::cerr << "Unable to open file for writing." << std::endl;
        return;
    }

    // BMP Header
    uint32_t file_size = 54 + 3 * width * height;  // 54 bytes for header + 3 bytes per pixel
    uint16_t file_type = 0x4D42;                  // BM
    uint32_t reserved = 0x0000;
    uint32_t offset = 54;                         // Pixel data offset
    uint32_t dib_header_size = 40;                // DIB Header size
    uint16_t planes = 1;
    uint16_t bits_per_pixel = 24;
    uint32_t compression = 0;
    uint32_t image_size = width * height * 3;
    int32_t x_pixels_per_meter = 0;
    int32_t y_pixels_per_meter = 0;
    uint32_t colors_used = 0;
    uint32_t important_colors = 0;

    // Write the BMP header
    file.write((char*)&file_type, sizeof(file_type));
    file.write((char*)&file_size, sizeof(file_size));
    file.write((char*)&reserved, sizeof(reserved));
    file.write((char*)&offset, sizeof(offset));
    file.write((char*)&dib_header_size, sizeof(dib_header_size));
    file.write((char*)&width, sizeof(width));
    file.write((char*)&height, sizeof(height));
    file.write((char*)&planes, sizeof(planes));
    file.write((char*)&bits_per_pixel, sizeof(bits_per_pixel));
    file.write((char*)&compression, sizeof(compression));
    file.write((char*)&image_size, sizeof(image_size));
    file.write((char*)&x_pixels_per_meter, sizeof(x_pixels_per_meter));
    file.write((char*)&y_pixels_per_meter, sizeof(y_pixels_per_meter));
    file.write((char*)&colors_used, sizeof(colors_used));
    file.write((char*)&important_colors, sizeof(important_colors));

    // Write pixel data
    for (int y = height - 1; y >= 0; y--) 
    {
        for (int x = 0; x < width; x++) 
        {
            Color& color = colorBuffer[y * width + x];
            uint8_t r = static_cast<uint8_t>(color.r * 255);
            uint8_t g = static_cast<uint8_t>(color.g * 255);
            uint8_t b = static_cast<uint8_t>(color.b * 255);
            file.write((char*)&b, sizeof(b));  // BMP format uses BGR
            file.write((char*)&g, sizeof(g));
            file.write((char*)&r, sizeof(r));
        }
    }

    file.close();
}