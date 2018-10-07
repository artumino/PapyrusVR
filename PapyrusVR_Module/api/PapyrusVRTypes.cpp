#include "PapyrusVRTypes.h"

namespace PapyrusVR
{
	const Vector3 Vector3::zero = { 0,0,0 };


	Vector3& operator-(Vector3 const& lhs, Vector3 const& rhs)
	{
		static Vector3 result;
		result.x = lhs.x - rhs.x;
		result.y = lhs.y - rhs.y;
		result.z = lhs.z - rhs.z;
		return result;
	};

	Vector3& operator+(Vector3 const& lhs, Vector3 const& rhs)
	{
		static Vector3 result;
		result.x = lhs.x + rhs.x;
		result.y = lhs.y + rhs.y;
		result.z = lhs.z + rhs.z;
		return result;
	};

	Vector3& operator*(Matrix34 const& lhs, Vector3 const& rhs)
	{
		static Vector3 transformedVector;

		float ax = lhs.m[0][0] * rhs.x;
		float by = lhs.m[0][1] * rhs.y;
		float cz = lhs.m[0][2] * rhs.z;

		float ex = lhs.m[1][0] * rhs.x;
		float fy = lhs.m[1][1] * rhs.y;
		float gz = lhs.m[1][2] * rhs.z;


		float ix = lhs.m[2][0] * rhs.x;
		float jy = lhs.m[2][1] * rhs.y;
		float kz = lhs.m[2][2] * rhs.z;

		transformedVector.x = ax + by + cz + lhs.m[0][3];
		transformedVector.y = ex + fy + gz + lhs.m[1][3];
		transformedVector.z = ix + jy + kz + lhs.m[2][3];

		return transformedVector;
	}

	Matrix33& operator*(Matrix33 const& lhs, Matrix33 const& rhs)
	{
		//O(n^3) :(
		static Matrix33 result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				result.m[i][j] = 0;
				for (int k = 0; k < 3; k++)
					result.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
			}
		return result;
	}

	Matrix34& Matrix34::operator+(Matrix34 const& rhs)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] += rhs.m[i][j];
		return *this;
	}

	Matrix34& Matrix34::operator-(Matrix34 const& rhs)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] -= rhs.m[i][j];
		return *this;
	}


	Matrix34& operator+(Matrix34 const& lhs, Matrix34 const& rhs)
	{
		static Matrix34 result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 4; j++)
				result.m[i][j] = lhs.m[i][j] + rhs.m[i][j];
		return result;
	}

	Matrix34& operator-(Matrix34 const& lhs, Matrix34 const& rhs)
	{
		static Matrix34 result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 4; j++)
				result.m[i][j] = lhs.m[i][j] - rhs.m[i][j];
		return result;
	}

	Matrix33& Matrix33FromTransform(Matrix34 const* matrix)
	{
		static Matrix33 result = Matrix33(matrix->m[0][0], matrix->m[0][1], matrix->m[0][2],
			matrix->m[1][0], matrix->m[1][1], matrix->m[1][2],
			matrix->m[2][0], matrix->m[2][1], matrix->m[2][2]);
		return result;
	}

	Matrix34& Matrix34FromRotation(Matrix33 const* matrix)
	{
		static Matrix34 result = Matrix34(matrix->m[0][0], matrix->m[0][1], matrix->m[0][2], 0,
			matrix->m[1][0], matrix->m[1][1], matrix->m[1][2], 0,
			matrix->m[2][0], matrix->m[2][1], matrix->m[2][2], 0);
		return result; 
	}
}