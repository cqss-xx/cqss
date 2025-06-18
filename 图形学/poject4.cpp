#include<stdlib.h>
#include <GL/glut.h>
#include<math.h>
#include<windows.h>
const double TWO_PI = 6.2831853;
GLsizei winWidth = 400, winHeight = 400;
GLuint regHex;

class screenPt
{
private:
    GLint x, y;

public:
    screenPt() {
        x = y = 0;
    }
    void setCoords(GLint xCoordValue, GLint yCoordValue) {
        x = xCoordValue;
        y = yCoordValue;
    }
    GLint getx() const {
        return x;
    }
    GLint gety() const {
        return y;
    }
};

static void init(void)
{
    screenPt hexVertex, circCtr;
    GLdouble theta;
    GLint k;
    circCtr.setCoords(winWidth / 2, winHeight / 2);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    regHex = glGenLists(1);
    glNewList(regHex, GL_COMPILE);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for (k = 0;k < 6;k++) {
        theta = TWO_PI * k / 6.0;
        hexVertex.setCoords(circCtr.getx() + 150 * cos(theta), circCtr.gety() + 150 * sin(theta));
        glVertex2i(hexVertex.getx(), hexVertex.gety());  
    }
    glEnd();
    glEndList();
}
void regHexagon(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glCallList(regHex);
    glFlush();
}

void winReshapeFcn(int newWidth, int newHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)newWidth, 0.0, (GLdouble)newHeight);
    glClear(GL_COLOR_BUFFER_BIT);
}

void setPixel(GLint x, GLint y, int color) {
    switch (color) {
    case 1: glColor3f(1.0, 0.0, 0.0); break; // 红色
    case 2: glColor3f(0.0, 1.0, 0.0); break; // 绿色
    case 3: glColor3f(0.0, 0.0, 1.0); break; // 蓝色
    default: glColor3f(0.0, 0.0, 0.0); break; // 黑色
    }
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}
void getPixel(int x, int y, int& color) {
    color = 0;
}



void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(400, 400);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Reshape-Function & Display-List Example");
    init();
    glutDisplayFunc(regHexagon);
    glutReshapeFunc(winReshapeFcn);
    glutMainLoop();
}

/*#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#define MAX_EDGES 1000
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

typedef struct {
    int ymax;
    float x;
    float dx;
} Edge;

typedef struct {
    int count;
    Edge edges[MAX_EDGES];
} EdgeList;

// 多边形顶点坐标（按顺时针顺序排列）
int polyVertices[][2] = {
    {100, 100}, {150, 200}, {300, 100},
    {400, 350}, {300, 400}, {50, 200}
};
const int numVertices = 6;

EdgeList ET[WINDOW_HEIGHT]; // 边表
EdgeList AET;               // 活性边表

void initEdgeList(EdgeList* list) {
    list->count = 0;
}

void addEdge(EdgeList* list, Edge edge) {
    if (list->count < MAX_EDGES) {
        list->edges[list->count++] = edge;
    }
}

int compareEdges(const void* a, const void* b) {
    Edge* e1 = (Edge*)a;
    Edge* e2 = (Edge*)b;
    return (e1->x > e2->x) ? 1 : -1;
}

// 初始化边表
void buildET() {
    for (int i = 0; i < WINDOW_HEIGHT; i++) initEdgeList(&ET[i]);

    for (int i = 0; i < numVertices; i++) {
        int x0 = polyVertices[i][0];
        int y0 = polyVertices[i][1];
        int x1 = polyVertices[(i + 1) % numVertices][0];
        int y1 = polyVertices[(i + 1) % numVertices][1];

        if (y0 == y1) continue;

        // 确保y0是边的较小y值
        if (y0 > y1) {
            int temp = y0; y0 = y1; y1 = temp;
            temp = x0; x0 = x1; x1 = temp;
        }

        float dx = (x1 - x0) / (float)(y1 - y0);
        Edge edge = { y1, (float)x0, dx };
        addEdge(&ET[y0], edge); // 边存储在起始y位置
    }
}

// 扫描线方向（从下到上）
void scanFill() {
    buildET();
    initEdgeList(&AET);

    for (int y = 0; y < WINDOW_HEIGHT; y++) { // 改为从下到上扫描
        // 合并当前y对应的边到AET
        for (int i = 0; i < ET[y].count; i++) {
            addEdge(&AET, ET[y].edges[i]);
        }

        // 移除已完成的边（y超过ymax）
        int k = 0;
        while (k < AET.count) {
            if (AET.edges[k].ymax <= y) {
                for (int j = k; j < AET.count - 1; j++)
                    AET.edges[j] = AET.edges[j + 1];
                AET.count--;
            }
            else {
                k++;
            }
        }

        // 按x坐标排序
        qsort(AET.edges, AET.count, sizeof(Edge), compareEdges);

        // 绘制填充区域（使用点模式确保完全填充）
        glBegin(GL_POINTS);
        for (int i = 0; i < AET.count; i += 2) {
            if (i + 1 >= AET.count) break;
            int xStart = (int)AET.edges[i].x;
            int xEnd = (int)AET.edges[i + 1].x;
            for (int x = xStart; x <= xEnd; x++) {
                glVertex2i(x, y);
            }
        }
        glEnd();

        // 更新x值
        for (int i = 0; i < AET.count; i++) {
            AET.edges[i].x += AET.edges[i].dx;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0); // 改为绿色填充

    // 绘制多边形边界
    glColor3f(1.0, 1.0, 1.0); // 白色边界
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numVertices; i++) {
        glVertex2iv(polyVertices[i]);
    }
    glEnd();

    // 填充多边形
    scanFill();

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("扫描填充算法填充多边形");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}*/

