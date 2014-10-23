#include <GL\freeglut.h>

#include "camera.h"

const static float StepScale = 0.1f;

Camera::Camera()
{
	m_pos = Vector3f(0.0f, 0.0f, 0.0f);
	m_target = Vector3f(0.0f, 0.0f, 1.0f);
	m_target.Normalize();
	m_up = Vector3f(0.0f, 1.0f, 0.0f);
	m_up.Normalize();
}

Camera::Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up)
{
	m_pos = pos;
	m_target = target;
	m_up = up;
}

bool Camera::onKeyboard(int Key)
{
	bool ret = false;

	switch (Key)
	{
	case GLUT_KEY_UP:
		m_pos += (m_target * StepScale);
		ret = true;
		break;
	case GLUT_KEY_DOWN:
	{
		m_pos -= (m_target * StepScale);
		ret = true; 
	}
		break;
	case GLUT_KEY_LEFT:
	{	
		Vector3f left = m_target.Cross(m_up);
		left.Normalize();
		left *= StepScale;
		m_pos += left;
		ret = true;
	}
		break;
	case GLUT_KEY_RIGHT:
	{
		Vector3f right = m_up.Cross(m_target);
		right.Normalize();
		right *= StepScale;
		m_pos += right;
		ret = true;
	}
		break;
	}

	return ret;
}
