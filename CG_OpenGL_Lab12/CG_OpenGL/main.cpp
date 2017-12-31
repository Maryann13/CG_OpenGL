#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma comment(lib, "glew32.lib")

#include <GL/glew/glew.h>
#include <GL/freeglut/freeglut.h>

#include <iostream>

int w = 600, h = 600;
//! ID Vertex Buffer Object
GLuint VBO_vertex;
GLuint VBO_color;
GLuint VBO_element;

GLuint VBO2_vertex;
GLuint VBO2_element;

GLint Indices_count;
GLint Indices2_count;

//! Вершина
struct vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

//! Проверка ошибок OpenGL, если есть то вывод в консоль тип ошибки
void checkOpenGLerror()
{
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << gluErrorString(errCode);
}

//! Инициализация VBO
void initVBO_vertex()
{
	glGenBuffers(1, &VBO_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	vertex vertices[] = {
		{ -1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f }
	};
	//! Передаем вершины в буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	checkOpenGLerror();
}

void initVBO2_vertex()
{
	glGenBuffers(1, &VBO2_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2_vertex);
	vertex vertices[] = {
		{ 5.0f, -2.83f,  0.0f },
		{ 3.0f, 0.0f, -2.0f },
		{ 7.0f, 0.0f, -2.0f },
		{ 7.0f,  0.0f, 2.0f },
		{ 3.0f,  0.0f, 2.0f },		
		{ 5.0f, 2.83f,  0.0f }
	};
	//! Передаем вершины в буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	checkOpenGLerror();
}

void initVBO_color()
{
	glGenBuffers(1, &VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	vertex colors[] = {
		{ 1.0f, 0.6f, 0.2f },
		{ 0.1f, 0.8f, 0.1f },
		{ 0.0f, 1.0f, 0.0f },		
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.5f, 1.0f, 0.5f }
	};
	//! Передаем вершины в буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	checkOpenGLerror();
}

void initVBO_element()
{
	glGenBuffers(1, &VBO_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_element);
	GLint indices[] = {
		0, 4, 5, 0, 5, 1,
		1, 5, 6, 1, 6, 2,
		2, 6, 7, 2, 7, 3,
		3, 7, 4, 3, 4, 0,
		4, 7, 6, 4, 6, 5,
		3, 0, 1, 3, 1, 2
	};
	Indices_count = sizeof(indices) / sizeof(indices[0]);

	//! Передаем вершины в буфер
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	checkOpenGLerror();
}

void initVBO2_element()
{
	glGenBuffers(1, &VBO2_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO2_element);
	//! Вершины нашего треугольника
	GLint indices[] = {
		0, 1, 2, 0, 2, 3,
		0, 3, 4, 0, 4, 1,
		5, 1, 2, 5, 2, 3,
		5, 3, 4, 5, 4, 1
	};
	Indices2_count = sizeof(indices) / sizeof(indices[0]);

	//! Передаем вершины в буфер
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	checkOpenGLerror();
}

void Init() {
	glClearColor(0.43f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	initVBO_element();
	initVBO_vertex();
	initVBO2_element();
	initVBO2_vertex();
	initVBO_color();	
}

//! Освобождение буфера
void freeVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &VBO_vertex);
	glDeleteBuffers(1, &VBO_color);
	glDeleteBuffers(1, &VBO2_vertex);
	glDeleteBuffers(1, &VBO_element);
	glDeleteBuffers(1, &VBO2_element);
}

void resizeWindow(int width, int height) {
	glViewport(0, 0, width, height);
	height = height > 0 ? height : 1;
	const GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspectRatio, 1.0f, 200.0f);
	glTranslatef(-2.0f, 0.0f, -15.0f);
	glRotatef(40.0f, 1.0f, 1.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_element);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawElements(GL_TRIANGLES, Indices_count, GL_UNSIGNED_INT, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO2_element);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2_vertex);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawElements(GL_TRIANGLES, Indices2_count, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	checkOpenGLerror();

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(w, h);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("OpenGL");

	//! Обязательно перед инициализацией шейдеров
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status)
	{
		//! GLEW не проинициализировалась
		std::cout << "Error: " << glewGetErrorString(glew_status) << "\n";
		return 1;
	}

	//! Проверяем доступность OpenGL 2.0
	if (!GLEW_VERSION_2_0)
	{
		//! OpenGl 2.0 оказалась не доступна
		std::cout << "No support for OpenGL 2.0 found\n";
		return 1;
	}

	glutDisplayFunc(Render);
	glutReshapeFunc(resizeWindow);	
	Init();

	glutMainLoop();

	freeVBO();
}
