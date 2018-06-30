// Graphics for the symbols i.e. ownship, target, torpedoe in water
void drawOwnship()
{
    //int outerRadius = 150;
    int innerRadius = 10;

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
}

void drawBadSubSurface(int x, int y)
{
   	TARGET *target = GetTargetItems();
    target->centerX = x;
    target->centerY = y;

    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_LINES);
        glVertex2f(x,y);
        glVertex2f(x+10,y+10);
    glEnd;
    
    glBegin(GL_LINES);
        glVertex2f(x,y);
        glVertex2f(x-10,y+10);
    glEnd;

    glEnd();
    glFlush();

    bresenham_circle2(1, x, y+8);
    glBegin(GL_LINES);
    glEnd();
    glFlush();
}

void drawUnknownSurface(int x, int y)
{
    glColor3f(0.0, 1.0, 1.0);

    glBegin(GL_LINES);
        glVertex2f(x,y);
        glVertex2f(x+14,y);
    glEnd;
    
    glBegin(GL_LINES);
        glVertex2f(x+14,y);
        glVertex2f(x+14,y+14);
    glEnd;

    glBegin(GL_LINES);
        glVertex2f(x,y+14);
        glVertex2f(x,y);
    glEnd;

    bresenham_circle2(1, x+7, y+12);

    glEnd();
    glFlush();
}

void TorpedoInWater(int x, int y)
{
    glColor3f(1.0, 0.0, 1.0);

    bresenham_circle2(5, x, y);

    glBegin(GL_LINES);
        glVertex2f(x,y);
        glVertex2f(x,y-9);
    glEnd;
    
    glBegin(GL_LINES);
        glVertex2f(x,y);
        glVertex2f(x+9,y+9);
    glEnd;
    
    glBegin(GL_LINES);
        glVertex2f(x,y);
        glVertex2f(x-9,y+9);
    glEnd;

    glEnd();
    glFlush();
}

void drawKnownGoodSurface(double x, double y)
{
    glColor3f(1.0, 1.0, 0.0);

    double radius = 8;
    double twoPI = 2 * PI;
    double start_angle = 3;
    double end_angle = 6.75;
    double max_angle = 2 * PI;
    double angle_incr = PI / 1000;

    glBegin(GL_POINTS);

    for (double theta = start_angle; theta < end_angle; theta += angle_incr)
    {
        glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }

    bresenham_circle2(1, x, y+1);

    glEnd();
    glFlush();
}

void drawTargetTriangle(int vertex)
{
    // making picture color green (in RGB mode), as middle argument is 1.0
    glColor3f(0.0, 1.0, 0.0);

    // Draw triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(vertex, vertex);
    glVertex2f(vertex-5, vertex-10);
    glVertex2f(vertex+5, vertex-10);
    glEnd();
    glFlush();
}

void circle()
{
    MK309_TMA *TMA_item = GetTMAitems(); 

    // Clears buffers to preset values 
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw outer circle
    bresenham_circle(TMA_CIRCLE_RADIUS);
    glBegin(GL_LINES);
    glEnd();

    drawOwnship();

    drawOuterCircleLines();
}

void plot_point2(int x, int y, int midPointX, int midPointY)
{
    glBegin(GL_POINTS);
    glVertex2i(midPointX+x, midPointY+y);
    glVertex2i(midPointX+x, midPointY-y);
    glVertex2i(midPointX+y, midPointY+x);
    glVertex2i(midPointX+y, midPointY-x);
    glVertex2i(midPointX-x, midPointY-y);
    glVertex2i(midPointX-y, midPointY-x);
    glVertex2i(midPointX-x, midPointY+y);
    glVertex2i(midPointX-y, midPointY+x);
    glEnd();
}

// Function to draw a circle using bresenham's
// circle drawing algorithm
void bresenham_circle2(int r, int midPointX, int midPointY)
{
  int x=0,y=r;
  float pk=(5.0/4.0)-r;

  // Plot the points 
  // Plot the first point 
  plot_point2(x,y, midPointX, midPointY);
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
  plot_point2(x,y, midPointX, midPointY);
  }
}




























