#include <GL/glut.h>

int projectionMode = 1; // 默认为一点透视
float rotateY = 30.0f;  // 两点透视绕Y轴旋转角度
float rotateX = 20.0f;  // 三点透视绕X轴旋转角度

void drawPyramid() {
    // 金字塔顶点坐标
    GLfloat vertices[][3] = {
        {-1.0f, 0.0f, -1.0f}, // A
        { 1.0f, 0.0f, -1.0f}, // B
        { 1.0f, 0.0f,  1.0f}, // C
        {-1.0f, 0.0f,  1.0f}, // D
        { 0.0f, 1.5f,  0.0f}  // E 顶点
    };

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);

    glBegin(GL_LINES);
    // 绘制底面四边形
    glVertex3fv(vertices[0]); glVertex3fv(vertices[1]);
    glVertex3fv(vertices[1]); glVertex3fv(vertices[2]);
    glVertex3fv(vertices[2]); glVertex3fv(vertices[3]);
    glVertex3fv(vertices[3]); glVertex3fv(vertices[0]);

    // 绘制侧面棱边
    glVertex3fv(vertices[0]); glVertex3fv(vertices[4]);
    glVertex3fv(vertices[1]); glVertex3fv(vertices[4]);
    glVertex3fv(vertices[2]); glVertex3fv(vertices[4]);
    glVertex3fv(vertices[3]); glVertex3fv(vertices[4]);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置摄像机位置
    gluLookAt(0.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 应用模型变换
    switch (projectionMode) {
    case 1: // 一点透视，无旋转
        break;
    case 2: // 两点透视，绕Y轴旋转
        glRotatef(rotateY, 0.0, 1.0, 0.0);
        break;
    case 3: // 三点透视，绕X和Y轴旋转
        glRotatef(rotateX, 1.0, 0.0, 0.0);
        glRotatef(rotateY, 0.0, 1.0, 0.0);
        break;
    }

    drawPyramid();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '1') projectionMode = 1;
    else if (key == '2') projectionMode = 2;
    else if (key == '3') projectionMode = 3;
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Pyramid Perspective Projection");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}