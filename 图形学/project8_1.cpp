#include <GL/glut.h>
#include <math.h>
#include <iostream>

class wcPt3D {
public:
    GLfloat x, y, z;
};

typedef float Matrix4x4[4][4];

Matrix4x4 matRot;
GLfloat transX = 0.0f, transY = 0.0f, transZ = 0.0f; // 平移量
GLfloat scaleFactor = 1.0f; // 缩放因子
const GLfloat TRANSLATION_STEP = 0.1f; // 每次平移的步长
const GLfloat SCALE_STEP = 0.1f; // 每次缩放的步长

void matrix4x4SetIdentity(Matrix4x4 matIdent4x4) {
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            matIdent4x4[row][col] = (row == col);
}

void matrix4x4PreMultiply(Matrix4x4 m1, Matrix4x4 m2) {
    Matrix4x4 matTemp;
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] +
            m1[row][2] * m2[2][col] + m1[row][3] * m2[3][col];
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            m2[row][col] = matTemp[row][col];
}

void translate3D(GLfloat tx, GLfloat ty, GLfloat tz) {
    Matrix4x4 matTransl3D;
    matrix4x4SetIdentity(matTransl3D);
    matTransl3D[0][3] = tx;
    matTransl3D[1][3] = ty;
    matTransl3D[2][3] = tz;
    matrix4x4PreMultiply(matTransl3D, matRot);
}

void scale3D(GLfloat sx, GLfloat sy, GLfloat sz) {
    Matrix4x4 matScale3D;
    matrix4x4SetIdentity(matScale3D);
    matScale3D[0][0] = sx;
    matScale3D[1][1] = sy;
    matScale3D[2][2] = sz;
    matrix4x4PreMultiply(matScale3D, matRot);
}

// 绕 X 轴旋转
void rotate3D(GLfloat radianAngle) {
    Matrix4x4 matRotX;
    matrix4x4SetIdentity(matRotX);
    GLfloat cosA = cos(radianAngle);
    GLfloat sinA = sin(radianAngle);

    matRotX[1][1] = cosA;
    matRotX[1][2] = -sinA;
    matRotX[2][1] = sinA;
    matRotX[2][2] = cosA;

    matrix4x4PreMultiply(matRotX, matRot);
}

void matrix4x4Transpose(Matrix4x4 src, GLfloat* dest) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            dest[j * 4 + i] = src[i][j];
}

// 绘制彩色立方体
void drawColoredCube(GLfloat xOffset, GLfloat yOffset, GLfloat zOffset) {
    glPushMatrix();
    glTranslatef(xOffset, yOffset, zOffset); // 基础偏移

    glBegin(GL_QUADS);
    // 前面（红）
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    // 后面（绿）
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    // 顶面（蓝）
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    // 底面（黄）
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    // 右面（紫）
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    // 左面（青）
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();

    glPopMatrix();
}

// 键盘回调函数（处理平移和缩放）
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': transY += TRANSLATION_STEP; break; // 沿 Y 轴正方向平移
    case 's': transY -= TRANSLATION_STEP; break; // 沿 Y 轴负方向平移
    case 'a': transX -= TRANSLATION_STEP; break; // 沿 X 轴负方向平移
    case 'd': transX += TRANSLATION_STEP; break; // 沿 X 轴正方向平移
    case 'q': transZ -= TRANSLATION_STEP; break; // 沿 Z 轴负方向平移
    case 'e': transZ += TRANSLATION_STEP; break; // 沿 Z 轴正方向平移
    case 'z': scaleFactor -= SCALE_STEP; break; // 缩小
    case 'x': scaleFactor += SCALE_STEP; break; // 放大
    default: return;
    }

    // 限制最小缩放因子
    if (scaleFactor < 0.1f) scaleFactor = 0.1f;

    glutPostRedisplay(); // 触发重绘
}

void displayFcn(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 绘制原立方体（原点，彩色，未旋转）
    drawColoredCube(0.0f, 0.0f, 0.0f);

    // 绘制旋转+平移+缩放后的立方体（右移 2 单位，可平移和缩放）
    glPushMatrix();
    glTranslatef(2.0f + transX, transY, transZ); // 总平移：基础偏移（2,0,0）+ 用户控制的平移

    GLfloat angle = 0.5f; // 固定旋转角度
    matrix4x4SetIdentity(matRot);
    rotate3D(angle);
    scale3D(scaleFactor, scaleFactor, scaleFactor); // 均匀缩放

    GLfloat glMatrix[16];
    matrix4x4Transpose(matRot, glMatrix);
    glMultMatrixf(glMatrix);

    drawColoredCube(0.0f, 0.0f, 0.0f); // 绘制变换后的立方体

    glPopMatrix();

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D 旋转、平移与缩放演示");

    init();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}