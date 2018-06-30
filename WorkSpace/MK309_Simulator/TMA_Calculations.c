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
void CalcRange(int x, int y)
{
	MK309_TMA *TMA_item = GetTMAitems();
	GUI_WIDGETS *gwidget = GetGuiWidgets();

	// Calculate the distance between ownship and target
	TMA_item->RangeToTarget = sqrt(((x - xc) * (x - xc))
	+ ((y - yc) * (y - yc)));
	// 20 knots/ inch of the screen
	// ~300 pixels per inch	
	TMA_item->RangeToTarget = CalcPixelDistance(TMA_item->RangeToTarget);
	// 1 knot = 2000 yrds/hr
	//g_print("\n\nThe Range to the target is: %d\n\n", TMA_item->RangeToTarget);

	char *range_ptr = malloc(10);
	sprintf(range_ptr, "%d", TMA_item->RangeToTarget);
	gtk_entry_set_text(GTK_ENTRY(gwidget->entry_Range), range_ptr);

	free(range_ptr);
}

void CalcBearing()
{
	OWNSHIP *ownship = GetOwnshipItems();
	TARGET *target = GetTargetItems();
	MK309_TMA *TMA_item = GetTMAitems();
	GUI_WIDGETS *gwidget = GetGuiWidgets();

	double convertToDegree = 180 /  PI;

	/*g_print("\n\nThe Bearing calculations are \nTARGET x: %d y:%d\nOwnship x:%d y:%d"
				, target->centerX, target->centerY, xc, yc);*/

	// get the difference of each coordinate
	// center of the window/ main circle is (xc, yc);
	double dx = target->centerX - xc;
	double dy = target->centerY - yc;
	
	//g_print("\ndy: %f  dx: %f\n dy/dx: %f", dy, dx, dy/dx); // CORRECT FOR (400,340)
	double  temp1Bearing = atan(dy/dx) * convertToDegree;
	//double	invTan = 1 / temp1Bearing;

	if (dx > 0.0)
	{
		double tempBearing =  90 - temp1Bearing; //works for TN quad IV
		//tempBearing = 360 - tempBearing;
		TMA_item->BearingToTarget = (int)tempBearing;
		//g_print("\n\ndx > 0.0\natan(y/x) = %f\n\nTemp Bearing = %f\n\n", temp1Bearing,tempBearing);
	}
	else if (dx < 0.0)
	{
		double tempBearing = 270 - temp1Bearing;
		TMA_item->BearingToTarget = tempBearing;
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

	g_print("\n\nBEARING: %d\n", TMA_item->BearingToTarget);

	char *bearing_ptr = malloc(10);
	sprintf(bearing_ptr, "%d", TMA_item->BearingToTarget);
	gtk_entry_set_text(GTK_ENTRY(gwidget->entry_Bearing), bearing_ptr);
	free(bearing_ptr);
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

double CalcPixelDistance(double distInPixels)
{
	double distInKnots, distInYards;
	// 20 knots/ inch of the screen
	// ~300 pixels per inch	
	// 1 knot = 2000 yrds/hr.... 15 pixels/ knot
	
	// convert from pixels to knots 
	distInKnots = distInPixels / 15;// returns how many total knots
	// then convert from knots to yards
	distInYards = distInKnots * 2000; //returns how many yards 
	return distInYards;
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

double CalcDistance(double x1, double y1, double x2, double y2)
{
	double dist = sqrt( ((x2-x1) * (x2-x1)) + ((y2-y1) * (y2-y1)) );

	//g_print("\nThe distance to the target in pixels = %lf\n", distance);

	return dist;
}

double CalcSpeed(double dist)
{
	MK309_TMA *TMA_item = GetTMAitems();
	clock_t time;

	time = clock() - TMA_item->time;
	double speed = dist / time;

	double time_taken = ((double)time)/ CLOCKS_PER_SEC;
	//g_print("\n\nThe time taken is %f\n   with the speed being %lf", time_taken, speed);

	return speed;
}








