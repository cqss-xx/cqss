﻿#include <GL/glut.h>

GLsizei winWidth = 500, winHeight = 500; 

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0); 
}

void displayWirePolyhedra(void)
{
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3f(0.0, 0.0, 1.0);      

   
    gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   
    glScalef(1.5, 2.0, 1.0);
    glutWireCube(1.0);

    
    glScalef(0.8, 0.5, 0.8);
    glTranslatef(-6.0, -5.0, 0.0);
    glutWireDodecahedron();

   
    glTranslatef(8.6, 8.6, 2.0);
    glutWireTetrahedron();

    
    glTranslatef(-3.0, -1.0, 0.0);
    glutWireOctahedron();

    
    glScalef(0.8, 0.8, 1.0);
    glTranslatef(4.3, -2.0, 0.5);
    glutWireIcosahedron();

    glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
    glViewport(0, 0, newWidth, newHeight);

    glMatrixMode(GL_PROJECTION);
    glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 20.0);

    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT);
}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Three - Dimensional Wire - Frame Objects");

    init();
    glutDisplayFunc(displayWirePolyhedra);
    glutReshapeFunc(winReshapeFcn);
    glutMainLoop();
}