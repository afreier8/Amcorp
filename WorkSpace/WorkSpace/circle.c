/* To draw a static clock using Bresenham's circle drawing algorithm */
#include <stdio.h>
#include <GL/glut.h>

// Center of the cicle = (320, 240)
int xc = 320, yc = 240;

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
  int k;
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
}

// Function to draw a static clock
void static_clock(void)
{
  /* Clears buffers to preset values */
  glClear(GL_COLOR_BUFFER_BIT);

// Draw triangle
    /*glBegin(GL_TRIANGLES);
    glVertex2f(0.4, 0.5);
    glVertex2f(0.6, 0.6);
    glVertex2f(0.6, 0.4);
    glEnd;*/

  int outerRadius = 150;
  int innerRadius = 10;
  
  // Draw outer circle
  bresenham_circle(outerRadius);
  glBegin(GL_LINES);
	glEnd();

  // Draw inner circle
  bresenham_circle(innerRadius);
  glBegin(GL_LINES);
	glEnd;
  
  // OwnShip
  glVertex2i(xc, yc);
  glVertex2i(xc, yc+10);
  glVertex2i(xc, yc);
  glVertex2i(xc+10, yc);
  glVertex2i(xc, yc);
  glVertex2i(xc, yc-10);
  glVertex2i(xc, yc);
  glVertex2i(xc-10, yc);
  
// add line to edge of circle
glBegin(GL_LINES);
	glVertex2f(355,365);
	glVertex2f(375,380);
glEnd;

// TEST to hit upper corner
glBegin(GL_LINES);
	glVertex2f(410,330);
	glVertex2f(430,350);
glEnd;

// 12 o'clock position
glBegin(GL_LINES);
	glVertex2f(320,380);
	glVertex2f(320,400);
glEnd;

// 6 o'clock position
glBegin(GL_LINES);
	glVertex2f(320,80);
	glVertex2f(320,100);
glEnd;

// 3 o'clock position
glBegin(GL_LINES);
	glVertex2f(460,240);
	glVertex2f(480,240);
glEnd;



// 9 o'clock position
glBegin(GL_LINES);
	glVertex2f(160,240);
	glVertex2f(180,240);
glEnd;

  glEnd();
  glFlush();
}

void Init()
{
  /* Set clear color to black */
  glClearColor(0.0,0.0,0.0,0);
  
  /* Set fill color to white */
  glColor3f(1.0,1.0,1.0);
  gluOrtho2D(0 , 640 , 0 , 480);
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
  glutCreateWindow("Practice circle by April");
  /* Initialize drawing colors */
  Init();
  /* Call the displaying function */
  glutDisplayFunc(static_clock);
  /* Keep displaying untill the program is closed */
  glutMainLoop();
}
