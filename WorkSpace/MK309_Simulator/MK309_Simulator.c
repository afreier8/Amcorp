/******************************************************************************* 
 
   FILE: MK309_Simulator.c
 
   DESCRIPTION:
   This module contains the main functions to simulate the tactical display of the 
   MK 309 system.  This file was created in conjunction with the SPAWAR 
   Software Coding Guidelines document.
 
   FUNCTIONS:
   static void destroy(GtkWidget *widget, gpointer data)
   static void on_combo_RangeScale_changed ()
   int main(int argc, char *argv[])
 
   FILE CHANGE LIST:
   05/18/18 (ARF) ALways changing
 
*******************************************************************************/
#include <gtk/gtk.h>
#include "MK309_Simulator.h"
#include "MK309_GUI.c"
#include "TMA_Calculations.c"
#include "OwnshipCalc.c"
#include "TargetCalc.c"
#include "TacticalGraphics.c"
#include "TacticalSymbols.c"


/******************************************************************************
 ***************************** LOCAL VARIABLES ********************************
 ******************************************************************************/


/******************************************************************************
 *************************** LOCAL PROTOTYPES *********************************
 ******************************************************************************/


/******************************************************************************* 
   FUNCTION: GetGuiWidgets() and GetTMAItems
 
   DESCRIPTION:                                                               
	Gets the MK309_Simulator GUI widgets to be used in different functions. 
        Gets the different TMA items needed to calculate the targets motion.
   RETURN:                                                                    
    gwidget
 
   CHANGE LIST:
   05/15/18 (AF) Initial Coding
 
*******************************************************************************/
MK309_TMA *GetTMAitems()
{
   return &TMA_item;
}

GUI_WIDGETS *GetGuiWidgets()
{
   return &gwidget;
}

OWNSHIP *GetOwnshipItems()
{
   return &ownship;
}

TARGET *GetTargetItems()
{
   return &target;
}
/******************************************************************************* 
   FUNCTION: main(int argc, char *argv[])
 
   DESCRIPTION:                                                               
	Runs the main execution of the Mk 309 Simulator 
 
   RETURN:                                                                    
      Nothing
 
   CHANGE LIST:
   05/15/18 (AF) Initial Coding

 
*******************************************************************************/
int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

    // Initialize GLUT library 
    glutInit(&argc,argv);

	InitWidgets();
	
	gtk_main();

	return 0;
}







