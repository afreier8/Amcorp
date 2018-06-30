#pragma once


// Header string for specific test
#define HEADER_TEST1			0x2
#define HEADER_TEST2			0x3

#define HEADER_TEST1STR			"Test 1"

#define CREATE_NEW_DIALOG		0x4

// Sets the color of the dialog window
#define COLOR_BLUE				0x001
#define COLOR_RED				0x002
#define COLOR_GREEN				0x003

// Test selection
#define RUN_CONNECTION_TEST		0x000004
#define RUN_MAINT_TEST			0x000005

// Test PASS or FAIL
#define TESTINFO_MESSAGE		0x057
#define FAIL_MESSAGE			0x058
#define PASS_CONNECTION_TEST	0x060
#define CHANGE_COLOR_RED		0X9
#define ENABLE_LAUNCH			0x4
#define BLACKOUT				0x3
#define CHANGE_HEIGHT			0x7


void on_btn_recieve_clicked();
// Forward declarations of functions included in this code module:
/*LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ListeningThread(LPVOID lpParameter);
DWORD WINAPI RecieveComm();
void SendComm();
void SetLog(LPCTSTR outputString);
void AssignValues( char *buffer);
void SetTestLog(LPCTSTR outputString);
void InitCommCPT();
INT_PTR CALLBACK PopupDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);*/
