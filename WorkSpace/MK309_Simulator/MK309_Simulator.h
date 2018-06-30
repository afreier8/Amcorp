/******************************************************************************* 
 
   FILE: MK309_Simulator.h
 
   April Freier
   SPAWAR Systems Center Pacific
   4297 Pacific Highway
   San Diego, CA 92110
   (619) 553-3295
 
   DESCRIPTION:
   This header contains the definitions, external declarations, structures,
   and function prototypes necessary for the MK309_Simulator project.
 
   APPLICATION CHANGE HISTORY: (*application change history only in
   MK309_Simulator.h, Not in all header files in the project *)
   05/15/18 (AF) Initial coding.

 
   FILE CHANGE LIST:
   01/01/12 (BTG) Modified VERSION to 1.0.7.
                  Added APP_NEW_DEFINE.
 
*******************************************************************************/
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <time.h>


/******************************************************************************
 ***************************** DEFINES ****************************************
 ******************************************************************************/
#define TMA_CIRCLE_RADIUS           200
#define PI                          3.14159265
#define xc                          320
#define yc                          240
#define ConvertToDegree					PI / 180

/* Speed factor or the pixels per knot
   SPEED_FACTOR = 1 / ( 20 knots/inch * 4.25 inch/ 198 pixels */
#define SPEED_FACTOR 					2.3255



/******************************************************************************
 *************************** GLOBAL STRUCT ************************************
 ******************************************************************************/
typedef struct 
{
    GtkWidget           *window;
    GtkWidget           *TMA_window;
    GtkWidget           *btn_set;
    GtkBuilder          *builder;
    GtkWidget           *rangeScaleCombo;
    GtkWidget           *entry_CQO;
    GtkWidget           *entry_DMHO;
    GtkWidget           *entry_CT;
    GtkWidget           *entry_DMHT;
    GtkWidget           *entry_Range;
    GtkWidget           *entry_Bearing;
    GtkWidget           *text_view;
    GtkTextBuffer       *textBuffer;
    GtkWidget           *gl_area;
}GUI_WIDGETS;

typedef struct
{
    double        CQO_input;
    double        DMHO_input;
    double        CT_input;
    double        DMHT_input;
    double        Range_input;
    double        Bearing_input;
    bool          runTMA;
	 int         	RangeScale;
	 int         	RangeToTarget;
	 int         	BearingToTarget;
	 double      	signalTravelTime;
	 bool        	closeWindow;
	 bool        	graphicsThreadStarted;
	 pthread_t   	graphics_thread;
	 bool        	targetWithinRange;
	 clock_t			time;	
}MK309_TMA;

typedef struct
{
    int        CQO;
    int        DMHO;
    double     endPointX;
    double     endPointY;
	 int			LOFX;
	 int	      LOFY;
    double 		Vx;
    double		Vy;
    double 		velocity_vector;
    double		velocity_angle;
	 int 			minAcqX;
	 int 			minAcqY;
	 int 			optAcqX;
	 int 			optAcqY;
	 int 			maxAcqX;
	 int 			maxAcqY;
}OWNSHIP;

typedef struct
{
    int         CT;
    int         DMHT;
    double      endPointX;
    double      endPointY;
    int         centerX;
    int         centerY;
    double 		Vx;
    double		Vy;
    double 		velocity_vector;
    double		velocity_angle;
    bool        targetOutOfRange;
}TARGET;


/******************************************************************************
 ***************************** FUNCTIONS **************************************
 ******************************************************************************/
//  MK309GUI.c
OWNSHIP ownship; 
TARGET target;
MK309_TMA TMA_item;
GUI_WIDGETS gwidget;
bool checkTMAinputs();
MK309_TMA *GetTMAitems();
GUI_WIDGETS *GetGuiWidgets();
OWNSHIP *GetOwnshipItems();
TARGET *GetTargetItems();

void on_combo_RangeScale_changed (GtkComboBox *widget, gpointer user_data);
void on_btn_set_clicked();
void on_btn_play_clicked();
void on_btn_stop_clicked();
void InitWidgets();

/*static void entry_activated_CQO(GtkWidget *entry, gpointer data);
static void entry_activated_DMHO(GtkWidget *entry, gpointer data);
static void entry_activated_CT(GtkWidget *entry, gpointer data);
static void entry_activated_DMHT(GtkWidget *entry, gpointer data);
static void entry_activated_Range(GtkWidget *entry, gpointer data);
static void entry_activated_Bearing(GtkWidget *entry, gpointer data);*/

// TMA_Calculations.c
void Calculations(/*int CQO, int*/);
void CalcRange(int x, int y);
void CalcBearing();
void CalcSignalTimeTravel();
double CalcPixelDistance(double distInPixels);
int convertTrueNorth(int angle);
double CalcDistance(double x1, double y1, double x2, double y2);
double CalcSpeed(double dist);

// TargetCalc.c
double CalcVelocityAngleTarget();
void calcTargetVelocityEndPoint();
double DetermineTargetHeadingVectorLength();
void drawTargetVelocityLine();
bool TargetWithinRange(int x, int y);
void TargetHistory(double n, double dist, int x, int y);

// OwnshipCalc.c
double CalcVelocityAngleOwnship();
double DetermineOwnshipHeadingVectorLength();
void calcOwnshipVelocityEndPoint();
void DetermineTorpedoLineOfFire();
void drawAcquisitionCircles();
void OwnshipHistory(double n, double dist);

// Tactical Graphics
void tma_main();
void Init();
void drawOuterCircleLines();
void OuterCircle(int r);
void plot_point(int x, int y);
void RedrawTargetTriangle(int x, int y, bool redTarget);
void StartGraphicsThread();
void drawOwnshipVelocityLine();
void RenderString(int x, int y, char *string);

// Tactical Symbols
void drawOwnship();
void drawTargetTriangle(int vertex);
void circle();
void drawBadSubSurface();
void drawUnknownSurface();
void drawKnownGoodSurface();
void plot_point2(int x, int y, int midPointX, int midPointY);
void SmallCircle(int r, int midPointX, int midPointY);
void TorpedoInWater();
void drawKnownGoodSurface(double x, double y);
void TorpedoInterceptionPoint(double x, double y);


















