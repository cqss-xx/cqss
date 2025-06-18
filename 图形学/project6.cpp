#include <GL/glut.h>
#include <math.h>
#define _USE_MATH_DEFINES
#include <cmath>

class wcPt2D {
public:
    GLfloat x, y;
};

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);      // 白色背景
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);    // 设置坐标系范围
}

void drawAxes() {
    glColor3f(0.0, 0.0, 0.0); // 黑色坐标轴
    glBegin(GL_LINES);
    // X轴
    glVertex2f(0.0, 0.0);
    glVertex2f(400.0, 0.0);
    // Y轴
    glVertex2f(0.0, 0.0);
    glVertex2f(0.0, 400.0);
    glEnd();

    // 绘制刻度
    glBegin(GL_LINES);
    for (int i = 100; i <= 400; i += 20) {
        // X轴刻度
        //glVertex2f(i, 0);
        //glVertex2f(i, 5);
        // Y轴刻度
        //glVertex2f(0, i);
        //glVertex2f(5, i);
    }
    glEnd();
}

void translatePolygon(wcPt2D* verts, GLint nVerts, GLfloat tx, GLfloat ty) {
    GLint k;
    for (k = 0; k < nVerts; k++) {
        verts[k].x += tx;
        verts[k].y += ty;
    }
    glBegin(GL_POLYGON);
    for (k = 0; k < nVerts; k++)
        glVertex2f(verts[k].x, verts[k].y);
    glEnd();
}

void rotatePolygon(wcPt2D* verts, GLint nVerts, wcPt2D pivPt, GLdouble theta) {
    wcPt2D vertsRot[4]; 
    GLint k;
    for (k = 0; k < nVerts; k++) {
        vertsRot[k].x = pivPt.x + (verts[k].x - pivPt.x) * cos(theta) - (verts[k].y - pivPt.y) * sin(theta);
        vertsRot[k].y = pivPt.y + (verts[k].x - pivPt.x) * sin(theta) + (verts[k].y - pivPt.y) * cos(theta);
    }
    glBegin(GL_POLYGON);
    for (k = 0; k < nVerts; k++)
        glVertex2f(vertsRot[k].x, vertsRot[k].y);
    glEnd();
}

void scalePolygon(wcPt2D* verts, GLint nVerts, wcPt2D fixedPt, GLfloat sx, GLfloat sy) {
    wcPt2D vertsNew[4]; 
    GLint k;
    for (k = 0; k < nVerts; k++) {
        vertsNew[k].x = verts[k].x * sx + fixedPt.x * (1 - sx)+100;
        vertsNew[k].y = verts[k].y * sy + fixedPt.y * (1 - sy)+100;
    }
    glBegin(GL_POLYGON);
    for (k = 0; k < nVerts; k++)
        glVertex2f(vertsNew[k].x, vertsNew[k].y);
    glEnd();
}


// 对称变换（支持x轴、y轴、原点对称）
void mirrorPolygon(wcPt2D* verts, GLint nVerts, GLenum mirrorType, wcPt2D centerPt = { 0, 0 }) {
    wcPt2D tempVerts[4]; // 临时顶点数组
    GLfloat cx = centerPt.x, cy = centerPt.y;

    for (GLint k = 0; k < nVerts; k++) {
        GLfloat x = verts[k].x, y = verts[k].y;

        // 平移顶点到以centerPt为原点的坐标系
        x -= cx;
        y -= cy;

        // 对称变换
        switch (mirrorType) {
        case 1:     y = -y; break;   // x轴对称
        case 2:     x = -x; break;   // y轴对称
        case 3:     x = -x; y = -y; break; // 原点对称
        default:            return;          // 无效对称类型
        }

        // 平移回原坐标系
        tempVerts[k].x = x + cx;
        tempVerts[k].y = y + cy;
    }

    // 保存结果到传入的顶点数组并绘制
    for (GLint k = 0; k < nVerts; k++) {
        verts[k] = tempVerts[k];
    }
    glBegin(GL_POLYGON);
    for (GLint k = 0; k < nVerts; k++) {
        glVertex2f(verts[k].x, verts[k].y);
    }
    glEnd();
}

// 错切变换（支持x方向和y方向错切）
void shearPolygon(wcPt2D* verts, GLint nVerts, GLfloat sh_x, GLfloat sh_y) {
    wcPt2D tempVerts[4];
    for (GLint k = 0; k < nVerts; k++) {
        
        tempVerts[k].x = verts[k].x + sh_y * verts[k].y+100;
        tempVerts[k].y = verts[k].y + sh_x * verts[k].x+100;
    }
    for (GLint k = 0; k < nVerts; k++) {
        verts[k] = tempVerts[k];
    }
    glBegin(GL_POLYGON);
    for (GLint k = 0; k < nVerts; k++) {
        glVertex2f(verts[k].x, verts[k].y);
    }
    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    wcPt2D originalVerts[] = { {100, 200}, {150, 200}, {150, 250}, {100, 250} };
    GLint nVerts = 4;

    // 1. 原始多边形（红色）
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for (int k = 0; k < nVerts; k++) {
        glVertex2f(originalVerts[k].x, originalVerts[k].y);
    }
    glEnd();


    // 2. 平移（蓝色）
    wcPt2D translatedVerts[4];
    for (int i = 0; i < nVerts; i++) {
        translatedVerts[i] = originalVerts[i];
    }
    glColor3f(0.0, 0.0, 1.0);
    translatePolygon(translatedVerts, nVerts, 50.0, 50.0);

    // 3. 旋转45°（绿色）
    wcPt2D rotatedVerts[4];
    for (int i = 0; i < nVerts; i++) {
        rotatedVerts[i] = originalVerts[i];
    }
    wcPt2D pivot = { 125.0, 125.0 }; // 旋转中心点
    glColor3f(0.0, 1.0, 0.0);
    rotatePolygon(rotatedVerts, nVerts, pivot, M_PI / 4); // 旋转45度

    // 4. 缩放1.5倍（紫色)）
    wcPt2D scaledVerts[4];
    for (int i = 0; i < nVerts; i++) {
        scaledVerts[i] = originalVerts[i];
    }
    wcPt2D fixedPoint = { 125.0, 125.0 }; // 缩放固定点
    glColor3f(0.5, 0.0, 0.5);
    scalePolygon(scaledVerts, nVerts, fixedPoint, 1.5, 1.5); // 放大1.5倍


    // 5. 关于x轴对称（黄色)）
    wcPt2D mirrorXVerts[4] = { 0 };
    for (int i = 0; i < nVerts; i++) {
        mirrorXVerts[i] = originalVerts[i];
    }
    wcPt2D mirrorCenterX = { 250, 125 };
    glColor3f(1.0, 1.0, 0.0); // 黄色
    mirrorPolygon(mirrorXVerts, nVerts, 1, mirrorCenterX);

    // 6. 错切变换（x方向错切系数0.5，y方向0.3，青色）
    wcPt2D shearVerts[4] = { 0 };
    for (int i = 0; i < nVerts; i++) {
        shearVerts[i] = originalVerts[i];
    }
    glColor3f(0.0, 1.0, 1.0); // 青色
    shearPolygon(shearVerts, nVerts, 0.3, 0.5); // sh_x=0.3（y方向错切）, sh_y=0.5（x方向错切）
    
    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
    glutInitWindowSize(800, 600); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D变换示例");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}