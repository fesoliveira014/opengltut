#ifndef CAMERA_H
#define CAMERA_H

#include "math_3d.h"

class Camera
{
public:

	Camera(int WindowWidth, int WindowHeight);

	Camera(int WindowWidth, int WindowHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up);

	bool OnKeyboard(int key);

	void OnMouse(int x, int y);

	void onRender();

	const Vector3f& getPos() const
	{
		return m_pos;
	}

	const Vector3f& getTarget() const
	{
		return m_target;
	}

	const Vector3f& getUp() const
	{
		return m_up;
	}

private:
	void Init();
	void Update();

	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;

	int m_windowWidth;
	int m_windowHeight;

	float m_AngleH;
	float m_AngleV;

	bool m_onUpperEdge;
	bool m_onLowerEdge;
	bool m_onLeftEdge;
	bool m_onRightEdge;

	Vector2i m_mousePos;
};

#endif /* CAMERA_H */