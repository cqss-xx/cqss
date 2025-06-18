#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { Left, Right, Bottom, Top } Boundary;
const GLint nClip = 4;

typedef struct { GLfloat x, y; } wcPt2D;

// 判断点是否在边界内侧
GLint inside(wcPt2D p, Boundary b, wcPt2D wMin, wcPt2D wMax) {
    switch (b) {
    case Left:   return (p.x >= wMin.x);
    case Right:  return (p.x <= wMax.x);
    case Bottom: return (p.y >= wMin.y);
    case Top:    return (p.y <= wMax.y);
    }
    return 0;
}

// 判断线段是否跨越边界
GLint cross(wcPt2D p1, wcPt2D p2, Boundary winEdge, wcPt2D wMin, wcPt2D wMax) {
    return (inside(p1, winEdge, wMin, wMax) != inside(p2, winEdge, wMin, wMax));
}

// 计算线段与边界的交点
wcPt2D intersect(wcPt2D p1, wcPt2D p2, Boundary winEdge, wcPt2D wMin, wcPt2D wMax) {
    wcPt2D iPt;
    GLfloat m = (p2.y - p1.y) / (p2.x - p1.x + 1e-6); // 避免除零

    switch (winEdge) {
    case Left:
        iPt.x = wMin.x;
        iPt.y = p1.y + (wMin.x - p1.x) * m;
        break;
    case Right:
        iPt.x = wMax.x;
        iPt.y = p1.y + (wMax.x - p1.x) * m;
        break;
    case Bottom:
        iPt.y = wMin.y;
        iPt.x = p1.x + (wMin.y - p1.y) / m;
        break;
    case Top:
        iPt.y = wMax.y;
        iPt.x = p1.x + (wMax.y - p1.y) / m;
        break;
    }
    return iPt;
}

// 裁剪点处理函数
void clipPoint(wcPt2D p, Boundary winEdge, wcPt2D wMin, wcPt2D wMax,
    wcPt2D* pOut, int* cnt, wcPt2D first[], wcPt2D s[]) {
    wcPt2D iPt;

    if (first[winEdge].x == -1 && first[winEdge].y == -1) {
        first[winEdge] = p;
    }
    else if (cross(p, s[winEdge], winEdge, wMin, wMax)) {
        iPt = intersect(p, s[winEdge], winEdge, wMin, wMax);
        if (winEdge < Top) {
            clipPoint(iPt, static_cast<Boundary>(winEdge + 1), wMin, wMax, pOut, cnt, first, s);
        }
        else {
            pOut[*cnt] = iPt;
            (*cnt)++;
        }
    }

    s[winEdge] = p;

    if (inside(p, winEdge, wMin, wMax)) {
        if (winEdge < Top) {
            clipPoint(p, static_cast<Boundary>(winEdge + 1), wMin, wMax, pOut, cnt, first, s);
        }
        else {
            pOut[*cnt] = p;
            (*cnt)++;
        }
    }
}

// 闭合多边形处理
void closeClip(wcPt2D wMin, wcPt2D wMax, wcPt2D* pOut, GLint* cnt,
    wcPt2D first[], wcPt2D s[]) {
    wcPt2D pt;
    
    for (int winEdgeInt = Left; winEdgeInt <= Top; winEdgeInt++) {
        Boundary winEdge = static_cast<Boundary>(winEdgeInt);
        if (cross(s[winEdge], first[winEdge], winEdge, wMin, wMax)) {
            pt = intersect(s[winEdge], first[winEdge], winEdge, wMin, wMax);
            
            if (winEdgeInt < Top) { // Top对应的整数值是3，小于3时才递增
                Boundary nextEdge = static_cast<Boundary>(winEdgeInt + 1);
                clipPoint(pt, nextEdge, wMin, wMax, pOut, cnt, first, s);
            } else {
                pOut[*cnt] = pt;
                (*cnt)++;
            }
        }
    }
}


// 主裁剪函数
GLint polygonClipSuthHodg(wcPt2D wMin, wcPt2D wMax, GLint n,
    wcPt2D* pIn, wcPt2D* pOut) {
    wcPt2D first[nClip], s[nClip];
    GLint cnt = 0;

    // 初始化first数组
    for (int i = 0; i < nClip; i++) {
        first[i].x = first[i].y = -1;
        s[i].x = s[i].y = -1;
    }

    for (int k = 0; k < n; k++) {
        clipPoint(pIn[k], Left, wMin, wMax, pOut, &cnt, first, s);
    }
    closeClip(wMin, wMax, pOut, &cnt, first, s);
    return cnt;
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 定义裁剪窗口
    wcPt2D wMin = { 100, 100 };
    wcPt2D wMax = { 400, 400 };

    // 原始多边形数据
    wcPt2D input[] = {
        {50, 50}, {50, 300}, {450, 500}, {300, 150}
    };
    wcPt2D output[20];
    int n = polygonClipSuthHodg(wMin, wMax, 4, input, output);

    // 绘制裁剪窗口
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(wMin.x, wMin.y);
    glVertex2f(wMax.x, wMin.y);
    glVertex2f(wMax.x, wMax.y);
    glVertex2f(wMin.x, wMax.y);
    glEnd();

    // 绘制原始多边形（红色）
    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 4; i++)
        glVertex2f(input[i].x, input[i].y);
    glEnd();

    // 绘制裁剪结果（蓝色）
    if (n > 0) {
        glColor3f(0, 0, 1);
        glBegin(GL_POLYGON);
        for (int i = 0; i < n; i++)
            glVertex2f(output[i].x, output[i].y);
        glEnd();
    }

    glFlush();
}

// OpenGL初始化
void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Sutherland-Hodgman ");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
