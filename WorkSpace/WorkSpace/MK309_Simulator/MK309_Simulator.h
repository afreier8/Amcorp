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
#include <gtk/gtkglarea.h>
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


/******************************************************************************
 ***************************** DEFINES ****************************************
 ******************************************************************************/
#define TMA_CIRCLE_RADIUS           200
#define PI                          3.14159265
#define xc                          320
#define yc                          240


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
    bool        runTMA;
    int        RangeScale;
    double      RangeToTarget;
    double      BearingToTarget;
    double      signalTravelTime;
    bool        closeWindow;
    bool        graphicsThreadStarted;
    pthread_t   graphics_thread;
}MK309_TMA;

typedef struct
{
    int         CQO;
    int         DMHO;
    double      endPointX;
    double      endPointY;
    double 		Vx;
    double		Vy;
    double 		velocity_vector;
    double		velocity_angle;
}OWNSHIP;

typedef struct
{
    double      CT;
    double      DMHT;
    double      endPointX;
    double      endPointY;
    int         centerX;
    int         centerY;
    double 		Vx;
    double		Vy;
    double 		velocity_vector;
    double		velocity_angle;
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
void CalcRange();
void CalcBearing();
void CalcSignalTimeTravel();
void CalcPixelDistance();
int convertTrueNorth(int angle);

// TargetCalc.c
double CalcVelocityVectorTarget();
double CalcVelocityAngleTarget();
void calcTargetVelocityEndPoint();
double DetermineTargetHeadingVectorLength();
void drawTargetVelocityLine();

// OwnshipCalc.c
void CalcVelocityVectorOwnship();
double CalcVelocityAngleOwnship();
double DetermineOwnshipHeadingVectorLength();
void calcOwnshipVelocityEndPoint();

// Tactical Graphics
void tma_main();
void Init();
void drawOuterCircleLines();
void bresenham_circle(int r);
void plot_point(int x, int y);
void RedrawTargetTriangle(int x, int y, bool redTarget);
void StartGraphicsThread();
void drawOwnshipVelocityLine();

// Tactical Symbols
void drawOwnship();
void drawTargetTriangle(int vertex);
void circle();
void drawBadSubSurface();
void drawUnknownSurface();
void drawKnownGoodSurface();
void plot_point2(int x, int y, int midPointX, int midPointY);
void bresenham_circle2(int r, int midPointX, int midPointY);
void TorpedoInWater();
void drawKnownGoodSurface(double x, double y);


















