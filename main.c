/*
 * primitive.c -- Show off the different geometric primitives in OpenGL
 */

#include <GLUT/glut.h>


#define FONT            GLUT_BITMAP_8_BY_13
#define CHAR_W            8
#define CHAR_H            13
#define CHAR_DESCENT        3
#define LINE_SEP        4
#define CARRIAGE_RETURN        13
#define BACK_SPACE        8



void color() {
    return glColor3ub(rand()%255,rand()%255 , rand()%255);
}

void displayCB(void)        /* function called whenever redisplay needed */
{
    glClear(GL_COLOR_BUFFER_BIT);        /* clear the display */
    glColor3f(1.0, 1.0, 1.0);        /* set current color to white */
    
    glLoadIdentity();            /* start with the identity matrix */
    gluOrtho2D(0,500,0,420);        /* how object is mapped to window */
    glMatrixMode(GL_MODELVIEW);        /* move the model around */
    
#define VERTEX_LIST \
glVertex2i(20,60);\
glVertex2i(60,30);\
glVertex2i(50,70);\
glVertex2i(50,20);\
glVertex2i(0,0)
    
    glTranslatef(43,320,0);    /* translate model to appropriate point */
    color();
    glPointSize(7.0);
    glBegin(GL_POINTS);
    
    VERTEX_LIST;
    glEnd();
    
    glTranslatef(0,-100,0);    /* translations are cumulative */
    glBegin(GL_LINES);
    glLineWidth(9);
    color();
    VERTEX_LIST;
    glEnd();
    
    glColor3f(0.534,0.53,1.0); //white points
    glTranslatef(166,0,0);
    glBegin(GL_LINE_STRIP);
    color();
    VERTEX_LIST;
    glEnd();
    
    glTranslatef(166,0,0);
    glBegin(GL_LINE_LOOP);
    color();
    VERTEX_LIST;
    glEnd();
    
    glTranslatef(-320,-100,0);
    glBegin(GL_TRIANGLES);
    color();
    VERTEX_LIST;
    glEnd();
    
    glTranslatef(166,0,0);
    glBegin(GL_TRIANGLE_STRIP);
    color();
    VERTEX_LIST;
    glEnd();
    
    glTranslatef(166,0,0);
    glBegin(GL_TRIANGLE_FAN);
    color();
    VERTEX_LIST;
    glEnd();
    
    glTranslatef(-320,-100,0);
    glBegin(GL_QUADS);
    color();

    glVertex2i(40,10);    // x, y
    glVertex2i( 40,40);
    glVertex2i( 10,40);
    color();
    glVertex2i(10, 10);
    glEnd();
    
    glTranslatef(166,0,0);
    glBegin(GL_QUAD_STRIP);
    color();
    glVertex2i(40,20);
    color();
    glVertex2i(50,40);
    glVertex2i(30,20);
    color();
    glVertex2i(5,30);
    color();
    glVertex2i(30,40);
    
    glEnd();
    
    glTranslatef(166,0,0);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_POLYGON);
    color();
    glVertex2i(20,60);
    color();
    glVertex2i(40,70);
    color();
    glVertex2i(50,20);
    color();
    glVertex2i(60,60);
    glEnd();
    
    
    glTranslatef(10,320,0);
    glShadeModel(GL_SMOOTH);
    glBegin(GL_POLYGON);
    color();
    glVertex2i(40, 60);
    color();
    glVertex2i(30, 10);
    glVertex2i(10, 20);
    glVertex2i(20, 10);
    color();
    glVertex2i(10, 30);
    glVertex2i(20, 10);
    glEnd();
    
    
    glFlush();
}

void keyCB(unsigned char key, int x, int y)    /* called on key press */
{
    if( key == 'q' ) exit(0);
}


void init() {
    glClearColor (0.0, 0.0, 0.0, 1.0); //black opaque window
    glColor3f(0.055, 0.07, 1.0); //white color
    glLoadIdentity ();
    
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); } //view volume 2X2X2 at origin


void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
                2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
                2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char *argv[])
{
    int win;
    
    glutInit(&argc, argv);        /* initialize GLUT system */
    
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(500,320);        /* width=500pixels height=420pixels */
    glutInitWindowPosition(100,100);
    win = glutCreateWindow("Lab 1");    /* create window */
    
    /* from this point on the current window is win */
    
    glClearColor(0.0,0.0,0.0,0.0);    /* set background to black */
    glutReshapeFunc(myReshape);
    glutDisplayFunc(displayCB);        /* set window's display callback */
    glutKeyboardFunc(keyCB);        /* set window's key callback */
    init();
    glutMainLoop();            /* start processing events... */
    
    /* execution never reaches this point */
    
    return 0;
}
