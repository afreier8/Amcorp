// 
// SendStuffCommApp.cpp : Mainly sends the comm message to be handled by the CommPacketTest app.
//
//	TODO: Add more recieve functionality for better 2-way communication
//			Add better comm time out checks
//
#include "stdafx.h"

#include "SendpacketApp.h"
#include "CommPacketTest.h"
#include <iostream>
#include <commctrl.h>
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <atlbase.h>
#include <atlconv.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
GLOBAL_DATA		gdata;
COMMTIMEOUTS	cto;
NEW_BUTTON		newButton;

GLOBAL_DATA *GetGlobalData()
{
   return &gdata;
}

NEW_BUTTON *GetNewButton()
{
   return &newButton;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	GLOBAL_DATA *gdata = GetGlobalData();

	gdata->hInst = hInstance;
	gdata->hSmallPacket = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_SENDDLG),NULL,(DLGPROC)WndProc);

	// Perform application initialization:
	if (gdata->hSmallPacket != NULL)
	{
		ShowWindow(gdata->hSmallPacket, nCmdShow);
		UpdateWindow(gdata->hSmallPacket);
		
		hAccelTable = LoadAccelerators(gdata->hInst, MAKEINTRESOURCE(IDC_SENDPACKETAPP));

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
	NEW_BUTTON newButton;
	DWORD nbw;
	DISPLAY_MESSAGE_1 msg1;
	DISPLAY_MESSAGE_2 msg2;
	CHANGE_COLOR changeColor;
	bool result = false;
	char buffer[200];
	
	int count = 0;
	HBRUSH hColorBrush = 0;

	switch (message)
	{
		case WM_INITDIALOG:

				gdata->hList = GetDlgItem(hDlg, IDC_LOGLIST);
				gdata->sendNewButton = GetDlgItem(hDlg, IDC_SENDNEWBUTTON); 
				gdata->progressSPA = GetDlgItem(hDlg, IDC_PROGRESS_SPA);
				gdata->testTypeButton = GetDlgItem(hDlg, IDC_TESTTYPE);
				gdata->changeColorButton = GetDlgItem(hDlg, IDC_CHANGECOLOR);
				gdata->newHeightButton = GetDlgItem(hDlg, IDC_SENDNEWHEIGHT);
				
				// Initialize the comm port for serial communication
				InitCommSPA();

			break;
		case WM_CTLCOLORBTN:
			if(!hColorBrush) hColorBrush = CreateSolidBrush(RGB(0,0,255));
			return (LRESULT) hColorBrush;
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
					break;
				case IDOK:
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
				case IDC_SENDBUTTON:
					
					break;
				case IDC_SENDNEWBUTTON:
					newButton.hdr.message = (char) CREATE_NEW_DIALOG;
					
					memset(newButton.myButtonStr, 20, 0);
					
					strcpy_s(newButton.myButtonStr, "New Button");
					newButton.height = 50;
					newButton.width = 200;
					newButton.x = 50;
					newButton.y = 50;
					newButton.hdr.size = sizeof(newButton);

					if(!(result = WriteFile(gdata->hCom, (char *)&newButton, sizeof(newButton),&nbw,NULL)))
					{
						int error = GetLastError();
						SetLog(L"Not able to write new button");
						break;
					}

					ListeningThread(0);

					break;
				case IDC_SENDNEWHEIGHT:
					strcpy_s(msg1.myTitle, "Change Height");
					msg1.hdr.message = (char) 0x7;
					msg1.hdr.size = sizeof(msg1); 
					
					if(!(WriteFile(gdata->hCom, (char *)&msg1, sizeof(msg1),&nbw,NULL)))
						SetLog(L"Unable to to write overcomm SEND NEW WEIGHT");

					break;
				case IDC_CHANGECOLOR:
					strcpy_s(changeColor.myTest, "Test 1");
					changeColor.hdr.message = (char) 0x9;
					changeColor.hdr.size = sizeof(changeColor); 

					if (gdata->hCom == INVALID_HANDLE_VALUE)
					{
						printf("INVALID HANDLE VALUE");
					}
					
					if(!(WriteFile(gdata->hCom, (char *)&changeColor, sizeof(changeColor),&nbw,NULL)))
						SetLog(L"Unable to to write overcomm SEND NEW COLOR");

					break;
				case IDC_TESTTYPE:
					strcpy_s(msg2.myString, "Test 2 Dialog");
					msg2.hdr.message = (char) 0x057;
					msg2.hdr.size = sizeof(msg2);

					if(!(WriteFile(gdata->hCom, (char *)&msg2, sizeof(msg2),&nbw,NULL)))
						SetLog(L"Unable to to write overcomm SEND NEW TEST TYPE");

					break;
				case IDM_EXIT:
					DestroyWindow(hDlg);
					break;
				case IDC_SENDPACKET:
					SendPacket();
				default:
					return DefWindowProc(hDlg, message, wParam, lParam);
			}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hDlg, &ps);
		break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == (GetDlgItem(hDlg, IDC_STATIC)))
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor((HDC)wParam, RGB(255,0,255));

			return (BOOL)GetSysColorBrush(COLOR_MENU);
		}

		if ((HWND)lParam == (GetDlgItem(hDlg, IDC_SENDMESSAGE)))
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor((HDC)wParam, RGB(255,0,0));

			return (BOOL)GetSysColorBrush(COLOR_MENU);
		}

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

//////////////////////////////////////////////////////////////////
//	SendPacket - sends specific packet information to perform a 
//			specific task
//
//////////////////////////////////////////////////////////////////
void SendPacket()
{
	GLOBAL_DATA *gdata = GetGlobalData();
	
	DWORD nbw;
	DISPLAY_MESSAGE_1 msg1;
	strcpy_s(msg1.myTitle, "Connection Test");
	msg1.hdr.message = (char) 0x057;
	msg1.hdr.size = sizeof(msg1);

	// Send the packet
	if(WriteFile(gdata->hCom, (char *)&msg1, sizeof(msg1),&nbw,NULL))
	{
		SetLog(L"Packet Sent.");

		// Start listening thread to recieve data
		SendMessage(gdata->progressSPA,PBM_SETRANGE,0,MAKELPARAM(0,3));
		SendMessage(gdata->progressSPA,PBM_SETPOS,1,0);

		// Start the listening thread
		if (CreateThread(NULL,0,ListeningThread,gdata,NULL,NULL) == NULL)
		 {
			SetLog(L"Listening thread unable to start.");
		 }
	 }
	 else
	 {
		SetLog(L"Packet NOT Sent.");
	 }
}

///////////////////////////////////////////////////////////////////////////////////
// SetLog - Prints a specific message to the LogList to view in the dialog
//		window
//
//		param - string to be output to dialog window
//
///////////////////////////////////////////////////////////////////////////////////
void SetLog(LPCTSTR outputString)
{
	GLOBAL_DATA *gdata = GetGlobalData();

	SendMessage(gdata->hList,LB_INSERTSTRING, 0,(LPARAM)(LPCTSTR)outputString);
}

DWORD WINAPI ListeningThread(LPVOID lpParameter)
{
	GLOBAL_DATA *gdata = GetGlobalData();
	// set some type of warning of serial comm activity
	HANDLE eventStatus = CreateEvent(NULL,true,false,NULL);
	char buffer[248];
	bool readComm = true;
	DWORD nbr,nbtr=1;
	bool _continue = true;
	int count = 0;
	int result;
	HEADER *hdr;

	SetLog(L"Listening Thread Entry.");
	       
	   if (gdata->hCom == INVALID_HANDLE_VALUE)
       {
          SetLog(L"Failure: Unable to open port");  
			int error = GetLastError();
       }
       else
       {

		if(!(SetCommMask(gdata->hCom, EV_RXCHAR)))
		{
			int error = GetLastError();
			printf("Cannot recieve serial com. failed with error %d" + error);
			//SendMessage(gdata->hList,LB_INSERTSTRING, 0,(LPARAM)SetLog6);
		}

		// set the memory in the buffer to be 0
		memset(buffer, 0, 248);

		while(readComm)
		{
			// Waits for interrupt with serial data to recieve
			WaitCommEvent(gdata->hCom,(LPDWORD)eventStatus, 0);
			
			SetCommMask(gdata->hCom, EV_RXCHAR);

			SetLog(L"Inside readComm while loop waiting to recieve...");

			if( EV_RXCHAR )
			{
				SetLog(L"Recieving Comm over port.");

				// Only read the simple first header then break out of listening thread
				if(count < 1)
					result = ReadFile(gdata->hCom,buffer,sizeof(HEADER),&nbr,NULL);
				else
					return 0;

				if (nbr == sizeof(HEADER))
				{
					hdr = (HEADER *)buffer;
					result = ReadFile(gdata->hCom,buffer+sizeof(HEADER),hdr->size - sizeof(HEADER),&nbr,NULL);
				}

				AssignValuesSPA((char *)buffer);			

				count++;
				
			}		
		}
	}

	::ExitThread(0);
	CloseHandle(gdata->hCom);

	return 0;
}

void AssignValuesSPA( char *buffer)
{
	GLOBAL_DATA *gdata = GetGlobalData();
	HEADER *hdr;
	DISPLAY_MESSAGE_1 *msg1;
	bool result = false;

	hdr = (HEADER *)buffer;

	switch( hdr->message )
	{
		case ENABLE_LAUNCH:
			
			break;
	    case PASS_CONNECTION_TEST:
			// Set all test data to be CONNECT test complete
			msg1 = (DISPLAY_MESSAGE_1 *)buffer;

			SendMessage(gdata->progressSPA,PBM_SETPOS,3,0);
			ShowWindow(gdata->sendNewButton,SW_SHOW);
			ShowWindow(gdata->changeColorButton,SW_SHOW);
			ShowWindow(gdata->newHeightButton,SW_SHOW);
			ShowWindow(gdata->testTypeButton,SW_SHOW);

			break;
		case HEADER_TEST1:
			SetLog(_T("Setting new window header string"));
			SetWindowText(gdata->hSmallPacket,(LPCWSTR)HEADER_TEST1STR);
			break;
		case BLACKOUT:
			SetLog(L"BLACKOUT MODE");
			//PostMessage

			break;
		default:
			SetLog(_T("Cannot find value to assign!"));
			break;
	}
}

void InitCommSPA()
{
	GLOBAL_DATA *gdata = GetGlobalData();

	DCB dcb;

	gdata->hCom = CreateFile(L"\\\\.\\COM26",  
                           GENERIC_READ | GENERIC_WRITE, 
                           0, 
                           0, 
                           OPEN_EXISTING,
                           NULL,
                           0);
      
	if (gdata->hCom == INVALID_HANDLE_VALUE)
    {
		printf("INVALID HANDLE VALUE");
    }
    else
    {
      //  Initialize the DCB structure.
      SecureZeroMemory(&dcb, sizeof(DCB));
      dcb.DCBlength = sizeof(DCB);

      dcb.BaudRate = CBR_9600;      //  baud rate
      dcb.ByteSize = 8;             //  data size, xmit and rcv
      dcb.Parity   = NOPARITY;      //  parity bit
      dcb.StopBits = ONESTOPBIT;    //  stop bit

      // Set new state.
      if (!SetCommState(gdata->hCom, &dcb))
      {
		  printf("Unable to set Comm state");
      }

      cto.ReadIntervalTimeout = 0;
      cto.ReadTotalTimeoutMultiplier = 0;
      cto.ReadTotalTimeoutConstant = 0;
      cto.WriteTotalTimeoutMultiplier = 0;
      cto.WriteTotalTimeoutConstant = 0;

      if(SetCommTimeouts(gdata->hCom,&cto) == 0)
      {
		  printf("Unable to set Comm timeouts");
      }
	}
}
