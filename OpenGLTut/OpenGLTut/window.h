#include "glut_window.h"
#include "camera.h"

class Window : public GlutWindow
{
public:
	Camera* pGameCamera;

	Window(int x0, int y0, int w, int h, const char* caption) : GlutWindow(x0, y0, w, h, caption)
	{
		if (!GLEW_VERSION_3_3)
		{
			printf("OpenGL 3.3 not supported.\n");
			exit(1);
		}

		pGameCamera = new Camera(w, h);
	}

	virtual void redisplay()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void mousePassiveMotion(int x, int y)
	{
		pGameCamera->OnMouse(x, y);
	}

	virtual void specialKeyUp(int key, int x, int y)
	{
		pGameCamera->OnKeyboard(key);
	}
};