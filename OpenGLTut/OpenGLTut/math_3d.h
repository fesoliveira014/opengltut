#ifndef MATH_3D_H
#define MATH_3D_H

#include <cstdio>
#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#else
#include <math.h>
#endif

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f()
	{

	}

	Vector3f(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f Cross(const Vector3f& v) const;

	Vector3f& Normalize();

	void print() const
	{
		printf("(%0.02f, %0.02f, %0.02f)", x, y, z);
	}


};

class Matrix4f
{
public:
	float m[4][4];

	Matrix4f()
	{
	}

	inline void InitIdentify()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f; 
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	inline Matrix4f operator*(const Matrix4f& Right) const
	{
		Matrix4f Ret;

		for (unsigned int i = 0; i < 4; i++){
			for (unsigned int j = 0; j < 4; j++) {
				Ret.m[i][j] = m[i][0] * Right.m[0][j] +
							  m[i][1] * Right.m[1][j] +
							  m[i][2] * Right.m[2][j] +
							  m[i][3] * Right.m[3][j];
			}
		}

		return Ret;
	}

	inline Matrix4f operator + (const Matrix4f& k)
	{
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				m[i][j] += k.m[i][j];
			}
		}

		return (*this);
	}

	inline Matrix4f operator - (const Matrix4f& k)
	{
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				m[i][j] -= k.m[i][j];
			}
		}

		return (*this);
	}

	void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
	void InitiTranslationTransform(float x, float y, float z);
	void InitCameraTransform(const Vector3f& target, const Vector3f& up);
	void InitPersProjTransform(float FOV, float width, float height, float zNear, float zFar);
};



#endif /* MATH_3D_H */