
/* Tam Nguyen - CS330
 
   This program is to build a 3D object which can be move back/forward by using keyboard arrow
   The window can be control by mouse
   There is a light rotating around the main object and there is also an arrow points to the light direction
   - Type "q" to escape
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       /* for cos(), sin(), and sqrt() */
#include <GLUT/GLUT.h>    /* OpenGL Utility Toolkit header */

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif
#define CYCLE_LENGTH   3.3f
#define HANDLE_LIMIT   70.0f
#define INC_SPEED      0.1f

/* Variable controlling various rendering modes. */
static int stencilReflection = 1, stencilShadow = 1, offsetShadow = 1;
static int renderShadow = 1, renderDinosaur = 1, renderReflection = 1;
static int linearFiltering = 0, useMipmaps = 0, useTexture = 1;
static int animation = 1;
//static GLboolean lightSwitch = GL_TRUE;
static int directionalLight = 1;
static int forceExtension = 0;

GLfloat pedalAngle, speed, steering;
GLfloat xpos,zpos,direction;
GLfloat camx,camy,camz;
/* Time varying or user-controled variables. */
static float lightAngle = 0.0, lightHeight = 20;
GLfloat angle = -150;   /* in degrees */
GLfloat angle2 = 30;   /* in degrees */

int moving, startx, starty;
int lightMoving = 0, lightStartX, lightStartY;
enum {
    MISSING, EXTENSION, ONE_DOT_ONE
};
int polygonOffsetVersion;

static GLfloat lightPosition[4];
static GLfloat lightColor[] = {0.8, 1.0, 0.8, 1.0}; /* green-tinted */



/* *INDENT-ON* */
enum {
    X, Y, Z, W
};
enum {
    A, B, C, D
};

/* Create a matrix that will project the desired shadow. */
void shadowMatrix(GLfloat shadowMat[4][4],
             GLfloat groundplane[4],
             GLfloat lightpos[4])
{
    GLfloat dot;

    /* Find dot product between light position vector and ground plane normal. */
    dot = groundplane[X] * lightpos[X] +
    groundplane[Y] * lightpos[Y] +
    groundplane[Z] * lightpos[Z] +
    groundplane[W] * lightpos[W];

    shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
    shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
    shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
    shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

    shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
    shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
    shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
    shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

    shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
    shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
    shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
    shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

    shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
    shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
    shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
    shadowMat[3][3] = dot - lightpos[W] * groundplane[W];

}

GLfloat angleSum(GLfloat a, GLfloat b)
{
  a += b;
  if (a < 0) return a+2*M_PI;
  else if (a > 2*M_PI) return a-2*M_PI;
  else return a;
}
//

/************************
*   Returns the value of
*   the given angle in
*   degrees
************************/
GLfloat degrees(GLfloat a)
{
   return a*180.0f/M_PI;
}

/************************
*   Returns the value of
*   the given angle in
*   radians
************************/
GLfloat radians(GLfloat a)
{
   return a*M_PI/180.0f;
}


/* Find the plane equation given 3 points. */
void
findPlane(GLfloat plane[4],
          GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
    GLfloat vec0[3], vec1[3];

    /* Need 2 vectors to find cross product. */
    vec0[X] = v1[X] - v0[X];
    vec0[Y] = v1[Y] - v0[Y];
    vec0[Z] = v1[Z] - v0[Z];

    vec1[X] = v2[X] - v0[X];
    vec1[Y] = v2[Y] - v0[Y];
    vec1[Z] = v2[Z] - v0[Z];

    /* find cross product to get A, B, and C of plane equation */
    plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
    plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
    plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

    plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}



/* Enumerants for refering to display lists. */
typedef enum {
    RESERVED, BODY_SIDE, BODY_EDGE, BODY_WHOLE, ARM_SIDE, ARM_EDGE, ARM_WHOLE,
    LEG_SIDE, LEG_EDGE, LEG_WHOLE, EYE_SIDE, EYE_EDGE, EYE_WHOLE
} displayLists;


#define ROD_RADIUS      0.05f
#define NUM_SPOKES      20
#define SPOKE_ANGLE      18
#define RADIUS_WHEEL   1.0f
#define TUBE_WIDTH      0.08f
#define RIGHT_ROD      1.6f

void ZCylinder(GLfloat radius,GLfloat length)
{
   GLUquadricObj *cylinder;
   cylinder=gluNewQuadric();
   glPushMatrix();
      glTranslatef(0.0f,0.0f,0.0f);
      gluCylinder(cylinder,radius,radius,length,15,5);
   glPopMatrix();
}
void drawTyre(void)
{
   int i;
   //   Draw The Rim
   glColor3f(1.0f,0.5959f,1.0f);
   glutSolidTorus(0.06f,0.92f,4,30);
   //   Draw The Central Cylinder
   //   Length of cylinder  0.12f
   glColor3f(1.0f,1.0f,0.5f);
   glPushMatrix();
      glTranslatef(0.0f,0.0f,-0.06f);
      ZCylinder(0.02f,0.12f);
   glPopMatrix();
   glutSolidTorus(0.02f,0.02f,3,20);

   //   Draw The Spokes
   glColor3f(1.0f,1.0f,1.0f);
   for(i=0;i<NUM_SPOKES;++i)
   {
      glPushMatrix();
         glRotatef(i*SPOKE_ANGLE,0.0f,0.0f,1.0f);
         glBegin(GL_LINES);
            glVertex3f(0.0f,0.02f,0.0f);
            glVertex3f(0.0f,0.86f,0.0f);
         glEnd();
      glPopMatrix();
   }

   //   Draw The Tyre

   glColor3f(0.0f,0.0f,0.0f);
   glutSolidTorus(TUBE_WIDTH,RADIUS_WHEEL,10,30);
   glColor3f(1.0f,0.0f,0.0f);
}
//

#define CRANK_RODS      1.12f
GLfloat pedalAngle = 5;


static GLfloat skinColor[] = {0.13689327, 1.0, 0.1, 1.0}, tyre_color[] = {0, 1, 1, 1}, eye_color[] = {1,0,0,1}, hat_color[] = {1.0,1.0,0,1};
GLfloat xr, yr, zr;

static void
drawShape(void)
{
    glPushMatrix();
//    glTranslatef(0+xr, 5+yr, 0+zr);
    glTranslatef(0+xr, 0+yr, 0+zr);
// Draw solid sphere
    glScalef(1.5,1.5,1.5);
    glPushMatrix();
        glTranslatef(0, 5, 0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, skinColor);
        glutSolidSphere(2.5,50,50);
        glTranslatef(0, 2, 0);
        glutSolidSphere(2,50,50);
    

    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(-2, 8 , 0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, eye_color);
        glutSolidSphere(0.5,50,50);
    glPopMatrix();


    // draw tyres
    glPushMatrix();
        glScalef(3,3,3);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, tyre_color);
        glTranslatef(cos(CRANK_RODS)-0.5,1.1f,1.0f);
        glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
        drawTyre();
    glPopMatrix();

    glPushMatrix();
        glScalef(3,3,3);
        glTranslatef(cos(CRANK_RODS)-0.5,1.1f,-1.0f);
        glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
        drawTyre();
    glPopMatrix();

    // Hat
    glPushMatrix();
        glScalef(3,3,3);
        glColor4f(0.1, 0.1, 0.7, 1.0);
        glTranslatef(0,3.0f,0);
        glRotatef(-270, -2.0, 0.0, -0.50);
        glRotatef(lightAngle * -270.0 , 0, 0, 1);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, hat_color);
        glutWireCone(1.3,0.5,5,7);
    glPopMatrix();
    
    glPopMatrix();


}

static GLfloat floorVertices[4][3] = {
    { -20.0, 0.0, 20.0 },
    { 20.0, 0.0, 20.0 },
    { 20.0, 0.0, -20.0 },
    { -20.0, 0.0, -20.0 },
};


/* Draw a floor (possibly textured). */
static void
drawFloor(void)
{
    glDisable(GL_LIGHTING);

    if (useTexture) {
        glEnable(GL_TEXTURE_2D);
    }


    glColor3f(1.0, 0.666, 1.0);
    glLineWidth(0.03);
    glBegin(GL_LINES);
    for (GLfloat i = -20.0; i <= 20.0; i += 5.0) {
        glVertex3f(i, 0, 20.0); glVertex3f(i, 0, -20.0);
        glVertex3f(20.0, 0, i); glVertex3f(-20.0, 0, i);

    }

    glEnd();

    glEnable(GL_LIGHTING);
}

static GLfloat floorPlane[4];
static GLfloat floorShadow[4][4];

static void
redraw(void)
{

    /* Clear; default stencil clears to zero. */
    if ((stencilReflection && renderReflection) || (stencilShadow && renderShadow)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    } else {
        /* Avoid clearing stencil when not using it. */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /* Reposition the light source. */
    lightPosition[0] = 12*cos(lightAngle);
    lightPosition[1] = lightHeight;
    lightPosition[2] = 12*sin(lightAngle);

    if (directionalLight) {
        lightPosition[3] = 0.0;
    } else {
        lightPosition[3] = 1.0;
    }

    shadowMatrix(floorShadow, floorPlane, lightPosition);

    glPushMatrix();
    /* Perform scene rotations based on user mouse input. */
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);

    /* Tell GL new light source position. */
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    if (renderReflection) {
        if (stencilReflection) {

            /* Don't update color or depth. */
            glDisable(GL_DEPTH_TEST);
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            /* Draw 1 into the stencil buffer. */
            glEnable(GL_STENCIL_TEST);
            glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
            glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

            /* Now render floor; floor pixels just get their stencil set to 1. */
            drawFloor();

            /* Re-enable update of color and depth. */
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glEnable(GL_DEPTH_TEST);

            /* Now, only render where stencil is set to 1. */
            glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }

        glPushMatrix();

        /* The critical reflection step: Reflect object through the floor
         (the Y=0 plane) to make a relection. */
        glScalef(1.0, -1.0, 1.0);

        /* Reflect the light position. */
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        /* To avoid our normals getting reversed and hence botched lighting
         on the reflection, turn on normalize.  */
        glEnable(GL_NORMALIZE);
        glCullFace(GL_FRONT);

        /* Draw the reflected object. */
        drawShape();

        /* Disable noramlize again and re-enable back face culling. */
        glDisable(GL_NORMALIZE);
        glCullFace(GL_BACK);

        glPopMatrix();

        /* Switch back to the unreflected light position. */
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        if (stencilReflection) {
            glDisable(GL_STENCIL_TEST);
        }
    }

    /* Back face culling will get used to only draw either the top or the
     bottom floor.  This let's us get a floor with two distinct
     appearances.  The top floor surface is reflective and kind of red.
     The bottom floor surface is not reflective and blue. */

    /* Draw "bottom" of floor in blue. */
    glFrontFace(GL_CW);  /* Switch face orientation. */
    glColor4f(0.1, 0.1, 0.7, 1.0);
    drawFloor();
    glFrontFace(GL_CCW);

    if (renderShadow) {
        if (stencilShadow) {
            /* Draw the floor with stencil value 3.  This helps us only
             draw the shadow once per floor pixel (and only on the
             floor pixels). */
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        }
    }


    if (renderDinosaur) {
        /* Draw "actual" dinosaur, not its reflection. */
        drawShape();

    }

    if (renderShadow) {

        /* Render the projected shadow. */

        if (stencilShadow) {

            /* Now, only render where stencil is set above 2 (ie, 3 where
             the top floor is).  Update stencil with 2 where the shadow
             gets drawn so we don't redraw (and accidently reblend) the
             shadow). */
            glStencilFunc(GL_LESS, 2, 0xffffffff);  /* draw if ==1 */
            glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        }

        /* To eliminate depth buffer artifacts, we use polygon offset
         to raise the depth of the projected shadow slightly so
         that it does not depth buffer alias with the floor. */
        if (offsetShadow) {
            switch (polygonOffsetVersion) {
                case EXTENSION:
#ifdef GL_EXT_polygon_offset
                    glEnable(GL_POLYGON_OFFSET_EXT);
                    break;
#endif
#ifdef GL_VERSION_1_1
                case ONE_DOT_ONE:
                    glEnable(GL_POLYGON_OFFSET_FILL);
                    break;
#endif
                case MISSING:
                    /* Oh well. */
                    break;
            }
        }

        /* Render 50% black shadow color on top of whatever the
         floor appareance is. */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_LIGHTING);  /* Force the 50% black. */
        glColor4f(0.0, 0.0, 0.0, 0.5);

        glPushMatrix();
        
        /* Project the shadow. */
        glMultMatrixf((GLfloat *) floorShadow);
        drawShape();
        glPopMatrix();

        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);

        if (offsetShadow) {
            switch (polygonOffsetVersion) {
#ifdef GL_EXT_polygon_offset
                case EXTENSION:
                    glDisable(GL_POLYGON_OFFSET_EXT);
                    break;
#endif
#ifdef GL_VERSION_1_1
                case ONE_DOT_ONE:
                    glDisable(GL_POLYGON_OFFSET_FILL);
                    break;
#endif
                case MISSING:
                    break;
            }
        }
        if (stencilShadow) {
            glDisable(GL_STENCIL_TEST);
        }
    }

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.0);

    if (directionalLight) {
        /* Draw an arrowhead. */
        glDisable(GL_CULL_FACE);
        glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
        glRotatef(lightAngle * -180.0 / M_PI, 0, 1, 0);
        glRotatef(atan(lightHeight/12) * 180.0 / M_PI, 0, 0, 1);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        glVertex3f(2, 1, 1);
        glVertex3f(2, -1, 1);
        glVertex3f(2, -1, -1);
        glVertex3f(2, 1, -1);
        glVertex3f(2, 1, 1);
        glEnd();

        /* Draw a white line from light direction. */
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(5, 0, 0);
        glEnd();
        glEnable(GL_CULL_FACE);
    } else {
        /* Draw a yellow ball at the light source. */
        glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
        glutSolidSphere(1.0, 5, 5);
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}


// Changing window view by mouse
static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
    if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            lightMoving = 1;
            lightStartX = x;
            lightStartY = y;
        }
        if (state == GLUT_UP) {
            lightMoving = 0;
        }
    }
}

// Changing light direction
static void
motion(int x, int y)
{
    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
    if (lightMoving) {
        lightAngle += (x - lightStartX)/40.0;
        lightHeight += (lightStartY - y)/20.0;
        lightStartX = x;
        lightStartY = y;
        glutPostRedisplay();
    }
}

// called by idle()
void updateScene()
{
    GLfloat xDelta, zDelta;
    GLfloat rotation;
    GLfloat sin_steering, cos_steering;

    // if the tricycle is not moving then do nothing
    if (-INC_SPEED < speed && speed < INC_SPEED) return;

    if(speed < 0.0f)
        pedalAngle = speed = 0.0f;

    // otherwise, calculate the new position of the tricycle
    // and the amount that each wheel has rotated.
    // The tricycle has moved "speed*(time elapsed)".
    // We assume that "(time elapsed)=1".

    xDelta = speed*cos(radians(direction + steering));
    zDelta = speed*sin(radians(direction + steering));
    xpos += xDelta;
    zpos -= zDelta;
    pedalAngle = degrees(angleSum(radians(pedalAngle), speed/RADIUS_WHEEL));

    // we'll be using sin(steering) and cos(steering) more than once
    // so calculate the values one time for efficiency
    sin_steering = sin(radians(steering));
    cos_steering = cos(radians(steering));

    rotation = atan2(speed * sin_steering, CYCLE_LENGTH + speed * cos_steering);
    direction = degrees(angleSum(radians(direction),rotation));
}

// Moving object by keyboard arrow
void special(int key,int x,int y)
{
   switch(key)
   {
      case GLUT_KEY_UP:
         speed += INC_SPEED;
         zr += INC_SPEED;
         break;
      case GLUT_KEY_DOWN:
         speed += INC_SPEED;
         zr -= INC_SPEED;
         break;
      case GLUT_KEY_LEFT:
         speed += INC_SPEED;
         xr += INC_SPEED;
         break;
      case GLUT_KEY_RIGHT:
         speed += INC_SPEED;
         xr -= INC_SPEED;
         break;
   }
   glutPostRedisplay();
}

/* Advance time varying state when idle callback registered. */
static void
idle(void)
{
    static float time = 0.0;

    time = glutGet(GLUT_ELAPSED_TIME) / 500.0;

    updateScene();
    
    if (!lightMoving) {
        //Updating light angle 
        lightAngle += 0.03;
    }
    glutPostRedisplay();
}


/* When not visible, stop animating.  Restart when visible again. */
static void
visible(int vis)
{
    if (vis == GLUT_VISIBLE) {
        if (animation)
        glutIdleFunc(idle);
    } else {
        if (!animation)
        glutIdleFunc(NULL);
    }
}

void reset()
{   //Reset to put the object back to the origin
   xr = yr = zr  = 0.0f;
   pedalAngle=steering=0.0f;
   pedalAngle=speed=steering=0.0f;
   camx=camy=0.0f;
   camz=5.0f;
   xpos=zpos=0.0f;
   direction=0.0f;
}

/* Press any key to redraw; good when motion stopped and
 performance reporting on. */
/* ARGSUSED */
static void
key(unsigned char c, int x, int y)
{
    switch(c)
    {
        case 's':
        case 'S':
//      Reset everything
            reset();
            break;
        case 'u':
            // Rotate clockwise
            speed += INC_SPEED;
            break;
        case 'd':
            // Roate counter clockwise - slowdown
            speed -= INC_SPEED;
            break;

        case 'q':
            exit(1);

    }

    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    int i;

    glutInit(&argc, argv);

    for (i=1; i<argc; i++) {
        if (!strcmp("-linear", argv[i])) {
            linearFiltering = 1;
        } else if (!strcmp("-mipmap", argv[i])) {
            useMipmaps = 1;
        } else if (!strcmp("-ext", argv[i])) {
            forceExtension = 1;
        }
    }

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);

    glutCreateWindow("Lab 3 - A Whirlygig Object");


    /* Register GLUT callbacks. */
    glutDisplayFunc(redraw);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutVisibilityFunc(visible);
    glutKeyboardFunc(key);
    glutSpecialFunc(special);


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glLineWidth(3.0);

    glMatrixMode(GL_PROJECTION);
    gluPerspective( /* field of view in degree */ 40.0,
                   /* aspect ratio */ 1.0,
                   /* Z near */ 20.0, /* Z far */ 100.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 8.0, 60.0,  /* eye is at (0,8,60) */
              0.0, 8.0, 0.0,      /* center is at (0,8,0) */
              0.0, 1.0, 0.);      /* up is in postivie Y direction */

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);


    /* Setup floor plane for projected shadow calculations. */
    findPlane(floorPlane, floorVertices[1], floorVertices[2], floorVertices[3]);

    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}
