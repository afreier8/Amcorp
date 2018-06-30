

void CalcVelocityVectorOwnship()
{
    OWNSHIP *ownship = GetOwnshipItems();

	double convertToDegree = PI / 180;

	ownship->Vx = sin(ownship->CQO * convertToDegree);
	ownship->Vy = cos(ownship->CQO * convertToDegree);

	g_print("\n\nOwnship calculations ---->");
	g_print("\n\nOwnship heading or angle: %d", ownship->CQO);
	g_print("\nVx: %f \nVy: %f", ownship->Vx, ownship->Vy);
	//g_print("\nThe velocity vector is: %lf", ownship->velocity_vector);
}

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
}

double DetermineOwnshipHeadingVectorLength()
{
	OWNSHIP *ownship = GetOwnshipItems();
	double n;

	if ( ownship->DMHO >= 0 && ownship->DMHO <= 5 )
	{
		n = 25;
	}
	else if ( ownship->DMHO > 5 && ownship->DMHO <= 10 )
	{
		n = 37;
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