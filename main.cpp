
#include <stdlib.h>
#include <GL\freeglut.h>
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <windows.h>

float angle = 0;
GLuint      texture[3];

//Colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat BLUE[] = { 0, 0, 1 };
GLfloat GRAY[] = { 0.5, 0.5, 0.5, 1.0 };
//A camera
class Camera {
	double theta; //determinate the x and z positions
	double y; // the curent y position
	double dTheta; // increment the theta for svinging the camera around
	double dy; // increment in y for moving the camera up/down

public:
	Camera():  theta(0), y(8), dTheta(0.04), dy(0.2) {}
	double getX() {
		return 20 * cos(theta);
	}
	double getY() {
		return y;
	}
	double getZ() {
		return 10 * sin(theta);
	}
	void moveRight() { theta += dTheta; }
	void moveLeft() { theta -= dTheta; }
	void moveUp() { y += dy; }
	void moveDown() { if (y > dy) y -= dy; }
};

//A ball
class Ball {
	double radius;
	GLfloat*color;
	double maximumHeight;
	double x;
	double y;
	double z;
	int direction;

public:
	Ball(double r, GLfloat* c, double h, double x, double z):
		radius(r), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {}

	void update() {
		y += direction * 0.1;
		if (y > maximumHeight) {
			y = maximumHeight;
			direction = -1;
		}
		else if (y < radius) {
			y = radius;
			direction = 1;
		}
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y-0.7, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}
};

void DrawFloor()                                // Draws The Floor
{
    glColor3d(0,1,0);
    GLfloat lightPosition[] = { 6, 6, -7, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glBegin(GL_QUADS);                          // Begin Drawing A Quad
        glNormal3f(0.0, 10.0, 0.0);                  // Normal Pointing Up
        glVertex3f(-5.0, 0.0, 5.0);                 // Bottom Left Corner Of Floor
        glVertex3f(-5.0, 0.0,-5.0);                 // Top Left Corner Of Floor
        glVertex3f( 5.0, 0.0,-5.0);                 // Top Right Corner Of Floor
        glVertex3f( 5.0, 0.0, 5.0);                 // Bottom Right Corner Of Floor
    glEnd();                                // Done Drawing The Quad
}



Camera camera;
Ball balls[] = {

	Ball(0.6, GREEN, 9, 2, -2),
	Ball(0.58, RED, 9, 0, 0),
	Ball(0.6, BLUE, 9, 2, 2)
};

//Application-specific initialization: Set up global lighting parameters and create display lists.
void init() {
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//checkerboard.create();
}

//Draws one frame, the checkerboard then the balls, from the current camera position
void display() {
    glClearStencil(0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	DrawFloor();
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glDisable(GL_DEPTH_TEST);
        glPushMatrix();
        glScalef(1.0f, -1.0f, 1.0f);
        glTranslatef(0,0,0);
        glRotatef(angle,0,1,0);
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {

	// draw reflection of scene in mirror

		balls[i].update();

	}
	glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);




    glEnable(GL_BLEND); //enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //set the blending function

	DrawFloor();
	glDisable(GL_BLEND);
	glRotatef(angle,0,1,0);
	// draw reflection of scene in mirror

	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
		balls[i].update();
	}
	glFlush();
	glutSwapBuffers();
}

//On reshape, constructs a camera that perfectly lists the window
void reshape(GLint M, GLint b) {
	glViewport(0, 0, M, b);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(M) / GLfloat(b), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

//Requests to draw the next frame
void timer(int v)
{
	glutPostRedisplay();
	glutTimerFunc(2400 / 60, timer, v);
}

//Moves the camera acording to the key pressed, then ask to refer onto dispay
void special(int key, int, int){
	switch (key)
	{
	case GLUT_KEY_LEFT: camera.moveLeft(); break;
	case GLUT_KEY_RIGHT: camera.moveRight(); break;
	case GLUT_KEY_UP: camera.moveUp(); break;
	case GLUT_KEY_DOWN: camera.moveDown(); break;
	}
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a': camera.moveLeft(); break;
	case 'd': camera.moveRight(); break;
	case 'w': camera.moveUp(); break;
	case 's': camera.moveDown(); break;
	case 'q': exit(0);
	}
	glutPostRedisplay();
}

//Initializes GLUT and enters the main loop
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Projet Infographie");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);
	glutKeyboardFunc(key);
	init();
	glutMainLoop();
	return 0;

}
