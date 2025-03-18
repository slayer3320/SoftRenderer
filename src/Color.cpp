#include "Color.hpp"

Color::Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    a = 1.0f;
}

Color::Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color Color::operator+(const Color& color) const
{
    return Color((r + color.r) > 1.0f ? 1.0f : (r + color.r), (g + color.g) > 1.0f ? 1.0f : (g + color.g), (b + color.b) > 1.0f ? 1.0f : (b + color.b), (a + color.a) > 1.0f ? 1.0f : (a + color.a));
}

Color Color::operator*(const Color& color) const
{
    return Color(r * color.r, g * color.g, b * color.b, a * color.a);
}

Color Color::operator*(float scalar) const
{
    return Color(r * scalar, g * scalar, b * scalar, a * scalar);
}