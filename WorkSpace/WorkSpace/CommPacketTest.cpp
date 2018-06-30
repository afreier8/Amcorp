// 
// CommPacketTest.cpp : Recieves Serial Comm data and handles the data accordingly
//
//	TODO: Add more data to be sent to other app to better 2-way comm traffic
//			Add comm handle check to check for time outs
//			Add more "checks" to make sure data is not manipulated and can send correctly
//			Add more button functionality
//

#include "stdafx.h"
#include "CommPacketTest.h"
#include "SendPacketApp.h"
#include <iostream>
#include <commctrl.h>
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <atlbase.h>
#include <atlconv.h>
#include <cstring>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
GLOBAL_DATA			gdata;
TEST_INFO			testInfo;
NEW_BUTTON			newButton;
bool	changeColorFlag = false;


GLOBAL_DATA *GetGlobalData()
{
   return &gdata;
}

NEW_BUTTON *GetNewButton()
{
	return &newButton;
}

TEST_INFO *GetTestInfo()
{
	return &testInfo;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{	
	MSG msg;
	HACCEL hAccelTable;
	GLOBAL_DATA *gdata = GetGlobalData();

	gdata->hInst = hInstance;
	gdata->hMainWnd = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_TESTCOMMDLG),NULL,(DLGPROC)WndProc);

	// Perform application initialization:
	if (gdata->hMainWnd != NULL)
	{
		ShowWindow(gdata->hMainWnd, nCmdShow);
		UpdateWindow(gdata->hMainWnd);
		
		hAccelTable = LoadAccelerators(gdata->hInst, MAKEINTRESOURCE(IDC_COMMPACKETTEST));

		// Main message loop:
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	GLOBAL_DATA *gdata = GetGlobalData();
	gdata->hDlg = hDlg;

	switch (message)
	{
		case WM_INITDIALOG:
			
			// Init new dialog items
			gdata->hList = GetDlgItem(hDlg, IDC_LOGLIST1);
			gdata->setCommPort = GetDlgItem(hDlg, IDC_COMMEDIT);
			gdata->testList = GetDlgItem(hDlg, IDC_TESTLOGLIST);
			gdata->checkTest1PASS = GetDlgItem(hDlg, IDC_CHECKPASS);
			gdata->checkTest1FAIL = GetDlgItem(hDlg, IDC_CHECKFAIL);
			gdata->progressCPT = GetDlgItem(hDlg, IDC_PROGRESS_CPT);
			gdata->test1Text = GetDlgItem(hDlg, IDC_STATICTEST1);
			SendMessage(gdata->checkTest1FAIL,BM_SETCHECK, BST_CHECKED,0);

			break;
		case WM_CTLCOLORSTATIC:
			if(((HWND)lParam == (GetDlgItem(hDlg,IDC_TEST1))) || ((HWND)lParam == (GetDlgItem(hDlg,IDC_TEST2))) 
				|| ((HWND)lParam == (GetDlgItem(hDlg,IDC_CHECKPASS))) || ((HWND)lParam == (GetDlgItem(hDlg,IDC_CHECKFAIL))) )
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(0,0,255));

				return (BOOL)GetSysColorBrush(COLOR_MENU);
			}
			
			if(((HWND)lParam == (GetDlgItem(hDlg,IDC_CONNECT))) || ((HWND)lParam == (GetDlgItem(hDlg,IDC_STATICTEST1))) 
				|| ((HWND)lParam == (GetDlgItem(hDlg,IDC_STATICTEST2))))  
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255,0,0));

				return (BOOL)GetSysColorBrush(COLOR_MENU);
			}			
			
			if(((HWND)lParam == (GetDlgItem(hDlg,IDC_CONNECTPASS))) || ((HWND)lParam == (GetDlgItem(hDlg,IDC_TEST1PASS))) 
				|| ((HWND)lParam == (GetDlgItem(hDlg,IDC_TEST2PASS))))  
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(0,255,0));

				return (BOOL)GetSysColorBrush(COLOR_MENU);
			}	
			
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(gdata->hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hDlg);
					break;
				case IDOK:
					EndDialog(hDlg, LOWORD(wParam));
					break;
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
				case IDC_RECIEVEMESSAGE:
					RecieveComm();
					break;
				case IDC_SENDMESSAGE:
					SendComm();
					break;
				default:
					return DefWindowProc(hDlg, message, wParam, lParam);
			}
			break;
		case WM_CTLCOLOREDIT:
			if(changeColorFlag)
			{
				HBRUSH hbr = (HBRUSH) DefWindowProc(hDlg, message, wParam, lParam);
				SetTextColor((HDC) wParam, RGB(255,0,0));
				return (BOOL) GetSysColorBrush(COLOR_MENU);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hDlg, &ps);
			EndPaint(hDlg, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hDlg, message, wParam, lParam);
	}

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// ListeningThread - listens for any messages that are sent across the
//		serial port and sends the message off to set values accordingly
//
////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ListeningThread(LPVOID lpParameter)
{
	GLOBAL_DATA *gdata = GetGlobalData();
	TEST_INFO *testInfo = GetTestInfo();
	int wmEvent;
	char buffer[30];

	DWORD nbr,nbtr=1;
	char msg1 = ' ';
	char msg2 = ' ';
	char msg3 = ' ';
	int count = 0;
	int result;
	HEADER *hdr;
	
	// set some type of warning of serial comm activity
	HANDLE eventStatus = CreateEvent(NULL,true,false,NULL);
	
	SetLog(L"Listening Thread Entry.");

	if(gdata->hCom == INVALID_HANDLE_VALUE)
		SetLog(L"Invalid Handle Value");
	
	if(!(SetCommMask(gdata->hCom, EV_RXCHAR)))
	{
		int error = GetLastError();
		SetLog(L" ERROR ");
	}
	
	// Waits for interrupt with serial data to recieve
	WaitCommEvent(gdata->hCom,(LPDWORD)eventStatus, 0);
	
	SetCommMask(gdata->hCom, EV_RXCHAR);

	SetLog(L"Inside readComm while loop waiting to recieve...");

	while( EV_RXCHAR && testInfo->readComm )
	{
		// set the memory in the buffer to be 0
		memset(buffer, 0, 30);

		SetLog(L"Recieving Comm over port.");

		int size = sizeof(HEADER);

		result = ReadFile(gdata->hCom,buffer,sizeof(HEADER),&nbr,NULL);		
		int error = GetLastError();

		if(result)
		{				
			if (nbr == sizeof(HEADER))
			{
				hdr = (HEADER *)buffer;
				result = ReadFile(gdata->hCom,buffer+sizeof(HEADER),hdr->size - sizeof(HEADER),&nbr,NULL);
			}
			AssignValues((char *)buffer);			
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////
//
// RecieveComm - Creates the listening thread that loops to recieve
//		messages over the serial port
//
/////////////////////////////////////////////////////////////////////////
DWORD WINAPI RecieveComm()
{
	GLOBAL_DATA *gdata = GetGlobalData();
	TEST_INFO *testInfo = GetTestInfo();

	testInfo->readComm = true;

	InitCommCPT();

	// Call Listening Thread to wait for packet to be recieved
	if (CreateThread(NULL,0,ListeningThread,gdata,NULL,NULL) == NULL)
	{
		SetLog(L"Listening thread unable to start.");
	}
		
	return 0;
}

//////////////////////////////////////////////////////////////////
//
// SendComm sends a simple message to test the serial comm port
//
//////////////////////////////////////////////////////////////////
void SendComm()
{
	GLOBAL_DATA *gdata = GetGlobalData();
	DWORD bytesWritten;

	char buffer[] = "Written across serial the WinCE old way";

	WriteFile(gdata->hCom, &buffer,1, &bytesWritten,NULL);			
	   
	CloseHandle(gdata->hCom);
}

///////////////////////////////////////////////////////////////////////////////////
// SetLog / SetTestLog - Prints a specific message to the LogList to view in the dialog
//		window
//
//		param - string to be output to dialog window
//
///////////////////////////////////////////////////////////////////////////////////
void SetLog(LPCTSTR outputString)
{
	GLOBAL_DATA *gdata = GetGlobalData();

	SendMessage(gdata->hList,LB_INSERTSTRING, 0,(LPARAM)outputString);
}

void SetTestLog(LPCTSTR outputString)
{
	GLOBAL_DATA *gdata = GetGlobalData();

	SendMessage(gdata->testList,LB_INSERTSTRING, 0,(LPARAM)outputString);
}

/////////////////////////////////////////////////////////////////////////////////////
// AssignValues - determines which message was sent over serial and then performs 
//		the task associated with the message recieved
//
//		param - ( char *buffer ) - pointer to the memory value associated with 
//			wanted messages and/or settings
//
////////////////////////////////////////////////////////////////////////////////////
void AssignValues( char *buffer)
{
	GLOBAL_DATA *gdata = GetGlobalData();
	TEST_INFO *testInfo = GetTestInfo();
	NEW_BUTTON *newButton = GetNewButton();
	
	HEADER *hdr;
	DWORD nbw;
	DISPLAY_MESSAGE_1 *msg1;
    DISPLAY_MESSAGE_2 msg2;
	std::string titleStr;
	bool result = false;

	hdr = (HEADER *)buffer;

	switch( hdr->message )
	{
		case TESTINFO_MESSAGE:
			// Set all test data to be CONNECT test
			msg1 = (DISPLAY_MESSAGE_1 *)buffer;
			SetLog(_T("Test Info Recieved"));		
			SendMessage(gdata->progressCPT,PBM_SETPOS,3,0);

			// Update the Title to say PASS test, check boxes
			SendMessage(GetDlgItem(gdata->hMainWnd,IDC_CHECKPASS),BM_SETCHECK, BST_CHECKED,0);
			SendMessage(GetDlgItem(gdata->hMainWnd,IDC_CHECKFAIL),BM_SETCHECK, BST_UNCHECKED,0);

			// switch connect to green since the apps can talk to each other
			ShowWindow(GetDlgItem(gdata->hMainWnd,IDC_CONNECT),SW_HIDE);
			ShowWindow(GetDlgItem(gdata->hMainWnd,IDC_CONNECTPASS),SW_SHOW);

			msg2.hdr.message = (char) 0x060;
			strcpy_s(msg2.myString, "Connection Test PASS");
			msg2.hdr.size = sizeof(msg2);
			
			// Send data to SendPacketApp about connect test
			if(!(WriteFile(gdata->hCom, (char *)&msg2,sizeof(msg2),&nbw,NULL)))
			{
				SetLog(_T("Message about Connect Test not sent"));
			}
			
			PurgeComm(gdata->hCom,PURGE_RXABORT|PURGE_TXABORT|PURGE_RXCLEAR|PURGE_TXCLEAR);		
			//testInfo->readComm = false;
			break;
		case HEADER_TEST1:
			SetLog(_T("Setting new window header string"));
			SetWindowText(gdata->hMainWnd,(LPCWSTR)HEADER_TEST1STR);

			break;
		case CHANGE_HEIGHT:
			
			break;
		case CREATE_NEW_DIALOG:
			newButton = (NEW_BUTTON *)buffer;
			gdata->newButton = (NEW_BUTTON *)newButton; 
			
			SetLog(L"Creating new window");
			SetTestLog(_T("Test 1"));
			ShowWindow(GetDlgItem(gdata->hMainWnd,IDC_STATICTEST1),SW_HIDE);
			ShowWindow(GetDlgItem(gdata->hMainWnd,IDC_TEST1PASS),SW_SHOW);

			msg2.hdr.message = (char) 0x3;
			strcpy_s(msg2.myString, "BLACKOUT");
			msg2.hdr.size = sizeof(msg2);
			
			// Send data to SendPacketApp about connect test
			if(!(WriteFile(gdata->hCom, (char *)&msg2,sizeof(msg2),&nbw,NULL)))
			{
				SetLog(_T("Message about Connect Test not sent"));
			}
			
			DialogBox(gdata->hInst, MAKEINTRESOURCE(IDD_POPUP), gdata->hDlg, PopupDialog);

			ListeningThread(0);
			break;
		case CHANGE_COLOR_RED:
			// toggle change color flag
			changeColorFlag = !changeColorFlag;
			ShowWindow(GetDlgItem(gdata->hMainWnd,IDC_STATICTEST2),SW_HIDE);
			ShowWindow(GetDlgItem(gdata->hMainWnd,IDC_TEST2PASS),SW_SHOW);
			InvalidateRect(gdata->test1Text, NULL, true);
			RedrawWindow(gdata->hDlg, NULL,NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				
			break;
		default:
			SetLog(_T("Cannot find value to assign!"));
			break;
	}
}

void InitCommCPT()
{
	GLOBAL_DATA *gdata = GetGlobalData();
	int max = 5000;
	COMMTIMEOUTS cto;
	DCB dcb;

	SetWindowText(gdata->setCommPort, L"COM27");
	SendMessage(gdata->progressCPT,PBM_SETRANGE,0,MAKELPARAM(0,3));
	SendMessage(gdata->progressCPT,PBM_SETPOS,1,0);

	SetLog (L"Ready to recieve Comm over port.");

		// must have all the extra stuff for comm ports over 10
	gdata->hCom = CreateFile(L"\\\\.\\COM27",  
                   GENERIC_READ | GENERIC_WRITE, 
                   0, 
                   0, 
                   OPEN_EXISTING,
                   NULL,
                   NULL);
       
	   if (gdata->hCom == INVALID_HANDLE_VALUE)
       {
          SetLog(L"Failure: Unable to open port");  
			int error = GetLastError();
       }
       else
       {
		  SetLog(L"-------> COM27 Initializing");
          //  Initialize the DCB structure.
          ZeroMemory(&dcb, sizeof(DCB));
          dcb.DCBlength = sizeof(DCB);

          dcb.BaudRate = CBR_9600;      //  baud rate
          dcb.ByteSize = 8;             //  data size, xmit and rcv
          dcb.Parity   = NOPARITY;      //  parity bit
          dcb.StopBits = ONESTOPBIT;    //  stop bit

          // Set new state.
          if (!SetCommState(gdata->hCom, &dcb))
          {
			SetLog(L"ERROR -- Cannot set the Comm state correctly.");         
          }

		  cto.ReadIntervalTimeout = 0;
          cto.ReadTotalTimeoutMultiplier = 0;
          cto.ReadTotalTimeoutConstant = 0;
          cto.WriteTotalTimeoutMultiplier = 0;
          cto.WriteTotalTimeoutConstant = 0;
	
		  if(SetCommTimeouts(gdata->hCom,&cto) == 0)
		  {
			  //ASSERT(0);
			  SetLog(L"ERROR -- Cannot set the Comm timeouts correctly.");             
		  }
		  else
		  {
			  SetLog(L"Comm port opened successfully.");
		  }
	   }
}

INT_PTR CALLBACK PopupDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	GLOBAL_DATA *gdata = GetGlobalData();
	NEW_BUTTON *newButton = GetNewButton();
	int wmEvent;
	HWND createdButton;
	HBRUSH brush = 0;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:	
		createdButton = CreateWindow(L"BUTTON",L"New Button",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
 			gdata->newButton->x, gdata->newButton->y, gdata->newButton->width, gdata->newButton->height, hDlg, (HMENU) IDC_NEWBUTTON ,gdata->hInst, NULL);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmEvent = HIWORD(wParam);
		
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDC_NEWBUTTON)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	case WM_ERASEBKGND:
		brush = CreateSolidBrush(RGB(0,0,100));
		SetClassLongPtr(hDlg, GCLP_HBRBACKGROUND, (LONG)brush);
		return (INT_PTR)TRUE;
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:
		return (INT_PTR)GetStockObject(GRAY_BRUSH);
		//GetStockObject(SetDCBrushColor(hdc, RGB(255,0,255)));
		// turns the dialog fuscia but cant get proper hdc to control 
		//GRAY_BRUSH
	}

	return (INT_PTR)FALSE;
}



