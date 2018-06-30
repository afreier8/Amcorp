double CalcVelocityVectorTarget()
{
	TARGET *target = GetTargetItems();

	target->Vx = cos(target->CT);
	target->Vy = sin(target->CT);
	
	target->velocity_vector = sqrt((target->Vx * target->Vx) + (target->Vy * target->Vy));
	g_print("\n\n");
	g_print("\n\nThe Target info ------>");
	g_print("\nVx: %lf \nVy:%lf", target->Vx, target->Vy);
	//g_print("\nThe velocity vector is: %lf", target->velocity_vector);
	
	return target->velocity_vector;
}

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

	double n;

	n = DetermineTargetHeadingVectorLength();

	double dist = sqrt(((target->endPointX - x) * (target->endPointX - x)) 
						+ ((target->endPointY - y) * (target->endPointY - y)));
	double r = n / dist;

	target->endPointX = r * target->endPointX + (1 - r) * x;
	target->endPointY = r * target->endPointY + (1 - r) * y;
}

double DetermineTargetHeadingVectorLength()
{
    TARGET *target = GetTargetItems();
	double n;

	if ( target->DMHT >= 0 && target->DMHT <= 5 )
	{
		n = 25;
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






