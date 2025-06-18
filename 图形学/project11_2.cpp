#include <GL/glut.h>
#include <stdio.h>

// 全局变量
float rotate_x = 0.0f;
float rotate_y = 0.0f;
float scale = 1.0f;
int mouse_button = -1;
int last_x, last_y;

// 初始化OpenGL设置
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // 设置光源
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 设置材质
    GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// 显示回调函数
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置相机位置
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 应用旋转和缩放
    glScalef(scale, scale, scale);
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);

    // 绘制茶壶
    glutSolidTeapot(1.0);

    glutSwapBuffers();
}

// 窗口大小改变回调函数
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

// 键盘回调函数
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        rotate_x += 5.0f;
        break;
    case 's':
        rotate_x -= 5.0f;
        break;
    case 'a':
        rotate_y += 5.0f;
        break;
    case 'd':
        rotate_y -= 5.0f;
        break;
    case '+':
        scale *= 1.1f;
        break;
    case '-':
        scale /= 1.1f;
        break;
    case 27:  // ESC键退出
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// 鼠标回调函数
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        mouse_button = button;
        last_x = x;
        last_y = y;
    }
    else {
        mouse_button = -1;
    }
}

// 鼠标移动回调函数
void motion(int x, int y) {
    int dx = x - last_x;
    int dy = y - last_y;

    switch (mouse_button) {
    case GLUT_LEFT_BUTTON:
        rotate_y += dx;
        rotate_x += dy;
        break;
    case GLUT_MIDDLE_BUTTON:
        scale *= (1.0f + dy * 0.01f);
        break;
    }

    last_x = x;
    last_y = y;
    glutPostRedisplay();
}

// 帮助信息
void printHelp() {
    printf("\nGLUT茶壶控制程序帮助:\n");
    printf("键盘控制:\n");
    printf("  w/s: 上下旋转\n");
    printf("  a/d: 左右旋转\n");
    printf("  +/: 放大/缩小\n");
    printf("  ESC: 退出程序\n");
    printf("鼠标控制:\n");
    printf("  左键拖动: 旋转茶壶\n");
    printf("  中键拖动: 缩放茶壶\n\n");
}

int main(int argc, char** argv) {
    printHelp();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLUT茶壶控制");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}