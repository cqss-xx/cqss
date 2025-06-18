#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>



void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);
	glBegin(GL_LINES);
	int x0, y0, xEnd, yEnd;
	printf("please input x0,y0");
	scanf_s("%d,%d", &x0, &y0);
	printf("please input xEnd,yEnd");
	scanf_s("%d,%d", &xEnd, &yEnd);
	glVertex2i(x0, y0);
	glVertex2i(xEnd, yEnd);
	glEnd();
	glFlush();
}

inline int round(const float a)
{
	return int(a + 0.5);
}

void setPixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd;
}
void lineDDA(int x0, int y0, int xEnd, int yEnd)
{
	int dx = xEnd - x0, dy = yEnd - y0, steps, k;
	float xIncrement, yIncreament, x = x0, y = y0;

	if (fabs(dx) > fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	xIncrement = float(dx) / float(steps);
	yIncreament = float(dy) / float(steps);

	setPixel(round(x), round(y));
	for (k = 0;k < steps;k++) {
		x += xIncrement;
		y += yIncreament;
		setPixel(round(x), round(y));
	}
}

void displayDDA(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);
	glBegin(GL_LINES);
	int x0, y0, xEnd, yEnd;
	printf("please input x0,y0");
	scanf_s("%d,%d", &x0, &y0);
	printf("please input xEnd,yEnd");
	scanf_s("%d,%d", &xEnd, &yEnd);
	lineDDA(x0, y0, xEnd, yEnd);
	glEnd();
	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("An Example OpenGL Program");
	init();
	//glutDisplayFunc(lineSegment);
	glutDisplayFunc(displayDDA);
	glutMainLoop();

}
