#pragma once

#include <cmath>
#include <iostream>
#include <array>

template<typename T, int N>
class Vector
{
public:
	union
	{
		std::array<T, N> data;     
		struct { T x, y, z, w; };
	};

	Vector() : data{} {}

	template<typename... Args>
	Vector(Args... args) : data{ {args...} }
	{
		static_assert(sizeof...(args) == N, "The number of arguments is out of Vector");
	}

	T& operator[](int i) { return data[i]; }
	
	Vector<T, N> operator+(const Vector<T, N>& v) const
	{
		Vector<T, N> result;
		for (int i = 0; i < N; i++)
		{
			result[i] = data[i] + v.data[i];
		}
		return result;
	}

	Vector<T, N> operator-(const Vector<T, N>& v) const
    {
		Vector<T, N> result;
        for (int i = 0; i < N; ++i) 
        {
            result[i] = data[i] - v.data[i];
        }
        return result;
    }

    template<typename T1>
	Vector<T, N> operator*(T1 scalar) const
    {
		Vector<T, N> result;
		for (int i = 0; i < N; ++i)
		{
			result[i] = data[i] * scalar;
		}
		return result;
    }

	template<typename T1>
	Vector<T, N> operator/(T1 scalar) const
	{
		Vector<T, N> result;
		for (int i = 0; i < N; ++i)
		{
			result[i] = data[i] / scalar;
		}
		return result;
	}

	bool operator==(const Vector<T, N>& v) const
	{
		for (int i = 0; i < N; i++)
		{
			if (data[i] != v.data[i])
			{
				return false;
			}
		}
		return true;
	}

	Vector Normalize()
	{
		T length = 0;

		for (int i = 0; i < N; i++)
		{
			length += data[i] * data[i];
		}

		length = sqrt(length);
		
		for (int i = 0; i < N; i++)
		{
			data[i] /= length;
		}

		Vector result = *this;
		return result;
	}

	void Print() const
	{
		for (int i = 0; i < N; i++)
		{
			std::cout << data[i] << " ";
		}
		std::cout << std::endl;
	}
};

template<typename T, int N>
T Dot(const Vector<T, N>& v1, const Vector<T, N>& v2)
{
	T result = 0;
	for (int i = 0; i < N; i++)
	{
		result += v1.data[i] * v2.data[i];
	}
	return result;
}

template<typename T>
Vector<T, 3> Cross(const Vector<T, 3>& v1, const Vector<T, 3>& v2)
{
	Vector<T, 3> result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}


//转置向量乘向量
template<typename T, int N>
T TransposeMutiply(const Vector<T, N>& v1, const Vector<T, N>& v2)
{
	T result = 0;
	for (int i = 0; i < N; i++)
	{
		result += v1.data[i] * v2.data[i];
	}
	return result;
}


template<typename T, int N, int M> Vector<T, M> embed(const Vector<T, N>& v, T fill = 1) 
{
	Vector<T, M> result;
	for(int i = M; i--; result[i] = (i < N ? v[i] : fill));
	return result;
}

template<typename T, int N, int M> Vector<T, N> proj(const Vector<T, M>& v)
{
	Vector<T, N> result;
	for(int i = N; i--; result[i] = v[i]);
	return result;
}

template<typename T, int N>
std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v)
{
	for (int i = 0; i < N; i++)
	{
		os << v.data[i] << " ";
	}
	os << std::endl;
	return os;
}




typedef Vector<int, 2> Vector2i;
typedef Vector<float, 2> Vector2f;
typedef Vector<int, 3> Vector3i;
typedef Vector<float, 3> Vector3f;
typedef Vector<int, 4> Vector4i;
typedef Vector<float, 4> Vector4f;