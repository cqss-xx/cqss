#include <GL/glut.h>
#include <math.h>

// 全局变量用于控制旋转、缩放和平移
float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
float scaleFactor = 1.0f;
float translationX = 0.0f;
float translationY = 0.0f;
float translationZ = 0.0f;

// 初始化 OpenGL 设置
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置背景颜色为黑色
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
}

// 绘制四棱锥
void drawPyramid() {
    glBegin(GL_LINE_STRIP);
    // 底部正方形
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    // 连接顶部顶点
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 应用平移、旋转和缩放变换
    glTranslatef(translationX, translationY, translationZ);
    glRotatef(rotationAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f);
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    glColor3f(1.0f, 1.0f, 1.0f); // 设置线框颜色为白色
    drawPyramid();
    glutSwapBuffers();
}

// 键盘回调函数，处理普通按键
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        rotationAngleX += 5.0f;
        break;
    case 's':
        rotationAngleX -= 5.0f;
        break;
    case 'a':
        rotationAngleY += 5.0f;
        break;
    case 'd':
        rotationAngleY -= 5.0f;
        break;
    case 'f':
        scaleFactor += 0.1f;
        break;
    case 'g':
        if (scaleFactor > 0.1f) {
            scaleFactor -= 0.1f;
        }
        break;
    case 27: // ESC 键退出程序
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// 特殊按键回调函数，处理方向键
void special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        translationY += 0.1f;
        break;
    case GLUT_KEY_DOWN:
        translationY -= 0.1f;
        break;
    case GLUT_KEY_LEFT:
        translationX -= 0.1f;
        break;
    case GLUT_KEY_RIGHT:
        translationX += 0.1f;
        break;
    }
    glutPostRedisplay();
}

// 调整窗口大小回调函数
void reshape(int width, int height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("四棱锥线框图 - 多变换控制");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}