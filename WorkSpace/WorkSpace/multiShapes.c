#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#define pi 3.142857


void drawline(float from_x, float from_y, float to_x, float to_y)
{
    // From coordinate position
    glVertex2f(from_x, from_y);

    // To coordinate position
    glVertex2f(to_x, to_y);
}

void drawShape()
{
    glColor3f(1.0, 1.0, 0.0); // Color (RGB): Yellow
    glLineWidth(2.0); // Set line width to 2.0

    // Draw line
    glBegin(GL_LINES);
    drawline(0.25, 0.5, 0.4, 0.5);
    drawline(0.4, 0.6, 0.4, 0.5);
    drawline(0.4, 0.4, 0.4, 0.5);
    drawline(0.6, 0.5, 0.75, 0.5);
    glEnd();

    // Draw triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(0.4, 0.5);
    glVertex2f(0.6, 0.6);
    glVertex2f(0.6, 0.4);
    glEnd();
}


void drawTriangle()
{
	/* Clears buffers to preset values */
	glColor3f(0.0, 1.0, 0.0); // Color (RGB): Yellow
	glLineWidth(2.0); // Set line width to 2.0

	// Draw triangle
	glBegin(GL_TRIANGLES);
	glVertex2f(0.2, 0.2);
	glVertex2f(0.1, 0.3);
	glVertex2f(0.3, 0.3);
	glEnd();
}

void drawTriangle2()
{
	/* Clears buffers to preset values */
	glColor3f(0.0, 1.0, 1.0); // Color (RGB): Yellow
	glLineWidth(2.0); // Set line width to 2.0

	// Draw triangle
	glBegin(GL_TRIANGLES);
	glVertex2f(0.4, 0.5);
	glVertex2f(0.6, 0.6);
	glVertex2f(0.6, 0.4);
	glEnd();
}


void drawCircle()
{
	// making picture color green (in RGB mode), as middle argument is 1.0
	glColor3f(0.0, 1.0, 0.0);
	 
	// breadth of picture boundary is 1 pixel
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	float x, y, i;
	 
	// iterate y up to 2*pi, i.e., 360 degree
	// with small increment in angle as
	// glVertex2i just draws a point on specified co-ordinate
	for ( i = 0; i < (2 * pi); i += 0.001)
	{
		// let 200 be the radius of circle and as,
		// circle is defined as x=r*cos(i) and y=r*sin(i)
		x = 200 * cos(i);
		y = 200 * sin(i);
		 
		glVertex2i(x, y);
	}
	glEnd();
	glFlush();
}


void render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0.0, 4.0, 0.0, 4.0, -1, 1 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw shape one
    /*glPushMatrix();
    glTranslatef(1.5, 1.5, 0.0);
    drawShape();
    glPopMatrix();

    // Draw shape two
    glPushMatrix();
    glTranslatef(2.5, 2.5, 0.0);
    drawTriangle();
    glPopMatrix();*/
    
    // Draw shape three
	glPushMatrix();
	glTranslatef(1.5, 1.5, 0.0);
	drawCircle();
	glPopMatrix();
	
	// Draw shape three
	glPushMatrix();
	glTranslatef(3.5, 3.5, 0.0);
	drawTriangle();
	glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(880, 780);
    glutCreateWindow("My OpenGL program");
    glutDisplayFunc(render);
    glutMainLoop();
    return 0;
}
