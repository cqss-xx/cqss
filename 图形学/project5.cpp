
#include <GL/glut.h>
#include <stdio.h>

// Cohen - Sutherland裁剪算法的区域码
#define INSIDE 0  // 0000
#define LEFT 1    // 0001
#define RIGHT 2   // 0010
#define BOTTOM 4  // 0100
#define TOP 8     // 1000

// 裁剪窗口
float x_min = -50, x_max = 50, y_min = -50, y_max = 50;

// 计算点的区域码
int compute_code(float x, float y) {
    int code = INSIDE;
    if (x < x_min)
        code |= LEFT;
    else if (x > x_max)
        code |= RIGHT;
    if (y < y_min)
        code |= BOTTOM;
    else if (y > y_max)
        code |= TOP;
    return code;
}

// Cohen - Sutherland算法
int cohen_sutherland(float* x1, float* y1, float* x2, float* y2) {
    int code1 = compute_code(*x1, *y1);
    int code2 = compute_code(*x2, *y2);
    int accept = 0;

    while (1) {
        if (!(code1 | code2)) {
            // 线段完全在裁剪窗口内
            accept = 1;
            break;
        }
        else if (code1 & code2) {
            // 线段完全在裁剪窗口外
            break;
        }
        else {
            // 部分在裁剪窗口内
            int outside_code = code1 ? code1 : code2;
            float x, y;
            if (outside_code & TOP) {
                x = *x1 + (*x2 - *x1) * (y_max - *y1) / (*y2 - *y1);
                y = y_max;
            }
            else if (outside_code & BOTTOM) {
                x = *x1 + (*x2 - *x1) * (y_min - *y1) / (*y2 - *y1);
                y = y_min;
            }
            else if (outside_code & RIGHT) {
                y = *y1 + (*y2 - *y1) * (x_max - *x1) / (*x2 - *x1);
                x = x_max;
            }
            else if (outside_code & LEFT) {
                y = *y1 + (*y2 - *y1) * (x_min - *x1) / (*x2 - *x1);
                x = x_min;
            }

            if (outside_code == code1) {
                *x1 = x;
                *y1 = y;
                code1 = compute_code(*x1, *y1);
            }
            else {
                *x2 = x;
                *y2 = y;
                code2 = compute_code(*x2, *y2);
            }
        }
    }
    return accept;
}

// 显示函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);  // 红色绘制原始线段
    float x1, y1, x2, y2;
    printf("请输入直线段的起点 (x1 y1) 和终点 (x2 y2): ");
    scanf_s("%f %f %f %f", &x1, &y1, &x2, &y2);

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    if (cohen_sutherland(&x1, &y1, &x2, &y2)) {
        glColor3f(0.0, 1.0, 0.0);  // 绿色绘制裁剪后的线段
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }

    glColor3f(0.0, 0.0, 1.0);  // 蓝色绘制裁剪窗口
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_min, y_min);
    glVertex2f(x_max, y_min);
    glVertex2f(x_max, y_max);
    glVertex2f(x_min, y_max);
    glEnd();

    glFlush();
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(-100, 100.0, -100.0, 100.0);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Cohen - Sutherland裁剪算法");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}


/*GLsizei winWidth = 600, winHeight = 500;
GLint xRaster = 25, yRaster = 150;
GLubyte label[36] = { 'J','a','n', 'F','e','b',  'M','a','r',
                      'A','p','r', 'M','a','y',  'J','u','n',
                      'J','u','l', 'A','u','g',  'S','e','p',
                      'O','c','t', 'N','o','v',  'D','e','c' };
GLint dataValue[12] = { 420,342,324,310,262,185,
                        190,196,217,240,312,438 };

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}

void drawAxes() {
    glColor3f(0.0, 0.0, 0.0);
    // 绘制 x 轴
    glBegin(GL_LINES);
    glVertex2i(50, 50);
    glVertex2i(winWidth - 50, 50);
    // 绘制 y 轴
    glVertex2i(50, 50);
    glVertex2i(50, winHeight - 50);
    glEnd();

    // 绘制 x 轴刻度和标签
    for (int i = 0; i < 12; i++) {
        int x = 50 + i * 50;
        glBegin(GL_LINES);
        glVertex2i(x, 50);
        glVertex2i(x, 45);
        glEnd();
        glRasterPos2i(x - 10, 20);
        for (int k = 3 * i; k < 3 * i + 3; k++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[k]);
    }

    // 绘制 y 轴刻度
    for (int i = 0; i < 10; i++) {
        int y = 50 + i * 40;
        glBegin(GL_LINES);
        glVertex2i(50, y);
        glVertex2i(45, y);
        glEnd();
    }
}

void lineGraph(void)
{
    GLint k;
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制坐标轴
    //drawAxes();

    // 绘制折线
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (k = 0; k < 12; k++) {
        glVertex2i(50 + k * 50, 50 + dataValue[k] / 10);
    }
    glEnd();

    // 绘制数据点
    glColor3f(1.0, 0.0, 0.0);
    for (k = 0; k < 12; k++) {
        glRasterPos2i(50 + k * 50 - 2, 50 + dataValue[k] / 10 - 4);
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
    }

    glFlush();
}

void winReshapeFcn(int newWidth, int newHeight)
{
    winWidth = newWidth;
    winHeight = newHeight;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)newWidth, 0.0, (GLdouble)newHeight);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Line Chart Data Plot");
    init();
    glutDisplayFunc(lineGraph);
    glutReshapeFunc(winReshapeFcn);
    glutMainLoop();
    return 0;
}*/