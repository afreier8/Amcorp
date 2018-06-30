// Holder of all Ownship calculations
// distance between target and ownship
// ----> distance = sqrt( ((x2-x1)^2) + ((y2-y1)^2) )
// needed equations velocity vector,
/******************************************************************************* 
 
   FILE: MK309_Simulator.c
 
   DESCRIPTION:
   This module contains the main calculations for TMA.
 
   FUNCTIONS:
   void Calculations()
   void CalcRange()
   void CalcBearing()
 
   FILE CHANGE LIST:
   05/18/18 (ARF) ALways changing
 
*******************************************************************************/
void Calculations()
{
	OWNSHIP *ownship = GetOwnshipItems();
	TARGET *target = GetTargetItems();
	
	//ownship->CQO = convertTrueNorth(ownship->CQO);
	//target->CT =  convertTrueNorth(target->CT);

	//CalcVelocityVectorOwnship();
	/*CalcVelocityAngleOwnship();// probably not correct
	
	CalcVelocityVectorTarget();
	CalcVelocityAngleTarget();*/// not good

	
}

void CalcRange()
{
	OWNSHIP *ownship = GetOwnshipItems();
	TARGET *target = GetTargetItems();
	MK309_TMA *TMA_item = GetTMAitems();
	
	// Calculate the distance between ownship and target
	TMA_item->RangeToTarget = sqrt(((target->Vx - target->Vy) * (target->Vx - target->Vy))
	+ ((ownship->Vx - ownship->Vy) * (ownship->Vx - ownship->Vy)));
}

void CalcBearing()
{
	OWNSHIP *ownship = GetOwnshipItems();
	TARGET *target = GetTargetItems();
	MK309_TMA *TMA_item = GetTMAitems();

	double convertToDegree =180 /  PI;

	g_print("\n\nThe Bearing calculations are \nTARGET x: %d y:%d\nOwnship x:%d y:%d"
				, target->centerX, target->centerY, xc, yc);

	// get the difference of each coordinate
	// center of the window/ main circle is (xc, yc);
	g_print("\n(dy) = %d(target center Y) - %d(xc)\n",  target->centerY, yc);
	double dx = target->centerX - xc;
	double dy = target->centerY - yc;
	
g_print("\ndy: %f  dx: %f\n", dy, dx); // CORRECT FOR (400,340)
	double  temp1Bearing = atan(dy/dx) * convertToDegree;
	//double	invTan = 1 / temp1Bearing;

	if (dx > 0.0)
	{
		double tempBearing =  90 - temp1Bearing; //works for TN quad IV
		tempBearing = 360 - tempBearing;
		TMA_item->BearingToTarget = tempBearing;
		g_print("\n\ndx > 0.0\natan(y/x) = %f\n\nTemp Bearing = %f\n\n", temp1Bearing,tempBearing);
	}
	else if (dx < 0.0)
	{
		double tempBearing = 90 + temp1Bearing;
		TMA_item->BearingToTarget = tempBearing;
		g_print("\n\ndx < 0.0 && dy > 0\ntan(y/x) = %f\n\nTemp Bearing = %f\n\n", temp1Bearing,tempBearing);
	}
	else // dx == 0
	{
		if ( dy > 0 )
		{
			TMA_item->BearingToTarget = 0;
		}
		else if ( dy < 0)
		{
			TMA_item->BearingToTarget = 180;
		}
		else
		{
			TMA_item->BearingToTarget = 0;
		}
	}

	g_print("\n\nThe BEARING to target is: %f\n\n", TMA_item->BearingToTarget);
}

void CalcSignalTimeTravel()
{
	MK309_TMA *TMA_item = GetTMAitems();

	// conversion from Range scale in yards 
	// * 2 to get total round trip 
	// * 3 to convert to feet
	// sound travels through water 4800 at ft/sec
	TMA_item->signalTravelTime = 2 * TMA_item->RangeScale * 3 * 4800;

	g_print("\n The total amount of time for the synchro signal to travel is %lf"
					, TMA_item->signalTravelTime);
}

void CalcPixelDistance()
{
	// 20 knots/ inch of the screen
	// ~300 pixels per inch	

	// 1 knot = 2000 yrds/hr
}

int convertTrueNorth(int angle)
{
	// Convert the angle to have the 0 degree at true north
	if ( angle<=269 )
	{
		angle = angle + 90;
	}
	else // angle must reset after 359
	{
		angle = angle - 270;
	}
	return angle;
}








