#include <GL/glut.h>
#include<math.h>
#include <stdio.h>
//#include<stdlib.h>
//using namespace std;
int Height=400, Width=400;

void display(void)
{
	/* clear all pixels */
	glClear (GL_COLOR_BUFFER_BIT);

	/* draw white polygon (rectangle) with corners at
	* (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0) 
	*/
	glColor3f (0.2, 0.9, 0.0);
	float cosine=0.0,sine=0.0;
	float center=0.5;
	float x,y,m=0.0,n=0.0;

	for(int i=1;i<5;i++)
	{
		::glPushMatrix();
		//::glScalef( 2.5, 1.5, 0.5 );
		::glTranslatef( i*0.2, i*0.2, 0.0 );
		//::glRotatef( 10.0, 0.0, 0.0, 1.0 );

		glBegin(GL_POLYGON);
		for(int j=0;j<100;j++)
		{
			float angle = ((2*3.1415926535897*j)/100);

			if(i==3) 
			{
				m= -0.2;
				n= -0.4;
				x = m+cos(angle)/32;
				y = n+sin(angle)/32;
			}
			else
			{
				x = (i/10)+cos(angle)/32;
				y = (i/10)+sin(angle)/32;
			}

			glVertex2f(x,y);
		}
		glEnd();
		::glPopMatrix();
	}

	/* don't wait! 
	* start processing buffered OpenGL routines 
	*/
	glFlush ();
}

static void Reshape(int w, int h)
{
	/*
	glClear(GL_COLOR_BUFFER_BIT);

	Width = w; Height = h;

	glViewport (0, 0, Width, Height);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glOrtho(-20.0*(GLfloat)w/(GLfloat)h,20.0*(GLfloat)w/(GLfloat)h, -20.0, 20.0, -20.0, 20.0);*/
	////////////////////////
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*if (w <= h)
	glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
	5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
	else*/
	glOrtho(-20.0*(GLfloat)w/(GLfloat)h,20.0*(GLfloat)w/(GLfloat)h, -20.0, 20.0, -20.0, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	///////////////////////
}
	
void init (void) 
{
	/* select clearing color */
	glClearColor (0.0, 0.0, 0.0, 0.0);

	/* initialize viewing values */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

/* 
* Declare initial window size, position, and display mode

* (single buffer and RGBA). Open window with "hello"
* in its title bar. Call initialization routines.
* Register callback function to display graphics.
* Enter main loop and process events.
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("hello");
	init ();
	// glutReshapeFunc(Reshape);
	glutDisplayFunc(display); 
	glutMainLoop();
	return 0; /* ANSI C requires main to return int. */
}
