//
//  main.c
//  CS300Lab2
//  Bresenham's Circle Drawing Algorithm using OpenGL
//  Created by Nguyen Tam on 9/25/19.
//  Copyright © 2019 Nguyen Tam. All rights reserved.
//  This program is to draw two concentric circles using bresenham's circle drawing algorithm with center (0, 0) and radii of circles as 100.

#include <stdio.h>
#include <math.h>
#include <GLUT/glut.h>

// Center of the cicle = (320, 240)
int xc = 320, yc = 240;  // Since the axis is at the corner left, I need to set
                            //the center of circle at (320,240) so the circle will appear at the center of window

// Plot eight points using circle's symmetrical property
void plot_point(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc+x, yc+y);
    glVertex2i(xc+x, yc-y);
    glVertex2i(xc+y, yc+x);
    glVertex2i(xc+y, yc-x);
    glVertex2i(xc-x, yc-y);
    glVertex2i(xc-y, yc-x);
    glVertex2i(xc-x, yc+y);
    glVertex2i(xc-y, yc+x);
    glEnd();
}

// Function to draw a circle using bresenham's
// circle drawing algorithm
void bresenham_circle(int r)
{
    int x=0,y=r;
    float pk=(5.0/4.0)-r;

    /* Plot the points */
    /* Plot the first point */
    plot_point(x,y);
    /* Find all vertices till x=y */
    while(x < y)
    {
        x = x + 1;
        if(pk < 0)
            pk = pk + 2*x+1;
        else
        {
            y = y - 1;
            pk = pk + 2*(x - y) + 1;
        }
        plot_point(x,y);
    }
    glFlush();
}

// Function to draw two concentric circles
void concentric_circles(void)
{
    /* Clears buffers to preset values */
    glClear(GL_COLOR_BUFFER_BIT);

    int radius1 = 100;
    bresenham_circle(radius1);

}

void init()
{
    /* Set clear color to white */
    glClearColor(1.0,1.0,1.0,0);
    /* Set fill color to black */
    glColor3f(0.0,0.0,0.0);
    gluOrtho2D(0 , 640 , 0 , 480);
}

void keyboard(unsigned char key, int x, int y)    /* called on key press */
{
    if( key == 'q' ) exit(0);
}


void myReshape(int w, int h)
// Reshape function to handle window event
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_COLOR);
    glLoadIdentity();
    
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
                2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
                2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
}

void main(int argc, char **argv)
{
    /* Initialise GLUT library */
    glutInit(&argc,argv);
    /* Set the initial display mode */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    /* Set the initial window position and size */
    glutInitWindowPosition(0,0);
    glutInitWindowSize(640,480);
    /* Create the window with title "DDA_Line" */
    glutCreateWindow("Lab2Midcircle");
    /* Initialize drawing colors */

    init();
    /* Call the displaying function */
    glutDisplayFunc(concentric_circles);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(myReshape);
    /* Keep displaying untill the program is closed */
    glutMainLoop();
}
