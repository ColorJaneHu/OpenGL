//
//  main.cpp
//  Assignment1
//
//  Created by Jane Hu on 17/7/19.
//  Copyright © 2017年 Jane Hu. All rights reserved.
//

//
//  main.cpp
//  kk
//
//  Created by Jane Hu on 17/7/17.
//  Copyright © 2017年 Jane Hu. All rights reserved.
//

//============================================================
// STUDENT NAME:HU XUEJUN
// NUS User ID.: gstcn085
// COMMENTS TO GRADER:
//
//
// ============================================================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CONSTANTS
/////////////////////////////////////////////////////////////////////////////

#define PI                  3.1415926535897932384626433832795

#define MAX_NUM_OF_DISCS    100     // Limit the number of discs.
#define MIN_RADIUS          10.0    // Minimum radius of disc.
#define MAX_RADIUS          40.0    // Maximum radius of disc.
#define NUM_OF_SIDES        18      // Number of polygon sides to approximate a disc.

#define MIN_X_SPEED         1.0     // Minimum speed of disc in X direction.
#define MAX_X_SPEED         20.0    // Maximum speed of disc in X direction.
#define MIN_Y_SPEED         1.0     // Minimum speed of disc in Y direction.
#define MAX_Y_SPEED         20.0    // Maximum speed of disc in Y direction.

#define DESIRED_FPS         30      // Approximate desired number of frames per second.
# define DEG_TO_RAD         0.017453
/////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////

typedef struct discType
{
    double pos[2];          // The X and Y coordinates of the center of the disc.
    double speed[2];        // The velocity of the disc in X and Y directions. Can be negative.
    double radius;          // Radius of the disc.
    unsigned char color[3]; // R, G, B colors of the disc.
} discType;


int numDiscs = 0;                   // Number of discs that have been added.

discType disc[ MAX_NUM_OF_DISCS ];  // Array for storing discs.

bool drawWireframe = false;         // Draw polygons in wireframe if true, otherwise
// otherwise polygons are filled.

int winWidth = 800;                 // Window width in pixels.
int winHeight = 600;                // Window height in pixels.



/////////////////////////////////////////////////////////////////////////////
// Draw the disc in its color using GL_TRIANGLE_FAN.
/////////////////////////////////////////////////////////////////////////////

void DrawDisc( const discType *d )      //把队列disc里的圆一个一个画出来
{
    //===========================
    // WRITE YOUR CODE HERE.
    //===========================
    
    /****
     glbegin()
     d->color……
     
     glend()
     ***/
    int i;
    double n=18;
    glPushMatrix();
    glColor3ub(d->color[0],d->color[1],d->color[2]);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(d->pos[0]/winWidth*2-1, d->pos[1]/winHeight*2-1);
    for(i=0; i<=n; i++)
        glVertex2f((d->pos[0]+d->radius*cos(2*(PI/n)*i))/winWidth*2-1, (d->pos[1]+d->radius*sin(2*PI/n*i))/winHeight*2-1);
    glEnd();
    glPopMatrix();
    
}



/////////////////////////////////////////////////////////////////////////////
// The display callback function.
/////////////////////////////////////////////////////////////////////////////

void MyDisplay( void )
{
    glClear( GL_COLOR_BUFFER_BIT );
    
    if ( drawWireframe )
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    for ( int i = 0; i < numDiscs; i++ ) DrawDisc( &disc[i] );
    
    glFlush();
    glutSwapBuffers();
}



/////////////////////////////////////////////////////////////////////////////
// The mouse callback function.
// If mouse left button is pressed, a new disc is created with its center
// at the mouse cursor position. The new disc is assigned the followings:
// (1) a random radius between MIN_RADIUS and MAX_RADIUS,
// (2) a random speed in X direction in the range
//     from -MAX_X_SPEED to -MIN_X_SPEED, and from MIN_X_SPEED to MAX_X_SPEED.
// (3) a random speed in Y direction in the range
//     from -MAX_Y_SPEED to -MIN_Y_SPEED, and from MIN_Y_SPEED to MAX_Y_SPEED.
// (4) R, G, B color, each ranges from 0 to 255.
/////////////////////////////////////////////////////////////////////////////

void MyMouse( int btn, int state, int x, int y )            //是用来定义圆的初始参数（颜色、x、y、speed）
{
    
    if ( btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        if ( numDiscs >= MAX_NUM_OF_DISCS )
            printf( "Already reached maximum number of discs.\n" );
        else
        {
            //===========================
            // WRITE YOUR CODE HERE.
            //===========================
            /*****
             Firtsly,
             定义disc[numDiscs]
             incude:
             color、r、spd、pos
             rand()
             ***/
            
            disc[numDiscs].color[0] = (char) rand()%256;
            disc[numDiscs].color[1] = (char) rand()%256;
            disc[numDiscs].color[2] = (char) rand()%256;
            int number1 = (char) 1+rand()%41;
            if (number1>21) {
                number1 = number1 - 41;
            }
            int number2 = (char) 1+rand()%41;
            if (number2>21) {
                number2 = number2 - 41;
            }
            disc[numDiscs].speed[0] = number1;
            disc[numDiscs].speed[1] = number2;
            disc[numDiscs].pos[0] = x;
            disc[numDiscs].pos[1] = 600 - y;
            disc[numDiscs].radius = (char) 10+rand()%31;
            
            numDiscs++;
            glutPostRedisplay();
        }
    }
}



/////////////////////////////////////////////////////////////////////////////
// The reshape callback function.
// It also sets up the viewing.
/////////////////////////////////////////////////////////////////////////////

void MyReshape( int w, int h )
{
    winWidth = w;
    winHeight = h;
    
    glViewport( 0, 0, w, h );
    
    glMatrixMode( GL_PROJECTION );
    
    //===========================
    // WRITE YOUR CODE HERE.
    //===========================
    
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}


/////////////////////////////////////////////////////////////////////////////
// The keyboard callback function.
/////////////////////////////////////////////////////////////////////////////

void MyKeyboard( unsigned char key, int x, int y )
{
    switch ( key )
    {
            // Quit program.
        case 'q':
        case 'Q': exit(0);
            break;
            
            // Toggle wireframe or filled polygons.
        case 'w':
        case 'W': drawWireframe = !drawWireframe;
            glutPostRedisplay();
            break;
            
            // Reset and erase all discs.
        case 'r':
        case 'R': numDiscs = 0;
            glutPostRedisplay();
            break;
    }
}



/////////////////////////////////////////////////////////////////////////////
// Updates the positions of all the discs.
//
// Increments the position of each disc by its speed in each of the
// X and Y directions. Note that the speeds can be negative.
// At its new position, if the disc is entirely or partially outside the
// left window boundary, then shift it right so that it is inside the
// window and just touches the left window boundary. Its speed in the X
// direction must now be reversed (negated). Similar approach is
// applied for the cases of the right, top, and bottom window boundaries.
/////////////////////////////////////////////////////////////////////////////

GLfloat theta = 0.0f;
void UpdateAllDiscPos( void )
{
    int time = DESIRED_FPS/1000;
    for ( int i = 0; i < numDiscs; i++ ){
        //===========================
        // WRITE YOUR CODE HERE.
        //===========================
        
        if(disc[i].pos[0]+disc[i].radius>=winWidth&&disc[i].pos[1]+disc[i].radius>=winHeight){
            disc[i].speed[0]=10;
            disc[i].speed[1]=10;
        }
        else if(disc[i].pos[0]+disc[i].radius>=winWidth&&disc[i].pos[1]-disc[i].radius<=0){
            disc[i].speed[0]=10;
            disc[i].speed[1]=-10;
        }
        else if(disc[i].pos[0]-disc[i].radius<=0&&disc[i].pos[1]+disc[i].radius>=winHeight){
            disc[i].speed[0]=-10;
            disc[i].speed[1]=10;
        }
        else if(disc[i].pos[0]-disc[i].radius<=0&&disc[i].pos[1]-disc[i].radius<=0){
            disc[i].speed[0]=-10;
            disc[i].speed[1]=-10;
        }
        
        /*if(disc[i].pos[0]+disc[i].radius>=winWidth || disc[i].pos[0]-disc[i].radius<=0){
         disc[i].speed[0] = -disc[i].speed[0];
         }
         if(disc[i].pos[1]-disc[i].radius<=0 ||disc[i].pos[1]+disc[i].radius>=winHeight){
         disc[i].speed[1]=-disc[i].speed[1];
         }
         */
        if (disc[i].pos[0]+disc[i].radius>=winWidth) {
            disc[i].speed[0] = -abs(disc[i].speed[0]);
            //disc[i].speed[1] = 0 -( 1 + rand()%21);
        }
        if (disc[i].pos[0]-disc[i].radius<=0) {
            disc[i].speed[0] = abs(disc[i].speed[0]);
            //disc[i].speed[1] = 1 + rand()%21;
            
        }
        if (disc[i].pos[1]+disc[i].radius>=winHeight) {
            disc[i].speed[1] = -abs(disc[i].speed[1]);
            //disc[i].speed[0] = 1 + rand()%21;
            
        }
        if (disc[i].pos[1]-disc[i].radius<=0) {
            disc[i].speed[1] = abs(disc[i].speed[1]);
            // disc[i].speed[1] = 1 + rand()%21;
            
        }
        disc[i].pos[0]=disc[i].pos[0]+disc[i].speed[0]*1.5;
        disc[i].pos[1]=disc[i].pos[1]+disc[i].speed[1]*1.5;
        time=time+1.5;
        
    }
    glutPostRedisplay();
}



/////////////////////////////////////////////////////////////////////////////
// The init function.
// It initializes some OpenGL states.
/////////////////////////////////////////////////////////////////////////////

void MyInit( void )
{
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // Black background color.
    glShadeModel( GL_FLAT );
}



/////////////////////////////////////////////////////////////////////////////
// The main function.
/////////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize( winWidth, winHeight );
    glutCreateWindow( "assign1" );
    
    MyInit();
    
    // Register the callback functions.
    glutDisplayFunc( MyDisplay );
    glutReshapeFunc( MyReshape );
    glutMouseFunc( MyMouse );
    glutKeyboardFunc( MyKeyboard );
    glutIdleFunc( UpdateAllDiscPos );
    
    // Display user instructions in console window.
    printf( "Click LEFT MOUSE BUTTON in window to add new disc.\n" );
    printf( "Press 'w' to toggle wireframe.\n" );
    printf( "Press 'r' to erase all discs.\n" );
    printf( "Press 'q' to quit.\n\n" );
    
    // Enter GLUT event loop.
    glutMainLoop();
    return 0;
}


