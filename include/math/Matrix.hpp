#pragma once

#include "Vector.hpp"

template<typename T, int N>
class Matrix
{
public:
	T value[N][N];

	Matrix()
	{
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				value[i][j] = 0;
	}

	Matrix(T value[N][N])
	{
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				this->value[i][j] = value[i][j];
	}

	Matrix operator + (const Matrix& right) const
	{
		Matrix result;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				result.value[i][j] = value[i][j] + right.value[i][j];
		return result;
	}

	Matrix operator - (const Matrix& right) const
	{
		Matrix result;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				result.value[i][j] = value[i][j] - right.value[i][j];
		return result;
	}

	Matrix operator * (const Matrix& right) const
	{
		Matrix result;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				for (int k = 0; k < N; k++)
					result.value[i][j] += value[i][k] * right.value[k][j];
		return result;
	}

	Vector<T, N> operator * (const Vector<T, N>& right) const
	{
		Vector<T, N> result;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				result.data[i] += value[i][j] * right.data[j];
		return result;
	}

	Matrix operator * (const T right) const
	{
		Matrix result;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				result.value[i][j] = value[i][j] * right;
		return result;
	}

	void Print()
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
				std::cout << value[i][j] << " ";
			std::cout << std::endl;
		}
	}
};

template<typename T, int N> std::ostream& operator<<(std::ostream& os, const Matrix<T, N>& m)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			os << m.value[i][j] << " ";
		os << std::endl;
	}
	return os;
}

typedef Matrix<float, 2> Matrix2x2f;
typedef Matrix<float, 3> Matrix3x3f;
typedef Matrix<float, 4> Matrix4x4f;


