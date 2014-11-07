#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")

#include <map>
#include <cstdio>

#include "3rd party/glew-1.11.0/include/GL/glew.h"
#ifdef _WIN32
#include "3rd party/glew-1.11.0/include/GL/wglew.h"
#else
#include "3rd party\glew-1.11.0\include\GL\glxew.h"
#endif

#include "glut_window.h"

#ifndef GLUT_WHEEL_UP_BUTTON
#define GLUT_WHEEL_UP_BUTTON 0x0003
#endif

#ifndef GLUT_WHEEL_DOWN_BUTTON
#define GLUT_WHEEL_DOWN_BUTTON 0x0004
#endif

static map<int, GlutWindow *> windows;
static bool init = false;

static void redisplayFunc()
{
	GlutWindow * window = windows[glutGetWindow()];

	window->redisplay();
	window->setTime();

	//glutSwapBuffers();
}

static void reshapeFunc(int w, int h)
{
	GlutWindow * window = windows[glutGetWindow()];

	window->reshape(w, h);
}

static void idleFunc()
{
	GlutWindow * window = windows[glutGetWindow()];
	window->idle();
}

static void mouseClickFunc(int button, int state, int x, int y)
{
	int modifier = glutGetModifiers();
	if (button == GLUT_WHEEL_UP_BUTTON)
	{
		windows[glutGetWindow()]->mouseWheel(1, 1, x, y);
		return;
	}

	if (button == GLUT_WHEEL_DOWN_BUTTON)
	{
		windows[glutGetWindow()]->mouseWheel(-1, -1, x, y);
		return;
	}

	windows[glutGetWindow()]->mouseClick(button, state, modifier, x, y);
}

static void mouseMotionFunc(int x, int y)
{
	windows[glutGetWindow()]->mouseMotion(x, y);
}

static void mousePassiveMotionFunc(int x, int y)
{
	windows[glutGetWindow()]->mousePassiveMotion(x, y);
}

static void wheelFunc(int button, int dir, int x, int y)
{
	windows[glutGetWindow()]->mouseWheel(button, dir, x, y);
}

static void keyTypedFunc(unsigned char ch, int x, int y)
{
	int modifier = glutGetModifiers();

	windows[glutGetWindow()]->keyTyped(ch, modifier, x, y);
}

static void keyReleasedFunc(unsigned char ch, int x, int y)
{
	int modifier = glutGetModifiers();

	windows[glutGetWindow()]->keyReleased(ch, modifier, x, y);
}

static void specialKeyFunc(int ch, int x, int y)
{
	int modifier = glutGetModifiers();

	windows[glutGetWindow()]->specialKey(ch, modifier, x, y);
}

static void specialKeyReleasedFunc(int ch, int x, int y)
{
	int modifier = glutGetModifiers();

	windows[glutGetWindow()]->specialKeyUp(ch, modifier, x, y);
}

GlutWindow::GlutWindow(int x0, int y0, int w, int h, const char* windowCaption) : caption(windowCaption)
{
	glutInitWindowPosition(x0, y0);
	glutInitWindowSize(width = w, height = h);

	id = glutCreateWindow(caption.c_str());

	windows[id] = this;

	if (!::init)
	{
		glewExperimental = GL_TRUE;
		::init = true;

		glewInit();
		glGetError();
	}

	glutDisplayFunc(redisplayFunc);
	glutReshapeFunc(reshapeFunc);
	glutIdleFunc(idleFunc);
	glutMouseFunc(mouseClickFunc);
	glutMotionFunc(mouseMotionFunc);
	glutPassiveMotionFunc(mousePassiveMotionFunc);
	glutKeyboardFunc(keyTypedFunc);
	glutKeyboardUpFunc(keyReleasedFunc);
	glutSpecialFunc(specialKeyFunc);
	glutSpecialUpFunc(specialKeyReleasedFunc);
	//glutIgnoreKeyRepeat(1);

	setTime();

	if (!GLEW_VERSION_3_3)
	{
		printf("OpenGL 3.3 not supported.\n");
		exit(1);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

GlutWindow::~GlutWindow()
{
	glutDestroyWindow(id);
	glutLeaveMainLoop();

	windows.erase(id);
}

void GlutWindow::init(int& argc, char ** argv, int major, int minor, int mode, bool debug)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(mode);
	glutInitContextVersion(major, minor);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | (debug ? GLUT_DEBUG : 0));

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();
}