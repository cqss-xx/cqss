#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// 窗口尺寸
#define WIDTH 800
#define HEIGHT 600

// 控制点定义
GLfloat curveCtrlPts[4][3] = {
    {-3.0f, 0.0f, 0.0f},
    {-1.0f, 4.0f, 0.0f},
    {1.0f, -4.0f, 0.0f},
    {3.0f, 0.0f, 0.0f}
};

GLfloat surfaceCtrlPts[4][4][3] = {
    {{-3.0f, 0.0f, 3.0f}, {-1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}, {3.0f, 0.0f, 3.0f}},
    {{-3.0f, 1.0f, 1.0f}, {-1.0f, 3.0f, 1.0f}, {1.0f, 3.0f, 1.0f}, {3.0f, 1.0f, 1.0f}},
    {{-3.0f, 1.0f, -1.0f}, {-1.0f, 3.0f, -1.0f}, {1.0f, 3.0f, -1.0f}, {3.0f, 1.0f, -1.0f}},
    {{-3.0f, 0.0f, -3.0f}, {-1.0f, 2.0f, -3.0f}, {1.0f, 2.0f, -3.0f}, {3.0f, 0.0f, -3.0f}}
};

// 计算二项式系数
int binomialCoeff(int n, int k) {
    if (k == 0 || k == n) return 1;
    return binomialCoeff(n - 1, k - 1) + binomialCoeff(n - 1, k);
}

// Bernstein基函数
float bernstein(int n, int i, float t) {
    return binomialCoeff(n, i) * powf(t, i) * powf(1 - t, n - i);
}

// 计算Bezier曲线点
void calculateBezierCurve() {
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 1.0f, 0.0f); // 绿色曲线

    for (int i = 0; i <= 100; i++) {
        float t = (float)i / 100.0f;
        GLfloat point[3] = { 0.0f, 0.0f, 0.0f };

        for (int j = 0; j < 4; j++) {
            float b = bernstein(3, j, t);
            point[0] += curveCtrlPts[j][0] * b;
            point[1] += curveCtrlPts[j][1] * b;
            point[2] += curveCtrlPts[j][2] * b;
        }
        glVertex3fv(point);
    }
    glEnd();
}

// 计算Bezier曲面点
void calculateBezierSurface() {
    glColor3f(0.0f, 0.0f, 1.0f); // 蓝色曲面

    // 绘制曲面网格
    for (int u = 0; u <= 20; u++) {
        float uu = (float)u / 20.0f;
        glBegin(GL_LINE_STRIP);
        for (int v = 0; v <= 20; v++) {
            float vv = (float)v / 20.0f;
            GLfloat point[3] = { 0.0f, 0.0f, 0.0f };

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    float bu = bernstein(3, i, uu);
                    float bv = bernstein(3, j, vv);
                    point[0] += surfaceCtrlPts[i][j][0] * bu * bv;
                    point[1] += surfaceCtrlPts[i][j][1] * bu * bv;
                    point[2] += surfaceCtrlPts[i][j][2] * bu * bv;
                }
            }
            glVertex3fv(point);
        }
        glEnd();
    }

    for (int v = 0; v <= 20; v++) {
        float vv = (float)v / 20.0f;
        glBegin(GL_LINE_STRIP);
        for (int u = 0; u <= 20; u++) {
            float uu = (float)u / 20.0f;
            GLfloat point[3] = { 0.0f, 0.0f, 0.0f };

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    float bu = bernstein(3, i, uu);
                    float bv = bernstein(3, j, vv);
                    point[0] += surfaceCtrlPts[i][j][0] * bu * bv;
                    point[1] += surfaceCtrlPts[i][j][1] * bu * bv;
                    point[2] += surfaceCtrlPts[i][j][2] * bu * bv;
                }
            }
            glVertex3fv(point);
        }
        glEnd();
    }
}

// 绘制控制点
void drawControlPoints() {
    glPointSize(5.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // 红色控制点

    // 曲线控制点
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++) {
        glVertex3fv(curveCtrlPts[i]);
    }
    glEnd();

    // 曲面控制点
    glColor3f(1.0f, 1.0f, 0.0f); // 黄色控制点
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            glVertex3fv(surfaceCtrlPts[i][j]);
        }
    }
    glEnd();
}

// 绘制控制多边形
void drawControlPolygons() {
    glColor3f(0.5f, 0.5f, 0.5f); // 灰色控制多边形

    // 曲线控制多边形
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 4; i++) {
        glVertex3fv(curveCtrlPts[i]);
    }
    glEnd();

    // 曲面控制网格
    for (int i = 0; i < 4; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 4; j++) {
            glVertex3fv(surfaceCtrlPts[i][j]);
        }
        glEnd();
    }

    for (int j = 0; j < 4; j++) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 4; i++) {
            glVertex3fv(surfaceCtrlPts[i][j]);
        }
        glEnd();
    }
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置视角
    gluLookAt(0.0f, 5.0f, 15.0f,   // 相机位置
        0.0f, 0.0f, 0.0f,    // 观察点
        0.0f, 1.0f, 0.0f);   // 上方向向量

    // 绘制曲线和曲面
    drawControlPolygons();
    drawControlPoints();
    calculateBezierCurve();
    calculateBezierSurface();

    glutSwapBuffers();
}

// 初始化OpenGL
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 黑色背景
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Bezier Curve and Surface Generation");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}