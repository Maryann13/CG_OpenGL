#include <GL/glew/glew.h>
#include <GL/freeglut/freeglut.h>
#include <cstdlib>
#include <ctime>
#include <list>

using std::list;

double rotate_x = 0;
double rotate_y = 0;
double rotate_z = 0;

int w = 600, h = 600;

struct sphere {
	float color[3];
	float x, y;

	sphere(float r, float g, float b, float x, float y)
			: x(x), y(y) {
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
};

list<sphere> spheres;

void Init() {
	srand(time(nullptr));
	glClearColor(0.43f, 0.0f, 0.0f, 1.0f);
}

void renderWireTeapot() {
	glLoadIdentity();
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);

	glColor3f(1.0, 1.0, 1.0);
	glutWireTeapot(0.4f);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 0.55);
	glVertex2f(-0.75f, -0.75f);
	glVertex2f(-0.9f, -0.25f);
	glVertex2f(-0.4f, -0.25f);
	glVertex2f(-0.25f, -0.75f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.53, 1.0, 1.0);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(-0.75f, 0.75f);
	glVertex2f(0.75f, 0.75f);

	glColor3f(1.0, 0.0, 0.0); glVertex2f(0.25f, -0.25f);
	glColor3f(0.0, 1.0, 0.0); glVertex2f(0.75f, -0.75f);
	glColor3f(0.0, 0.0, 1.0); glVertex2f(0.25f, -0.75f);
	glEnd();

	renderWireTeapot();

	for each (auto s in spheres) {
		glLoadIdentity();
		glTranslatef(s.x / w * 2 - 1.0f, 1.0f - s.y / h * 2, 0.0f);
		glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
		glColor3fv(s.color);
		glutWireSphere(0.2f, 10, 10);
	}

	glFlush();
	glutSwapBuffers();
}

void specialKeys(int key, int, int) {
	switch (key) {
		case GLUT_KEY_UP: rotate_x += 5; break;
		case GLUT_KEY_DOWN: rotate_x -= 5; break;

		case GLUT_KEY_RIGHT: rotate_y += 5; break;
		case GLUT_KEY_LEFT: rotate_y -= 5; break;

		case GLUT_KEY_PAGE_UP: rotate_z += 5; break;
		case GLUT_KEY_PAGE_DOWN: rotate_z -= 5;
	}
	glutPostRedisplay();
}

float randf() {
	return static_cast<float>(rand()) / RAND_MAX;
}

void Mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		spheres.push_back(sphere(randf(), randf(), randf(), x, y));
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(w, h);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("OpenGL");

	glutDisplayFunc(Display);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(Mouse);
	Init();

	glutMainLoop();
}
