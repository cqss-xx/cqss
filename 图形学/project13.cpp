#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 窗口尺寸
#define WIDTH 800
#define HEIGHT 600

// 旋转角度
float angleX = 30.0f;
float angleY = 45.0f;

// 初始化 OpenGL 设置
void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // 设置背景色为黑色
    glEnable(GL_DEPTH_TEST);               // 启用深度测试（Z-buffer）
    glEnable(GL_LIGHTING);                 // 启用光照
    glEnable(GL_LIGHT0);                   // 启用光源0

    // 设置光源属性
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // 设置材质属性
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
}

// 绘制立方体
void drawCube(float size) {
    glBegin(GL_QUADS);

    // 前面
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-size / 2, -size / 2, size / 2);
    glVertex3f(size / 2, -size / 2, size / 2);
    glVertex3f(size / 2, size / 2, size / 2);
    glVertex3f(-size / 2, size / 2, size / 2);

    // 后面
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glVertex3f(-size / 2, size / 2, -size / 2);
    glVertex3f(size / 2, size / 2, -size / 2);
    glVertex3f(size / 2, -size / 2, -size / 2);

    // 顶面
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-size / 2, size / 2, -size / 2);
    glVertex3f(-size / 2, size / 2, size / 2);
    glVertex3f(size / 2, size / 2, size / 2);
    glVertex3f(size / 2, size / 2, -size / 2);

    // 底面
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glVertex3f(size / 2, -size / 2, -size / 2);
    glVertex3f(size / 2, -size / 2, size / 2);
    glVertex3f(-size / 2, -size / 2, size / 2);

    // 右面
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(size / 2, -size / 2, -size / 2);
    glVertex3f(size / 2, size / 2, -size / 2);
    glVertex3f(size / 2, size / 2, size / 2);
    glVertex3f(size / 2, -size / 2, size / 2);

    // 左面
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glVertex3f(-size / 2, -size / 2, size / 2);
    glVertex3f(-size / 2, size / 2, size / 2);
    glVertex3f(-size / 2, size / 2, -size / 2);

    glEnd();
}

// 绘制金字塔
void drawPyramid(float baseSize, float height) {
    glBegin(GL_TRIANGLES);

    // 前面
    glNormal3f(0.0f, -0.5f, 0.866f);
    glVertex3f(0.0f, height / 2, 0.0f);
    glVertex3f(-baseSize / 2, -height / 2, baseSize / 2);
    glVertex3f(baseSize / 2, -height / 2, baseSize / 2);

    // 右面
    glNormal3f(0.866f, -0.5f, 0.0f);
    glVertex3f(0.0f, height / 2, 0.0f);
    glVertex3f(baseSize / 2, -height / 2, baseSize / 2);
    glVertex3f(baseSize / 2, -height / 2, -baseSize / 2);

    // 后面
    glNormal3f(0.0f, -0.5f, -0.866f);
    glVertex3f(0.0f, height / 2, 0.0f);
    glVertex3f(baseSize / 2, -height / 2, -baseSize / 2);
    glVertex3f(-baseSize / 2, -height / 2, -baseSize / 2);

    // 左面
    glNormal3f(-0.866f, -0.5f, 0.0f);
    glVertex3f(0.0f, height / 2, 0.0f);
    glVertex3f(-baseSize / 2, -height / 2, -baseSize / 2);
    glVertex3f(-baseSize / 2, -height / 2, baseSize / 2);

    glEnd();

    // 底面
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-baseSize / 2, -height / 2, -baseSize / 2);
    glVertex3f(-baseSize / 2, -height / 2, baseSize / 2);
    glVertex3f(baseSize / 2, -height / 2, baseSize / 2);
    glVertex3f(baseSize / 2, -height / 2, -baseSize / 2);
    glEnd();
}

// 绘制场景
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲区和深度缓冲区

    // 设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 8.0,  // 相机位置
        0.0, 0.0, 0.0,  // 观察点
        0.0, 1.0, 0.0); // 上方向

    // 应用旋转
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // 设置不同材质颜色
    GLfloat red_diffuse[] = { 1.0f, 0.2f, 0.2f, 1.0f };
    GLfloat green_diffuse[] = { 0.2f, 1.0f, 0.2f, 1.0f };
    GLfloat blue_diffuse[] = { 0.2f, 0.2f, 1.0f, 1.0f };
    GLfloat yellow_diffuse[] = { 1.0f, 1.0f, 0.2f, 1.0f };
    GLfloat purple_diffuse[] = { 1.0f, 0.2f, 1.0f, 1.0f };

    // 绘制第一个立方体（红色，位于前方）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, -2.0f);
    drawCube(1.0f);
    glPopMatrix();

    // 绘制第二个立方体（绿色，位于后方）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse);
    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 0.0f);
    drawCube(1.2f);
    glPopMatrix();

    // 绘制第三个立方体（蓝色，位于左上方）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
    glPushMatrix();
    glTranslatef(-2.0f, 1.5f, 1.0f);
    drawCube(0.8f);
    glPopMatrix();

    // 绘制第一个金字塔（黄色，位于右下方）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
    glPushMatrix();
    glTranslatef(1.0f, -1.5f, -1.5f);
    drawPyramid(1.0f, 1.5f);
    glPopMatrix();

    // 绘制第二个金字塔（紫色，位于中间）
    glMaterialfv(GL_FRONT, GL_DIFFUSE, purple_diffuse);
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 1.5f);
    drawPyramid(0.8f, 1.2f);
    glPopMatrix();

    glutSwapBuffers();  // 交换前后缓冲区
}

// 窗口大小改变时的回调函数
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    // 设置视口
    glViewport(0, 0, width, height);

    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

// 键盘事件处理函数
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:    // ESC 键退出程序
        exit(0);
        break;
    case 'x':   // 绕 X 轴旋转
        angleX += 5.0f;
        glutPostRedisplay();
        break;
    case 'X':   // 绕 X 轴反向旋转
        angleX -= 5.0f;
        glutPostRedisplay();
        break;
    case 'y':   // 绕 Y 轴旋转
        angleY += 5.0f;
        glutPostRedisplay();
        break;
    case 'Y':   // 绕 Y 轴反向旋转
        angleY -= 5.0f;
        glutPostRedisplay();
        break;
    }
}

// 特殊键处理函数（方向键）
void special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:    // 上方向键：增大 X 旋转角度
        angleX += 5.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:  // 下方向键：减小 X 旋转角度
        angleX -= 5.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:  // 左方向键：减小 Y 旋转角度
        angleY -= 5.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT: // 右方向键：增大 Y 旋转角度
        angleY += 5.0f;
        glutPostRedisplay();
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL Z-buffer 3D 演示");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutMainLoop();
    return 0;
}