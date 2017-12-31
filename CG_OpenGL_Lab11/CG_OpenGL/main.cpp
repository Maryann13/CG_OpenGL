#include <GL/glew/glew.h>
#include <GL/freeglut/freeglut.h>
#include <cmath>

float Angle = 17.42f;
float CamAngle = 1.4f;
float CamRadius = 250.0f;

float rotate_center = 0.0f;

float mat1_dif[] = { 0.8f, 0.6f, 0.2f };
float mat1_amb[] = { 0.2f, 0.2f, 0.0f };
float mat1_spec[] = { 0.6f, 0.6f, 0.6f };
float mat1_shininess = 0.05f * 128;

float mat2_dif[] = { 0.7f, 0.7f, 0.7f };
float mat2_amb[] = { 0.2f, 0.2f, 0.0f };
float mat2_spec[] = { 0.6f, 0.6f, 0.6f };
float mat2_shininess = 0.05f * 128;

float mat3_dif[] = { 0.6f, 0.4f, 0.1f };
float mat3_amb[] = { 0.2f, 0.2f, 0.0f };
float mat3_spec[] = { 0.6f, 0.6f, 0.6f };
float mat3_shininess = 0.05f * 128;

float light_dif[] = { 1.0f, 1.0f, 1.0f, 0.5f };
float light_amb[] = { 0.0f, 0.0f, 0.0f, 0.5f };
float light_spec[] = { 1.0f, 1.0f, 1.0f, 0.5f };
float light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };

float light1_dif[] = { 1.0f, 0.0f, 0.0f, 0.5f };
float light1_amb[] = { 0.0f, 0.0f, 0.0f, 0.5f };
float light1_spec[] = { 1.0f, 1.0f, 1.0f, 0.5f };
float light1_pos[] = { -1.0f, 1.0f, 0.0f, 0.0f };

float light2_dif[] = { 0.0f, 1.0f, 0.0f, 1.0f };
float light2_amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float light2_spec[] = { 1.0f, 1.0f, 1.0f, 0.5f };
float light2_pos[] = { -1.0f, 0.0f, -3.0f, 0.0f };

float light2_dir[] = { 3.0f, 0.0f, 3.0f, 0.0f };
float light2_cut = 40.0f;
float light2_exp = 0.0f;

float lmodel_amb[] = { 0.9f, 0.9f, 0.9f, 0.8f };

void Init() {
	glClearColor(0.43f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH_TEST);
}

void Update() {
	Angle += 0.01f;
	rotate_center += 0.02;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(CamRadius * cos(CamAngle), 50.0f, CamRadius * sin(CamAngle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_amb);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);

	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_spec);
	
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_amb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_dif);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_spec);

	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light2_cut);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, light2_exp);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
	
	glPushMatrix();
	glRotatef(Angle, 0.0f, 1.0f, 0.0f);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat1_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat1_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat1_shininess);
	glPushMatrix();
	glRotatef(rotate_center, 0.0f, 1.0f, 0.0f);
	glutSolidCube(70.0f);
	glPopMatrix();	

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat2_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat2_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat2_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat2_shininess);
	glPushMatrix();
	glTranslatef(-65.0f, -5.0f, 0.0f);
	glRotatef(rotate_center, 0.0f, 1.0f, 0.0f);
	glutSolidCube(60.0f);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat3_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat3_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat3_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat3_shininess);
	glPushMatrix();
	glTranslatef(60.0f, -10.0f, 0.0f);
	glRotatef(rotate_center, 0.0f, 1.0f, 0.0f);
	glutSolidCube(50.0f);
	glPopMatrix();

	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat3_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat3_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat3_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat3_shininess);
	glPushMatrix();
	glTranslatef(0.0f, -20.0f, 120.0f);
	glutSolidCube(30.0f);
	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();
}

void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0f, w / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void specialKeys(int key, int, int) {
	switch (key) {
		case GLUT_KEY_UP: CamRadius -= 5; break;
		case GLUT_KEY_DOWN: CamRadius += 5; break;

		case GLUT_KEY_RIGHT: CamAngle += 0.05f; break;
		case GLUT_KEY_LEFT: CamAngle -= 0.05f;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("OpenGL");

	glutIdleFunc(Update);
	glutDisplayFunc(Update);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(specialKeys);
	Init();

	glutMainLoop();
}
