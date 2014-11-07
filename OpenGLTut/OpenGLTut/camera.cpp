#include <GL\freeglut.h>

#include "camera.h"

const static float STEP_SCALE = 0.1f;
const static int MARGIN = 10;

Camera::Camera(int WindowWidth, int WindowHeight)
{
	m_windowHeight = WindowHeight;
	m_windowWidth  = WindowWidth;
	m_pos		   = Vector3f(0.0f, 0.0f, 0.0f);
	m_target	   = Vector3f(0.0f, 0.0f, 1.0f);
	m_up		   = Vector3f(0.0f, 1.0f, 0.0f);

	Init();
}

Camera::Camera(int WindowWidth, int WindowHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up)
{
	m_pos = pos;
	m_windowHeight = WindowHeight;
	m_windowWidth = WindowWidth;

	m_target = target;
	m_target.Normalize();

	m_up = up;
	m_up.Normalize();

	Init();
}

void Camera::Init()
{
    Vector3f HTarget(m_target.x, 0.0, m_target.z);
    HTarget.Normalize();
    
    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
        }
        else
        {
            m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = ToDegree(asin(-HTarget.z));
        }
        else
        {
            m_AngleH = 90 + ToDegree(asin(-HTarget.z));
        }
    }
    
    m_AngleV = -ToDegree(asin(m_target.y));

    m_onUpperEdge = false;
    m_onLowerEdge = false;
    m_onLeftEdge  = false;
    m_onRightEdge = false;
    m_mousePos.x  = m_windowWidth / 2;
    m_mousePos.y  = m_windowHeight / 2;

    glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

bool Camera::OnKeyboard(int Key)
{
	bool ret = false;

	switch (Key)
	{
	case GLUT_KEY_UP:
		m_pos += (m_target * STEP_SCALE);
		ret = true;
		break;
	case GLUT_KEY_DOWN:
	{
		m_pos -= (m_target * STEP_SCALE);
		ret = true; 
	}
		break;
	case GLUT_KEY_LEFT:
	{	
		Vector3f left = m_target.Cross(m_up);
		left.Normalize();
		left *= STEP_SCALE;
		m_pos += left;
		ret = true;
	}
		break;
	case GLUT_KEY_RIGHT:
	{
		Vector3f right = m_up.Cross(m_target);
		right.Normalize();
		right *= STEP_SCALE;
		m_pos += right;
		ret = true;
	}
		break;
	}

	return ret;
}

void Camera::OnMouse(int x, int y)
{
	const int DeltaX = x - m_mousePos.x;
	const int DeltaY = y - m_mousePos.y;

	m_mousePos.x = x;
	m_mousePos.y = y;

	m_AngleH += (float)DeltaX / 20.0f;
	m_AngleV += (float)DeltaY / 20.0f;

	if (DeltaX == 0) {
		if (x <= MARGIN) {
			m_onLeftEdge = true;
		}
		else if (x >= (m_windowWidth - MARGIN)) {
			m_onRightEdge = true;
		}
	}
	else {
		m_onLeftEdge = false;
		m_onRightEdge = false;
	}

	if (DeltaY == 0) {
		if (y <= MARGIN) {
			m_onUpperEdge = true;
		}
		else if (y >= (m_windowHeight - MARGIN)) {
			m_onLowerEdge = true;
		}
	}
	else {
		m_onUpperEdge = false;
		m_onLowerEdge = false;
	}

	Update();
}

void Camera::onRender()
{
	bool ShouldUpdate = false;

	if (m_onLeftEdge) {
		m_AngleH -= 0.1f;
		ShouldUpdate = true;
	}
	else if (m_onRightEdge) {
		m_AngleH += 0.1f;
		ShouldUpdate = true;
	}

	if (m_onUpperEdge){
		// Checks if minimum vertical angle
		if (m_AngleV > -90.0f) {
			m_AngleV -= 0.1f;
			ShouldUpdate = true;
		}
		
	}
	else if (m_onLowerEdge) {
		// Checks if maximum vertical angle
		if (m_AngleV < 90.0f) {
			m_AngleV += 0.1f;
			ShouldUpdate = true;
		}
	}

	if (ShouldUpdate) {
		Update();
	}
}

void Camera::Update() 
{
	const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

	// Rotates the view vector by the horizontal angle around the vertical axis
	Vector3f View(1.0f, 0.0f, 0.0f);
	View.Rotate(m_AngleH, Vaxis);
	View.Normalize();

	// Rotates the view vector by the horizontal angle around the horizontal axis
	Vector3f Haxis = Vaxis.Cross(View);
	Haxis.Normalize();
	View.Rotate(m_AngleV, Haxis);
	View.Normalize();

	m_target = View;
	m_target.Normalize();

	m_up = m_target.Cross(Haxis);
	m_up.Normalize();

}