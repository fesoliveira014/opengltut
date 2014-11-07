#include "glut_window.h"
#include "camera.h"

class Window : public GlutWindow
{
public:
	Camera* pGameCamera;

	void (*idleCB) (void);
	void (*redisplayCB) (void);

	Window(int x0, int y0, int w, int h, const char* caption) : GlutWindow(x0, y0, w, h, caption)
	{
		if (!GLEW_VERSION_3_3)
		{
			printf("OpenGL 3.3 not supported.\n");
			exit(1);
		}
	}

	virtual void mousePassiveMotion(int x, int y)
	{
		pGameCamera->OnMouse(x, y);
	}

	virtual void specialKeyUp(int key, int x, int y)
	{
		pGameCamera->OnKeyboard(key);
	}

	void getIdleCB(void(*callback)(void))
	{
		idleCB = callback;
	}

	void getRedisplayCB(void(*callback)(void))
	{
		redisplayCB = callback;
	}

	virtual void idle()
	{
		idleCB();
	}

	virtual void redisplay()
	{
		redisplayCB();
	}



};