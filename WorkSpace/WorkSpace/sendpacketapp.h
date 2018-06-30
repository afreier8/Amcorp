#pragma once
#include <stdio.h>
//using namespace std;

typedef struct 
{
	char message;
	int size;
} HEADER;

typedef struct
{
	HEADER hdr;
	char myButtonStr[20];
	int height;
	int width;
	int x;
	int y;
}NEW_BUTTON;

typedef struct 
{
   int                      commPort;
	int                      commPort2;
   bool                     commStarted;
   bool						readPort;
   NEW_BUTTON				*newButton;
   const gchar*				inputString;
   GtkWidget 				*btn_Send;
   GtkWidget 				*btn_Recieve;
   GtkWidget				*btn_Start;
   GtkWidget				*btn_Quit;
   GtkWidget				*entry_inputWidth;
   GtkWidget				*entry_input3;
   GtkWidget				*entry_input4;
   GtkWidget				*entry_input5;
   GtkWidget				*entry_input6;
   GtkWidget				*entry_inputHeight;
   GtkBuilder 				*builder;
   GtkWidget 				*window;
   GtkWidget				*progressBar;
   GtkWidget				*textView;
} GLOBAL_DATA;

typedef struct
{
   char						testType;
   bool						readComm;
   int						runMaintTest;                  
   int						runConnectTest;
   char						header;
   char						color;
   char						logType;   
} TEST_INFO;

// Handles messages RECIEVED
typedef struct _DISPLAY_MESSAGE_1
{	
	HEADER hdr;
	char myTitle[20];
}DISPLAY_MESSAGE_1;

typedef struct _CHANGE_COLOR
{
	HEADER hdr;
	char myTest[20];

}CHANGE_COLOR;

// Handles messages SENT
typedef struct _DISPLAY_MESSAGE_2
{	
	HEADER hdr;
	char myString[40];
}DISPLAY_MESSAGE_2;

typedef struct _DISPLAY_MESSAGE_3
{	
	HEADER hdr;
	char myString[40];
}DISPLAY_MESSAGE_3;


void InitComm();
void initWidgets();
void stepProgressBar();
// Forward declarations of functions included in this code module:
/*LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void InitCommSPA();
void SendPacket();
void SetLog(LPCTSTR outputString);
void AssignValuesSPA( char *buffer);*/
