/* To draw a static clock using Bresenham's circle drawing algorithm */
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

// Center of the cicle = (320, 240)
//int xc = 320, yc = 240;

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
void OuterCircle(int r)
{
    int x=0,y=r;
    float pk=(5.0/4.0)-r;

    // Plot the points 
    // Plot the first point 
    plot_point(x,y);
    int k;
    // Find all vertices till x=y 
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

void RedrawTargetTriangle(int x, int y, bool redTarget)
{
    MK309_TMA *TMA_item = GetTMAitems();
	 TARGET *target = GetTargetItems();
    OWNSHIP *ownship = GetOwnshipItems();

    g_print(" REDRAW \n");
    // during redraw, must redraw everything to delete previous target
    circle();

    if(redTarget)
    {
        // making picture color red (in RGB mode)
        glColor3f(1.0, 0.0, 0.0);
    }
    else
    {
        // making picture color green (in RGB mode)
        glColor3f(0.0, 1.0, 0.0);
    }

    drawTargetVelocityLine(x, y+8); // change to use variables params -> (x, y+8)
    drawBadSubSurface(x, y);// TEST IN PROGRESS!!
    drawUnknownSurface(20, 60);
    drawKnownGoodSurface(20, 150);
    TorpedoInterceptionPoint(20, 180);

    drawOwnshipVelocityLine();

    // Calculate the bearing and range after everything is set
    CalcRange(x, y);

    // skip recalculating bearing if the target is out of range
    if ( TMA_item->targetWithinRange )
    {
        CalcBearing();
		  DetermineTorpedoLineOfFire();
		  TorpedoInWater(20, yc + 20);
    }
    
	 double dist = CalcDistance(xc, yc, target->Vx, target->Vy);
	 double speed = CalcSpeed( dist );

    // Redraw the display
    glutPostRedisplay();
    // fill color to be white
    glColor3f(1.0, 1.0, 1.0);
}

void drawTargetVelocityLine(int x , int y)
{
    TARGET *target = GetTargetItems();
    MK309_TMA *TMA_item = GetTMAitems(); 

    double convertToDegree = PI / 180;

    g_print("\ntarget->CT = %d\n", target->CT);

    target->Vx = sin(target->CT * convertToDegree);
    target->Vy = cos(target->CT * convertToDegree);

    g_print("\nVx: %f \nVy: %f\n", target->Vx, target->Vy);

    target->endPointX = (target->Vx * x) + x;
    target->endPointY = (target->Vy * y) + y;

    calcTargetVelocityEndPoint(x, y);
    
    glBegin(GL_LINES);
        glVertex2f(x,y);
        glVertex2f(target->endPointX, target->endPointY);
    glEnd;

    glEnd();
    glFlush();
}

void drawOwnshipVelocityLine()
{
    OWNSHIP *ownship = GetOwnshipItems();
    MK309_TMA *TMA_item = GetTMAitems(); 
    
    double convertToDegree = PI / 180;

    ownship->Vx = sin(ownship->CQO * convertToDegree);
    ownship->Vy = cos(ownship->CQO * convertToDegree);

    ownship->endPointX = (ownship->Vx * xc) + xc;
    ownship->endPointY = (ownship->Vy * yc) + yc;

    calcOwnshipVelocityEndPoint();

	 glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINES);
        glVertex2f(xc,yc);
        glVertex2f(ownship->endPointX, ownship->endPointY);
    glEnd;

    glEnd();
    glFlush();
}

void drawOuterCircleLines()
{
    // Center of the cicle = (320, 240)
    // 12 o'clock position
    glBegin(GL_LINES);
        glVertex2f(xc,yc+TMA_CIRCLE_RADIUS+10);
        glVertex2f(xc,yc+TMA_CIRCLE_RADIUS-10);
    glEnd;

    RenderString(xc,yc+120, "APRIL");

    // 6 o'clock position
    glBegin(GL_LINES);
        glVertex2f(xc,yc-TMA_CIRCLE_RADIUS+10);
        glVertex2f(xc,yc-TMA_CIRCLE_RADIUS-10);
    glEnd;

    // 3 o'clock position
    glBegin(GL_LINES);
        glVertex2f(xc+TMA_CIRCLE_RADIUS+10,yc);
        glVertex2f(xc+TMA_CIRCLE_RADIUS-10,yc);
    glEnd;

    // 9 o'clock position
    glBegin(GL_LINES);
        glVertex2f(xc-TMA_CIRCLE_RADIUS+10,yc);
        glVertex2f(xc-TMA_CIRCLE_RADIUS-10,yc);
    glEnd;

    glEnd();
    glFlush();
}

void Init()
{
    // Set clear color to black 
    glClearColor(0.0,0.0,0.0,0);

    // Set fill color to white 
    glColor3f(1.0,1.0,1.0);
    gluOrtho2D(0 , 640 , 0 , 480);   
}

void tma_main()
{
    //pthread_t graphics_thread;
    MK309_TMA *TMA_item = GetTMAitems(); 
    GUI_WIDGETS *gwidget = GetGuiWidgets();

    // Spawn thread to display the graphic continuously
    if( !TMA_item->graphicsThreadStarted )
    {
        g_print("\nCreating a new graphics window\n");
        // Set the initial display mode 
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        // Set the initial window position and size 
        glutInitWindowPosition(0,0);
        glutInitWindowSize(640,480);
        // Create the window with title "DDA_Line" 
        glutCreateWindow("Target Motion Analysis");
        // Initialize drawing colors 
        Init();

        // Call the displaying function 
        glutDisplayFunc(circle);
        // Graphics thread not started
        pthread_create(&TMA_item->graphics_thread, NULL, (void *)StartGraphicsThread, NULL);
    }
    else
    {
        g_print("\nGraphics Thread started!\n");
    }
}

void StartGraphicsThread()
{
    MK309_TMA *TMA_item = GetTMAitems(); 
    TMA_item->graphicsThreadStarted = true;

    glutMouseFunc(NULL);
    glutMotionFunc(NULL);
    glutPassiveMotionFunc(NULL);
    glutMainLoop();   
}

void RenderString(int x, int y, char *string)
{
    void *font = GLUT_BITMAP_TIMES_ROMAN_24;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0,1.0,1.0);
    glRasterPos2f(x,y);
    int len = (int) strlen(string);

    for (int i = 0 ; i < len ; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnd();
    glFlush();
}

