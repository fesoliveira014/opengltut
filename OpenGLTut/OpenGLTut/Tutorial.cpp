#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")

#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <gl/glew.h>
#include <GL/freeglut.h>

#include "ogldev_util.h"
#include "pipeline.h"

#define WINDOW_WIDHT 1024
#define WINDOW_HEIGHT 768

GLuint VBO; // Vertex buffer object
GLuint IBO; // Index buffer object
GLuint gWorldLocation;

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;

	Scale += 0.1f;

	Pipeline p;
	//p.Scale(1.1f, 1.0f, 1.1f);
	p.Rotate(0.0f, Scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 5.0f);
	p.SetProjection(30.0f, WINDOW_WIDHT, WINDOW_HEIGHT, 1.0f, 100.0f);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // 0 - Index of first vertex
														   // 3 - Number of vertices
														   // GL_FLOAT - Type of the vertices
														   // GL_FALSE - Indecates that there will be no normalization
														   // 0 - Stride; number between two instances of that attrib in buffer
														   // 0 - Offset
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Binds the Index Buffer

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); // Draw vertices using predifined indexes.

	glDisableVertexAttribArray(0); // Disables vertex attribute array after use

	glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
	// Calls RenderSceneCB as the display function
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB); // Calls an idle function. If this is a dedicated func, call glutPostRedisplay() at it's end
}

static void CreateVertexBuffer()
{
	Vector3f Vertices[4];
	Vertices[0] = Vector3f(-1.0f, -1.0f, 0.5773f);
	Vertices[1] = Vector3f(0.0f, -1.0f, -1.15475f);
	Vertices[2] = Vector3f(1.0f, -1.0f, 0.5773f);
	Vertices[3] = Vector3f(0.0f, 1.0f, 0.0f);

	glGenBuffers(1, &VBO); // Generates a single buffer, allocated in the driver and stored in the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
	unsigned int Indices[] = {  1, 2, 3,
								3, 0, 1,
								1, 2, 0,
								0, 3, 2 };

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType); // Creates a shader object

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	// Specifies the source code in a array of characters
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths); // ShaderObj - Object in which the source will be stored
											  // 1 - Number of slots in the character arrays
											  // p - Source file array
											  // Lenghts - Source file lengths array

	glCompileShader(ShaderObj); // Compiles the given shader

	// Checks the compilation for errors and returns the infolog
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success); 
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj); // Attach the compiled shader to a program object
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram(); // Creates a program object

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	string vs, fs;

	if (!ReadFile(pVSFileName, vs)) {
		exit(1);
	};

	if (!ReadFile(pFSFileName, fs)) {
		exit(1);
	};

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram); // Links program to driver
	
	// Checks program for linking errors
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram); // Checks if the program can execute 
									  // given the current pipeline
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, GL_VALIDATE_STATUS, NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld"); // Queries the program object for the uniform variable
	assert(gWorldLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Initializes GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // Configures GLUT options
	glutInitWindowSize(WINDOW_WIDHT, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial");

	InitializeGlutCallbacks();

	// Initializes GLEW and checks for errors
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders();

	glutMainLoop();

	return 0;
}