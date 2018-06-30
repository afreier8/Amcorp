/*
* triangle.c -- A simple example of OpenGL and GLUT.
*/
#include <GL/glut.h>

void displayCB(void)
/* function called whenever redisplay needed */
{
	glClear(GL_COLOR_BUFFER_BIT);
	/* clear the display */
	glColor3f(1.0, 1.0, 1.0);
	/* set current color to white */
	glBegin(GL_POLYGON);
	/* draw filled triangle */
	glVertex2i(200,125);
	/* specify each vertex of triangle */
	glVertex2i(100,375);
	glVertex2i(300,375);
	glEnd();
	/* OpenGL draws the filled triangle */
	glFlush();
	/* Complete any pending operations */
}

void keyCB(unsigned char key, int x, int y)
{
	if( key == 'q' ) exit(0);
}

/* called on key press */
int main(int argc, char *argv[])
{
	int win;
	glutInit(&argc, argv); /* initialize GLUT system */
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(400,500);
	win = glutCreateWindow("Triangle"); /* width=400pixels height=500pixels */
	/* create window */
	/* from this point on the current window is win */
	glClearColor(0.0,0.0,0.0,0.0);
	gluOrtho2D(0,400,0,500);
	glutDisplayFunc(displayCB);
	glutKeyboardFunc(keyCB); /*

	/*set how background to black */
	//object is mapped to window */
	//window's display callback */
	//window's key callback */
	glutMainLoop(); /* start processing events... */
/* execution never reaches this point */

	return 0;
}
