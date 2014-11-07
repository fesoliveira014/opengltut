#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")

#ifndef _GLUT_WINDOW_
#define _GLUT_WINDOW_

#include "3rd party/glew-1.11.0/include/GL/glew.h"
#ifdef _WIN32
#include "3rd party/glew-1.11.0/include/GL/wglew.h"
#else
#include "3rd party\glew-1.11.0\include\GL\glxew.h"
#endif

#include <string>
#include <GL/freeglut.h>

using namespace std;

class GlutWindow
{
	int width, height;
	string caption;
	int id;
	float time;

public:
	GlutWindow(int x0, int y0, int w, int h, const char* caption);
	~GlutWindow();

	int getWidth() const
	{
		return width;
	}

	int getHeight() const
	{
		return height;
	}

	const string& getCaption() const
	{
		return caption;
	}

	float getDeltaTime() const
	{
		return time - 0.001f * glutGet(GLUT_ELAPSED_TIME);
	}

	float getTime() const
	{
		return 0.001f * glutGet(GLUT_ELAPSED_TIME);
	}

	void setTime()
	{
		time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
	}

	virtual void redisplay() {}

	virtual void reshape(int w, int h)
	{
		width = w;
		height = h;
	}

	virtual void mouseClick(int button, int state, int modifiers, int x, int y) {}
	virtual void mouseWheel(int wheel, int modifier, int x, int y) {}
	virtual void mouseMotion(int x, int y) {}
	virtual void mousePassiveMotion(int x, int y) {}
	virtual void keyTyped(unsigned char key, int modifier, int x, int y)
	{
		if (key == 27 || key == 'q' || key == 'Q')
			exit(0);
	}

	virtual void keyReleased(unsigned char c, int modifiers, int x, int y) {}
	virtual void specialKey(int key, int modifier, int x, int y) {}
	virtual void specialKeyUp(int key, int modifier, int x, int y) {}

	virtual void idle()
	{
		glutPostRedisplay();
	}

	static void run()
	{
		glutMainLoop();
	}

	static void init(int& argc, char** argv, int major = 4, int minor = 2,
					 int mode = GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH, bool debug = true);
};
#endif