/*******************************************************************************
	Name: Chess
	Copyright: indy
	Author: indy
	Date: 12/03/15 19:30
	Description: A Chinese chess game.
*******************************************************************************/
#include "chess.h"
#include "net.h"
HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
HDC hdc;
PAINTSTRUCT ps;
POINTS pos;

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_LBUTTONDOWN: {
			TCHAR cpos[10];
			pos = MAKEPOINTS(lParam);		//获取鼠标位置 
			//doGame(pos.x,pos.y);		//将鼠标位置转换成棋盘坐标 
			sendData(pos.x,pos.y);
			InvalidateRgn(hwnd,NULL,TRUE);
			UpdateWindow(hwnd);
			break;
		}
		case WM_PAINT: {
			hdc =BeginPaint(hwnd,&ps);
			drawChessman(hdc);
			drawStatus(hdc);
			//drawPos(hdc,pos.x,pos.y);
			EndPaint(hwnd,&ps);
			break;
		}
		/* trap the WM_CLOSE (clicking X) message, and actually tell the window to close */
		case WM_CLOSE: {
			closeRC();
			closeNet();
			DestroyWindow(hwnd);
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
//	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG Msg; /* A temporary location for all messages */
	
	if(!initGameRC(hInstance)){
		MessageBox(hwnd,"游戏初始化失败!","错误!",MB_ICONEXCLAMATION|MB_OK);
		return Msg.wParam;
	}
	
	if(!initGameNet()){
		MessageBox(hwnd,"网络连接失败!","错误!",MB_ICONEXCLAMATION|MB_OK);
		return Msg.wParam;
	}
	
	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.hbrBackground = CreatePatternBrush(hQP);
	wc.lpszClassName = "WindowClass"; 
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(hwnd, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","中国象棋    -indy",WS_VISIBLE|WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX&~WS_THICKFRAME,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		800, /* width */
		600, /* height */
		NULL,NULL,hInstance,NULL);
		
	/*CreateWindow("edit",NULL,WS_VISIBLE|WS_CHILD|ES_WANTRETURN|ES_NUMBER,
		530,
		140, 
		200, 
		20,
		hwnd,NULL, hInstance,NULL);*/

	if(hwnd == NULL) {
		MessageBox(hwnd, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produre unreasonably CPU usage
	*/
	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate keycodes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
	}
	return Msg.wParam;
}
