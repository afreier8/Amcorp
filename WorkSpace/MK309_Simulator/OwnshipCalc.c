

double CalcVelocityAngleOwnship()
{
	OWNSHIP *ownship = GetOwnshipItems();

	ownship->velocity_angle = atan( ownship->Vy / ownship->Vx);
	
	//g_print("\nThe velocity angle is: %lf", ownship->velocity_angle);
	
	return ownship->velocity_angle;
}

void calcOwnshipVelocityEndPoint()
{
	OWNSHIP *ownship = GetOwnshipItems();

	double n;
	n = DetermineOwnshipHeadingVectorLength();

	double dist = sqrt(((ownship->endPointX - xc) * (ownship->endPointX - xc)) 
						+ ((ownship->endPointY - yc) * (ownship->endPointY - yc)));
	double r = n / dist;

	ownship->endPointX = r * ownship->endPointX + (1 - r) * xc;
	ownship->endPointY = r * ownship->endPointY + (1 - r) * yc;
	//g_print("\n\nOwnship ENDPOINTS x: %f  y: %f\n", ownship->endPointX, ownship->endPointY);

	OwnshipHistory(n, dist);
}

void OwnshipHistory(double n, double dist)
{
	OWNSHIP *ownship = GetOwnshipItems();

	n = n /*+ n*/;
	g_print("\n THe value of n %f", n);
	int tempDotDivide = 0;
	int dotSpotX[5];
	int dotSpotY[5];

	for (int i = 0; i < 5; i++)
	{
		tempDotDivide = tempDotDivide + n;
		double r = tempDotDivide / dist;

		dotSpotX[i] = (int) (r * ownship->endPointX + (1 - r) * xc) ;
		dotSpotY[i] = (int) (r * ownship->endPointY + (1 - r) * yc) ;

		g_print("\nDotSpotX = %d  at -%d- ", dotSpotX[i], i);
		g_print("\nDotSpotY = %d  at -%d- ", dotSpotY[i], i);

		int tempSpotX = dotSpotX[i] - xc;
		int tempSpotY = dotSpotY[i] - yc;
		g_print("\ntempSpotX = %d  ", tempSpotX);
		g_print("\ntempSpotY = %d  ", tempSpotY);

		dotSpotX[i] = xc - tempSpotX;
		dotSpotY[i] = yc - tempSpotY;

		g_print("\nDotSpotX2 = %d ", dotSpotX[i]);
		g_print("\nDotSpotY2 = %d  ", dotSpotY[i]);

		glColor3f(1.0, 1.0, 1.0);
		SmallCircle(1, dotSpotX[i], dotSpotY[i]);
	}
}

void DetermineTorpedoLineOfFire()
{
	OWNSHIP *ownship = GetOwnshipItems();
	TARGET *target = GetTargetItems();
   MK309_TMA *TMA_item = GetTMAitems(); 

	int lineofFireAngle = TMA_item->BearingToTarget;
	
	//g_print("\nAngle of LINE OF FIRE\ndy: %f  dx: %f\n dy/dx: %f", dy, dx, dy/dx); // CORRECT FOR (400,340)

	if (lineofFireAngle >= 270 || lineofFireAngle <= 90 )
	{
		g_print("\nBearing 270 - 360 to 0 - 90\n");

		if (target->CT >= 0 && target->CT <= 180 )
		{
			g_print("\nTarget heading 0 to 180\n");
			lineofFireAngle += 20;
		}
		else
		{
			g_print("\nTarget heading 180 to 360\n");
			lineofFireAngle -= 20;
		}
	}
	else
	{
		g_print("\nBearing 90 to 270\n");

		if (target->CT >= 0 && target->CT <= 180 )
		{
			g_print("\nTarget heading 0 to 180\n");
			lineofFireAngle -= 20;
		}
		else
		{
			g_print("\nTarget heading 180 to 360\n");
			lineofFireAngle += 20;
		}
	} 

	ownship->LOFX = xc + (int)(199 * sin(lineofFireAngle * ConvertToDegree));
	ownship->LOFY = yc + (int)(199 * cos(lineofFireAngle * ConvertToDegree));

	glColor3f(1.0, 1.0, 1.0);

	drawAcquisitionCircles();
}

void drawAcquisitionCircles()
{
	OWNSHIP *ownship = GetOwnshipItems();
	MK309_TMA *TMA_item = GetTMAitems(); 

	double dist = sqrt(((ownship->LOFX - xc) * (ownship->LOFX - xc)) 
						+ ((ownship->LOFY - yc) * (ownship->LOFY - yc)));

	double minAcq = 60 / dist; // may want to change number to variable
	double optAcq = 120 / dist;
	double maxAcq = 180 / dist;

	ownship->minAcqX = (int) (minAcq * ownship->LOFX + (1 - minAcq) * xc);
	ownship->minAcqY = (int) (minAcq * ownship->LOFY + (1 - minAcq) * yc);

	SmallCircle(2, ownship->minAcqX, ownship->minAcqY);

	ownship->optAcqX = (int) (optAcq * ownship->LOFX + (1 - optAcq) * xc);
	ownship->optAcqY = (int) (optAcq * ownship->LOFY + (1 - optAcq) * yc);

	SmallCircle(2, ownship->optAcqX, ownship->optAcqY);

	ownship->maxAcqX = (int) (maxAcq * ownship->LOFX + (1 - maxAcq) * xc);
	ownship->maxAcqY = (int) (maxAcq * ownship->LOFY + (1 - maxAcq) * yc);

	SmallCircle(2, ownship->maxAcqX, ownship->maxAcqY);

	glColor3f(0.0, 1.0, 0.5);

	glBegin(GL_LINES);
      glVertex2f(xc,yc);
		glVertex2f(ownship->maxAcqX, ownship->maxAcqY);
    glEnd;

	 glEnd();
	glFlush();
}

double DetermineOwnshipHeadingVectorLength()// Split into two different for heading and history
{
	OWNSHIP *ownship = GetOwnshipItems();
	double n;

	if ( ownship->DMHO >= 0 && ownship->DMHO <= 5 )
	{
		n = 40;
	}
	else if ( ownship->DMHO > 5 && ownship->DMHO <= 10 )
	{
		n = 44;
	}
	else if ( ownship->DMHO > 10 && ownship->DMHO <= 15 )
	{
		n = 49;
	}
	else if ( ownship->DMHO > 15 && ownship->DMHO <= 20 )
	{
		n = 61;
	}
	else // 21 - 25
	{
		n = 73;
	}
	return n;
}
