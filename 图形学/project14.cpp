#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// 定义光源属性
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };  // 方向光
GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

// 定义材质属性
GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat mat_diffuse[] = { 1.0, 0.8, 0.0, 1.0 };  // 金色
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };

// 旋转角度
GLfloat x_rotate = 0.0, y_rotate = 0.0;

// 光源位置参数
float lightX = 1.0, lightY = 1.0, lightZ = 1.0;

// 当前显示的模型
int current_model = 0;
const int MAX_MODELS = 3;

// 初始化OpenGL
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 启用光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 设置光源属性
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // 设置材质属性
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// 显示回调函数
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置观察点
    gluLookAt(0.0, 0.0, 5.0,  // 眼睛位置
        0.0, 0.0, 0.0,  // 观察点
        0.0, 1.0, 0.0); // 向上向量

    // 应用旋转
    glRotatef(x_rotate, 1.0, 0.0, 0.0);
    glRotatef(y_rotate, 0.0, 1.0, 0.0);

    // 更新光源位置
    light_position[0] = lightX;
    light_position[1] = lightY;
    light_position[2] = lightZ;
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 绘制坐标系
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    // X轴 - 红色
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(2.0, 0.0, 0.0);
    // Y轴 - 绿色
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 2.0, 0.0);
    // Z轴 - 蓝色
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 2.0);
    glEnd();
    glEnable(GL_LIGHTING);

    // 绘制光源位置
    glPushMatrix();
    glTranslatef(lightX, lightY, lightZ);
    glColor3f(1.0, 1.0, 0.0);  // 黄色光源
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    // 根据当前模型选择绘制不同的物体
    switch (current_model) {
    case 0:
        // 球体
        glutSolidSphere(1.0, 30, 30);
        break;
    case 1:
        // 立方体
        glutSolidCube(1.5);
        break;
    case 2:
        // 茶壶
        glutSolidTeapot(1.0);
        break;
    }

    glutSwapBuffers();
}

// 窗口大小改变回调函数
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// 键盘回调函数
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        lightY += 0.1;
        break;
    case 's':
        lightY -= 0.1;
        break;
    case 'a':
        lightX -= 0.1;
        break;
    case 'd':
        lightX += 0.1;
        break;
    case 'q':
        lightZ -= 0.1;
        break;
    case 'e':
        lightZ += 0.1;
        break;
    case 'm':
        current_model = (current_model + 1) % MAX_MODELS;
        break;
    case 27:  // ESC键
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// 特殊键回调函数 (用于旋转控制)
void special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        x_rotate += 5.0;
        break;
    case GLUT_KEY_DOWN:
        x_rotate -= 5.0;
        break;
    case GLUT_KEY_LEFT:
        y_rotate += 5.0;
        break;
    case GLUT_KEY_RIGHT:
        y_rotate -= 5.0;
        break;
    }
    glutPostRedisplay();
}

// 鼠标回调函数
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 左键点击增加环境光
        light_ambient[0] = fmin(light_ambient[0] + 0.1, 1.0);
        light_ambient[1] = fmin(light_ambient[1] + 0.1, 1.0);
        light_ambient[2] = fmin(light_ambient[2] + 0.1, 1.0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // 右键点击减少环境光
        light_ambient[0] = fmax(light_ambient[0] - 0.1, 0.0);
        light_ambient[1] = fmax(light_ambient[1] - 0.1, 0.0);
        light_ambient[2] = fmax(light_ambient[2] - 0.1, 0.0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    }
    glutPostRedisplay();
}

// 显示帮助信息
void displayHelp() {
    printf("\nPong光照模型实验控制说明:\n");
    printf("--------------------------------------\n");
    printf("方向键: 旋转模型\n");
    printf("W/S/A/D/Q/E: 移动光源位置\n");
    printf("M: 切换显示模型 (球体/立方体/茶壶)\n");
    printf("鼠标左键: 增加环境光强度\n");
    printf("鼠标右键: 减少环境光强度\n");
    printf("ESC: 退出程序\n");
    printf("--------------------------------------\n\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pong光照模型实验");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);

    displayHelp();

    glutMainLoop();
    return 0;
}
