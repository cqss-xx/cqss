#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 窗口大小
#define WIDTH 800
#define HEIGHT 600

// 视图枚举
typedef enum {
    FRONT_VIEW,
    TOP_VIEW,
    SIDE_VIEW,
    ISO_VIEW      // 新增：等轴测视图（原三维图形）
} ViewType;

// 当前显示模式
typedef enum {
    MULTI_VIEW,  // 多视图模式
    FULLSCREEN   // 全屏模式
} DisplayMode;

DisplayMode displayMode = MULTI_VIEW;
ViewType currentFullscreenView = FRONT_VIEW;

// 自定义图形顶点数据（示例：立方体）
GLfloat vertices[8][3] = {
    {-0.5, -0.5, -0.5},
    {0.5, -0.5, -0.5},
    {0.5, 0.5, -0.5},
    {-0.5, 0.5, -0.5},
    {-0.5, -0.5, 0.5},
    {0.5, -0.5, 0.5},
    {0.5, 0.5, 0.5},
    {-0.5, 0.5, 0.5}
};

// 立方体面的顶点索引
GLint faces[6][4] = {
    {0, 1, 2, 3}, // 前面 -Z
    {4, 5, 6, 7}, // 后面 +Z
    {0, 3, 7, 4}, // 左面 -X
    {1, 2, 6, 5}, // 右面 +X
    {0, 1, 5, 4}, // 底面 -Y
    {2, 3, 7, 6}  // 顶面 +Y
};

// 初始化OpenGL设置
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);  // 设置深度测试函数
    glEnable(GL_CULL_FACE); // 启用面剔除
    glCullFace(GL_BACK);    // 剔除背面
}

// 设置视图
void setView(ViewType view) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 使用正交投影，确保所有面都能看到
    glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (view) {
    case FRONT_VIEW:
        gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    case TOP_VIEW:
        gluLookAt(0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
        break;
    case SIDE_VIEW:
        gluLookAt(5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    case ISO_VIEW:
        // 等轴测视图：沿三个轴等比例观察
        gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    }
}

// 绘制坐标轴
void drawAxes(void) {
    glLineWidth(2.0);

    // X轴（红色）
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(-2.0, 0.0, 0.0);
    glVertex3f(2.0, 0.0, 0.0);
    glEnd();

    // Y轴（绿色）
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, -2.0, 0.0);
    glVertex3f(0.0, 2.0, 0.0);
    glEnd();

    // Z轴（蓝色）
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, -2.0);
    glVertex3f(0.0, 0.0, 2.0);
    glEnd();

    glLineWidth(1.0);
}

// 绘制自定义图形（立方体）
void drawCustomObject(void) {
    for (int i = 0; i < 6; i++) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 4; j++) {
            // 为不同面设置不同颜色
            switch (i) {
            case 0: glColor3f(1.0, 0.0, 0.0); break; // 前面 - 红色
            case 1: glColor3f(0.0, 1.0, 0.0); break; // 后面 - 绿色
            case 2: glColor3f(0.0, 0.0, 1.0); break; // 左面 - 蓝色
            case 3: glColor3f(1.0, 1.0, 0.0); break; // 右面 - 黄色
            case 4: glColor3f(1.0, 0.0, 1.0); break; // 底面 - 紫色
            case 5: glColor3f(0.0, 1.0, 1.0); break; // 顶面 - 青色
            }
            glVertex3fv(vertices[faces[i][j]]);
        }
        glEnd();
    }
}

// 显示多视图模式
void displayMultiView(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 视口设置 - 分为四个区域
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    // 前视图
    glViewport(0, h / 2, w / 2, h / 2);
    setView(FRONT_VIEW);
    drawAxes();
    drawCustomObject();

    // 顶视图
    glViewport(w / 2, h / 2, w / 2, h / 2);
    setView(TOP_VIEW);
    drawAxes();
    drawCustomObject();

    // 侧视图
    glViewport(0, 0, w / 2, h / 2);
    setView(SIDE_VIEW);
    drawAxes();
    drawCustomObject();

    // 原三维图形（等轴测视图）
    glViewport(w / 2, 0, w / 2, h / 2);
    setView(ISO_VIEW);
    drawAxes();
    drawCustomObject();
}

// 显示全屏视图
void displayFullscreen(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    glViewport(0, 0, w, h);
    setView(currentFullscreenView);
    drawAxes();
    drawCustomObject();
}
void display(void) {
    if (displayMode == MULTI_VIEW) {
        displayMultiView();
    }
    else {
        displayFullscreen();
    }

    glutSwapBuffers();
}

// 窗口大小改变回调函数
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glutPostRedisplay();
}

// 键盘回调函数
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC键退出
        exit(0);
        break;
    case 'f': case 'F':
        currentFullscreenView = FRONT_VIEW;
        displayMode = FULLSCREEN;
        glutPostRedisplay();
        break;
    case 't': case 'T':
        currentFullscreenView = TOP_VIEW;
        displayMode = FULLSCREEN;
        glutPostRedisplay();
        break;
    case 's': case 'S':
        currentFullscreenView = SIDE_VIEW;
        displayMode = FULLSCREEN;
        glutPostRedisplay();
        break;
    case 'i': case 'I':
        currentFullscreenView = ISO_VIEW;
        displayMode = FULLSCREEN;
        glutPostRedisplay();
        break;
    case 'm': case 'M':
        displayMode = MULTI_VIEW;
        glutPostRedisplay();
        break;
    }
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL 3D Views");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // 显示使用帮助
    printf("键盘控制:\n");
    printf("F: 全屏显示前视图\n");
    printf("T: 全屏显示顶视图\n");
    printf("S: 全屏显示侧视图\n");
    printf("I: 全屏显示原三维图形\n");
    printf("M: 返回多视图模式\n");
    printf("ESC: 退出程序\n");

    glutMainLoop();
    return 0;
}