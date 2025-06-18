
/*#include<stdlib.h>
#include <GL/glut.h>

void myinit(void)
{

	

	glClearColor(1.0, 1.0, 1.0, 0.0); 
	glColor3f(1.0, 0.0, 0.0); 

	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}
void  plot_circle_points(int xc, int yc, int x, int y)
{
	glBegin(GL_POINTS);
	glVertex3f(xc + x, yc + y, 0);
	glVertex3f(xc - x, yc + y, 0);
	glVertex3f(xc + x, yc - y, 0);
	glVertex3f(xc - x, yc - y, 0);
	glVertex3f(xc + y, yc + x, 0);
	glVertex3f(xc - y, yc + x, 0);
	glVertex3f(xc + y, yc - x, 0);
	glVertex3f(xc - y, yc - x, 0);
	glEnd();
}
void drawcircle(int xc, int yc, int radius)
{
	int x, y, p;
	x = 0;
	y = radius;
	p = 3 - 2 * radius;
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	while (x < y)
	{
		plot_circle_points(xc, yc, x, y);
		if (p < 0)
			p = p + 4 * x + 6;
		else
		{
			p = p + 4 * (x - y) + 10;
			y -= 1;
		}
		x += 1;
	}
	if (x == y)
		plot_circle_points(xc, yc, x, y);
}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); 

	
	glViewport(0, 0, 500, 500);


	drawcircle(200, 200, 100);

	
	glFlush();
}

int main(int argc, char** argv)
{

	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
	glutInitWindowSize(500, 500); 
	glutInitWindowPosition(0, 0); 
	glutCreateWindow("Bresenham  circile"); 
	glutDisplayFunc(display); 
	myinit(); 
	glutMainLoop(); 
}*/
#include <GL/glut.h>
#include <iostream>

// 定义 screenPt 类
class screenPt
{
private:
    GLint x, y;

public:
    screenPt() {
        x = y = 0;
    }
    void setCoords(GLint xCoordValue, GLint yCoordValue) {
        x = xCoordValue;
        y = yCoordValue;
    }
    GLint getx() const {
        return x;
    }
    GLint gety() const {
        return y;
    }
    void incrementx() {
        x++;
    }
    void decrementy() {
        y--;
    }
};

// 中点画圆算法
void circleMidpoint(GLint xc, GLint yc, GLint radius)
{
    screenPt circPt;
    GLint p = 1 - radius;
    circPt.setCoords(0, radius);
    void circlePlotPoints(GLint, GLint, screenPt);
    circlePlotPoints(xc, yc, circPt);
    while (circPt.getx() < circPt.gety()) {
        circPt.incrementx();
        if (p < 0)
            p += 2 * circPt.getx() + 1;
        else {
            circPt.decrementy();
            p += 2 * (circPt.getx() - circPt.gety()) + 1;
        }
        circlePlotPoints(xc, yc, circPt);
    }
}

// 绘制圆上的点
void circlePlotPoints(GLint xc, GLint yc, screenPt circPt)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + circPt.getx(), yc + circPt.gety());
    glVertex2i(xc - circPt.getx(), yc + circPt.gety());
    glVertex2i(xc + circPt.getx(), yc - circPt.gety());
    glVertex2i(xc - circPt.getx(), yc - circPt.gety());
    glVertex2i(xc + circPt.gety(), yc + circPt.getx());
    glVertex2i(xc - circPt.gety(), yc + circPt.getx());
    glVertex2i(xc + circPt.gety(), yc - circPt.getx());
    glVertex2i(xc - circPt.gety(), yc - circPt.getx());
    glEnd();
}

// 设置像素点的函数
void setPixel(GLint x, GLint y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// 显示回调函数
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    // 圆心坐标和半径
    GLint xc = 0, yc = 0, radius = 100;
    circleMidpoint(xc, yc, radius);
    glFlush();
}

// 初始化 OpenGL 设置
void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-200.0, 200.0, -200.0, 200.0);
}

// 主函数
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle Drawing");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}