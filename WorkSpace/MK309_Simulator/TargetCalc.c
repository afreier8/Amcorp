

double CalcVelocityAngleTarget()
{
	TARGET *target = GetTargetItems();

	target->velocity_angle = atan( target->Vy / target->Vx);
	
	//g_print("\nThe velocity angle is: %lf\n", target->velocity_angle);
	
	return target->velocity_angle;
}

void calcTargetVelocityEndPoint(int x, int y)
{
    TARGET *target = GetTargetItems();
	 MK309_TMA *TMA_item = GetTMAitems();

	double n;

	n = DetermineTargetHeadingVectorLength();

	double dist = sqrt(((target->endPointX - x) * (target->endPointX - x)) 
						+ ((target->endPointY - y) * (target->endPointY - y)));
	double r = n / dist;

	target->endPointX = r * target->endPointX + (1 - r) * x;
	target->endPointY = r * target->endPointY + (1 - r) * y;

	if (TMA_item->targetWithinRange)
	{
		// pass the parameter to find the length of the velocity vector
		TargetHistory(n, dist, x, y);
	}
		//g_print("\nTarget velocity vector endpoints x: %f  y: %f\n", target->endPointX, target->endPointY);
}

void TargetHistory(double n, double dist, int x, int y)
{
	TARGET *target = GetTargetItems();

	double n1 = n + n;
	double n2 = n + n + n;
//g_print("\nThe value of n is %f n2 %f n3 %f\n", n, n1, n2);

	int tempDotDivide = 0;
	int dotSpotX1;
	int dotSpotY1;
	int dotSpotX2;
	int dotSpotY2;
	int deltaSpotY;
	int deltaSpotX;

	double r1 = n1 / dist;
	double r2 = n2 / dist;

	dotSpotX1 = (int) (r1 * target->endPointX + (1 - r1) * x) ;
	dotSpotY1 = (int) (r1 * target->endPointY + (1 - r1) * y) ;

	dotSpotX2 = (int) (r2 * target->endPointX + (1 - r2) * x) ;
	dotSpotY2 = (int) (r2 * target->endPointY + (1 - r2) * y) ;

	deltaSpotX = dotSpotX2 - dotSpotX1;
	deltaSpotY = dotSpotY2 - dotSpotY1;

	/*g_print("\nTempX %d = %d - %d ", deltaSpotX, dotSpotX2, dotSpotX1);
	g_print("\nTempY %d = %d - %d \n", deltaSpotY, dotSpotY2, dotSpotY1);*/
	
	for ( int i = 1; i < 6; i++)
	{
		int targetHistoryX = target->centerX - (deltaSpotX * i);
		int targetHistoryY = target->centerY - (deltaSpotY * i);

		/*g_print("\nDifference x: %d y: %d", deltaSpotX, deltaSpotY);
		g_print("\nThe target center point is x: %d y: %d", target->centerX, target->centerY);
		g_print("\nThe history points are x: %d y: %d\n", targetHistoryX, targetHistoryY);*/

		glColor3f(1.0, 0.0, 0.0);
		SmallCircle(1, targetHistoryX, targetHistoryY);
	}
}

double DetermineTargetHeadingVectorLength()// Split into two different for heading and history
{
   TARGET *target = GetTargetItems();
	double n;

	if ( target->DMHT >= 0 && target->DMHT <= 5 )
	{
		n = 35;
	}
	else if ( target->DMHT > 5 && target->DMHT <= 10 )
	{
		n = 37;
	}
	else if ( target->DMHT > 10 && target->DMHT <= 15 )
	{
		n = 49;
	}
	else if ( target->DMHT > 15 && target->DMHT <= 20 )
	{
		n = 61;
	}
	else // 21 - 25
	{
		n = 73;
	}

	return n;
}

bool TargetWithinRange(int x, int y)
{
    MK309_TMA *TMA_item = GetTMAitems(); 

	// determine if the target location is valid
	int tempX;
	int tempY;
	int dist;

	dist = sqrt(((x - xc) * (x - xc)) + ((y - yc) * (y - yc)));

	if(dist >= (int)TMA_CIRCLE_RADIUS)
	{
		g_print("\nTarget NOT Within Range\n");
		TMA_item->targetWithinRange = false;
	}
	else
	{
		g_print("\nTarget Within Range\n");
		TMA_item->targetWithinRange = true;
	}

	return TMA_item->targetWithinRange;
}






