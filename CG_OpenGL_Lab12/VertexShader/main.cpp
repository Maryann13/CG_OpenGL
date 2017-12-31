#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma comment(lib, "glew32.lib")

#include <GL/glew/glew.h>
#include <GL/freeglut/freeglut.h>
#include <GL/glm/glm.hpp> 
#include <GL/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <locale>

using std::string;
using std::ifstream;

using glm::mat4;
using glm::vec3;

int w = 600, h = 600;
//! ID Vertex Buffer Object
GLuint VBO_vertex;
GLuint VBO_element;

GLuint VBO2_vertex;
GLuint VBO2_element;

GLint Indices_count;
GLint Indices2_count;

//! ID шейдерной программы
GLuint Program;
GLuint Program2;
//! ID атрибута
GLint  Attrib_vertex;

GLint Unif_projection;
GLint Unif_rotation;
GLint Unif_scale;

GLint Unif_color;
GLint Unif_color1;
GLint Unif_color2;
GLint Unif_strip_width;

GLfloat scale_x, scale_y;
GLfloat angle_x, angle_y, angle_z;

mat4 Matrix_projection;

//! Вершина
struct vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

//! Функция печати лога шейдера
void shaderLog(unsigned int shader)
{
	int   infologLen = 0;
	int   charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 1)
	{
		infoLog = new char[infologLen];
		if (infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate InfoLog buffer\n";
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		std::cout << "InfoLog: " << infoLog << "\n\n\n";
		delete[] infoLog;
	}
}

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

//! Инициализация шейдеров
void initShader(string const & vsfName, string const & fsfName, GLuint & program)
{
	string vssSource, fssSource;
	string vsLine, fsLine;
	ifstream vsfStream(vsfName);
	while (true)
	{
		getline(vsfStream, vsLine);
		if (!vsfStream)
			break;
		vssSource += vsLine + '\n';
	}
	vsfStream.close();

	ifstream fsfStream(fsfName);
	while (true)
	{
		getline(fsfStream, fsLine);
		if (!fsfStream)
			break;
		fssSource += fsLine + '\n';
	}
	fsfStream.close();

	const char* vsSource = vssSource.c_str();
	const char* fsSource = fssSource.c_str();

	//! Переменные для хранения идентификаторов шейдеров
	GLuint vShader, fShader;

	//! Создаем вершинный шейдер
	vShader = glCreateShader(GL_VERTEX_SHADER);
	//! Передаем исходный код
	glShaderSource(vShader, 1, &vsSource, NULL);
	//! Компилируем шейдер
	glCompileShader(vShader);

	std::cout << "vertex shader \n";
	shaderLog(vShader);

	//! Создаем фрагментный шейдер
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//! Передаем исходный код
	glShaderSource(fShader, 1, &fsSource, NULL);
	//! Компилируем шейдер
	glCompileShader(fShader);

	std::cout << "fragment shader \n";
	shaderLog(fShader);

	//! Создаем программу и прикрепляем шейдеры к ней
	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	//! Линкуем шейдерную программу
	glLinkProgram(program);

	//! Проверяем статус сборки
	int link_ok;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}
	checkOpenGLerror();
}

void Init() {
	glClearColor(0.43f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	initVBO_element();
	initVBO_vertex();
	initVBO2_element();
	initVBO2_vertex();

	scale_x = 1.0f;
	scale_y = 1.0f;

	angle_x = 0.0f;
	angle_y = 0.0f;
	angle_z = 0.0f;

	initShader("vsShader.txt", "fsShader1.txt", Program);
	initShader("vsShader.txt", "fsShader2.txt", Program2);
	//! Вытягиваем ID атрибута из собранной программы 
	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1)
	{
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}
	//! Вытягиваем ID юниформ
	const char* unif_name = "projection";
	Unif_projection = glGetUniformLocation(Program, unif_name);
	if (Unif_projection == -1)
	{
		std::cout << "could not bind uniform " << unif_name << std::endl;
		return;
	}
	const char* unif_name1 = "rot_matrix";
	Unif_rotation = glGetUniformLocation(Program, unif_name1);
	if (Unif_rotation == -1)
	{
		std::cout << "could not bind uniform " << unif_name1 << std::endl;
		return;
	}
	const char* unif_name2 = "scale_matrix";
	Unif_scale = glGetUniformLocation(Program, unif_name2);
	if (Unif_scale == -1)
	{
		std::cout << "could not bind uniform " << unif_name2 << std::endl;
		return;
	}
	Unif_color = glGetUniformLocation(Program, "color");
	if (Unif_color == -1)
	{
		std::cout << "could not bind uniform color" << std::endl;
		return;
	}
	Unif_color1 = glGetUniformLocation(Program2, "color1");
	if (Unif_color1 == -1)
	{
		std::cout << "could not bind uniform color1" << std::endl;
		return;
	}
	Unif_color2 = glGetUniformLocation(Program2, "color2");
	if (Unif_color2 == -1)
	{
		std::cout << "could not bind uniform color2" << std::endl;
		return;
	}
	Unif_strip_width = glGetUniformLocation(Program2, "strip_width");
	if (Unif_strip_width == -1)
	{
		std::cout << "could not bind uniform strip_width" << std::endl;
		return;
	}
	checkOpenGLerror();
}

//! Освобождение шейдеров
void freeShader()
{
	//! Передавая ноль, мы отключаем шейдрную программу
	glUseProgram(0);
	//! Удаляем шейдерную программу
	glDeleteProgram(Program);
	glDeleteProgram(Program2);
}

//! Освобождение буфера
void freeVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &VBO_vertex);
	glDeleteBuffers(1, &VBO2_vertex);
	glDeleteBuffers(1, &VBO_element);
	glDeleteBuffers(1, &VBO2_element);
}

void resizeWindow(int width, int height) {
	glViewport(0, 0, width, height);
	height = height > 0 ? height : 1;
	const GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
	
	Matrix_projection = glm::perspective(45.0f, aspectRatio, 1.0f, 200.0f);
	Matrix_projection = glm::translate(Matrix_projection, vec3(-2.0f, 0.0f, -12.0f));
	Matrix_projection = glm::rotate(Matrix_projection, 19.65f, vec3(1.0f, 1.0f, 0.0f));
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//! Устанавливаем шейдерную программу текущей
	glUseProgram(Program);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_element);

	//! Включаем массив атрибутов
	glEnableVertexAttribArray(Attrib_vertex);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_TRUE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mat4 rotation_mat = glm::rotate(mat4(1.0f), angle_x, vec3(1.0f, 0.0f, 0.0f));
	rotation_mat = glm::rotate(rotation_mat, angle_y, vec3(0.0f, 1.0f, 0.0f));
	rotation_mat = glm::rotate(rotation_mat, angle_z, vec3(0.0f, 0.0f, 1.0f));

	mat4 scale_mat = glm::scale(mat4(1.0f), vec3(scale_x, scale_y, 1.0f));

	glUniformMatrix4fv(Unif_projection, 1, GL_FALSE, &Matrix_projection[0][0]);
	glUniformMatrix4fv(Unif_rotation, 1, GL_FALSE, &rotation_mat[0][0]);
	glUniformMatrix4fv(Unif_scale, 1, GL_FALSE, &scale_mat[0][0]);

	static float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(Unif_color, 1, red);

	glDrawElements(GL_TRIANGLES, Indices_count, GL_UNSIGNED_INT, 0);
	
	glUseProgram(Program2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO2_element);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2_vertex);
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_TRUE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUniformMatrix4fv(Unif_projection, 1, GL_FALSE, &Matrix_projection[0][0]);
	glUniformMatrix4fv(Unif_rotation, 1, GL_FALSE, &rotation_mat[0][0]);
	glUniformMatrix4fv(Unif_scale, 1, GL_FALSE, &scale_mat[0][0]);

	static float dgreen[4] = { 0.1f, 0.6f, 0.1f, 1.0f };
	static float lgreen[4] = { 0.1f, 0.8f, 0.1f, 1.0f };
	static float swidth[2] = { 10.0f, 5.0f };
	glUniform4fv(Unif_color1, 1, dgreen);
	glUniform4fv(Unif_color2, 1, lgreen);
	glUniform2fv(Unif_strip_width, 1, swidth);

	glDrawElements(GL_TRIANGLES, Indices2_count, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(Attrib_vertex);

	//! Отключаем шейдерную программу
	glUseProgram(0);

	checkOpenGLerror();

	glFlush();
	glutSwapBuffers();
}

void specialKeys(int key, int, int) {
	if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		switch (key) {
			case GLUT_KEY_UP: scale_y += 0.1f; break;
			case GLUT_KEY_DOWN: scale_y -= 0.1f; break;

			case GLUT_KEY_RIGHT: scale_x += 0.1f; break;
			case GLUT_KEY_LEFT: scale_x -= 0.1f;
		}
	else
		switch (key) {
			case GLUT_KEY_UP: angle_x -= 0.05f; break;
			case GLUT_KEY_DOWN: angle_x += 0.05f; break;

			case GLUT_KEY_RIGHT: angle_y += 0.05f; break;
			case GLUT_KEY_LEFT: angle_y -= 0.05f; break;

			case GLUT_KEY_PAGE_UP: angle_z += 0.05f; break;
			case GLUT_KEY_PAGE_DOWN: angle_z -= 0.05f;
		}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	std::locale::global(std::locale(""));
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

	Init();

	glutDisplayFunc(Render);
	glutReshapeFunc(resizeWindow);
	glutSpecialFunc(specialKeys);
	
	glutMainLoop();

	//! Освобождение ресурсов
	freeShader();
	freeVBO();
}
