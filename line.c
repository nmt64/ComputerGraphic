////
////  line.c
////  CS300Lab2
////  Bresenham's Line Drawing Algorithm using OpenGL
////  Created by Nguyen Tam on 9/25/19.
////  Copyright © 2019 Nguyen Tam. All rights reserved.
////  This program is to draw concentric circle using bresenham's circle drawing algorithm with center (0, 0) and radii of circles as 100.
//
//
//#include<GLUT/glut.h>
//#include<stdio.h>
//#include<math.h>
//void init(void)
//{
//    glClearColor(1.0,1.0,1.0,0.0);
//    glMatrixMode(GL_PROJECTION);
//    gluOrtho2D(0.0,500.0,0.0,500.0);
//}
//
//void setPixel(GLint x,GLint y)
//{
//    glBegin(GL_POINTS);
//    glVertex2f(x,y);
//    glEnd();
//}
//
//void myDisplay()
//{
//    int x0=0,y0=0,xn=400,yn=150,x,y;
//    int dx,dy;
//    int pk;
//    int k;
//    glClear(GL_COLOR_BUFFER_BIT);
//    glColor3f(0,0,0);
//    setPixel(x0,y0);
//    dx=xn-x0;
//    dy=yn-y0;
//    pk=2*dy-dx;
//    x=x0;
//    y=y0;
//    for(k=0;k<dx-1;++k)
//    {
//        if(pk<0)
//        {
//            pk=pk+2*dy;
//        }
//        else
//        {
//            pk=pk+2*dy-2*dx;
//            ++y;
//        }
//        ++x;
//        setPixel(x,y);
//    }
//    
//    glColor3f(0,0,0);
//    glBegin(GL_LINES);
//        glVertex2f(0,20.0);
//        glVertex2f(200.0,95.0);
//    glEnd();
//    
//    glFlush();
//}
//
//
//void keyboard(unsigned char key, int x, int y)    /* called on key press */
//{
//    if( key == 'q' ) exit(0);
//}
//
//
//void myReshape(int w, int h)
//// Reshape function to handle window event
//{
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_COLOR);
//    glLoadIdentity();
//    
//    if (w <= h)
//        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
//                2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
//    else
//        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
//                2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
//}
//
//int main (int argc,char **argv)
//{
//    glutInit(&argc,argv);
//    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
//    glutInitWindowPosition(50,25);
//    glutInitWindowSize(600,250);
//    glutCreateWindow("Lab2Midline");
//    glutKeyboardFunc(keyboard);
//    glutReshapeFunc(myReshape);
//    init();
//    glutDisplayFunc(myDisplay);
//    glutMainLoop();
//    return 0;
//}
