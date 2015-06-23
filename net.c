#include "net.h"
//接收线程 
DWORD WINAPI threadRead(LPVOID pM)
{
	int count,time;
	while(1){
		count = recv(sockChess,recvBuffer,BUFFSIZE,0);
		if(count <= 0)
		{
			shutdown(sockChess,SD_BOTH);
			closesocket(sockChess);
			time = 0;
			while(connect(sockChess,(SOCKADDR*)&addrChess,sizeof(SOCKADDR))!=0)
			{
				Sleep(100);
				if(time>=RECONNECTTIME)
				{
					netflag = OFF;
					MessageBox(NULL, "网络连接中断！","错误!",MB_ICONEXCLAMATION|MB_OK);
					PostMessage(hwnd,WM_CLOSE,0,0);
				}
				time++;
			}
		}
		else
		{
			switch(recvBuffer[TYPE]){
				case 0x00: {
					self = recvBuffer[RB];
					netflag = recvBuffer[NET];
					if(netflag != ON_RB)
					{
						initMove();
						gameflag = BEGIN;
						readyflag = OFF;
					}
					doGame(0,0,self);
					break;
				}
				case 0x01: {
					doGame(recvBuffer[J],recvBuffer[I],recvBuffer[RB]);
					break;
				}
				case 0x02: {
					switch(recvBuffer[ACT]){
						case UNDO_ASK: {
							tmpflag = gameflag; 
							gameflag = UNDO;
							if(MessageBox(hwnd,"对方请求悔棋！\n是否同意？","提示！", MB_ICONASTERISK|MB_YESNO)==IDYES)
							{
								sendBuffer[TYPE] = 0x02;
								sendBuffer[RB] = self;
								sendBuffer[ACT] = UNDO_YES;
								backGame(recvBuffer[RB]);
								send(sockChess,sendBuffer,BUFFSIZE,0);
							} 
							else 
							{
								sendBuffer[TYPE] = 0x02;
								sendBuffer[RB] = self;
								sendBuffer[ACT] = UNDO_NO;
								send(sockChess,sendBuffer,BUFFSIZE,0);
							} 
							gameflag = tmpflag;
							break;
						}
						case TIE_ASK:{
							tmpflag = gameflag;
							gameflag = TIE;
							if(MessageBox(hwnd,"对方请求和局！\n是否同意？","提示！", MB_ICONASTERISK|MB_YESNO)==IDYES)
							{
								sendBuffer[TYPE] = 0x02;
								sendBuffer[RB] = self;
								sendBuffer[ACT] = TIE_YES;
								send(sockChess,sendBuffer,BUFFSIZE,0);
								gameflag = RB_TIE;
							} 
							else 
							{
								sendBuffer[TYPE] = 0x02;
								sendBuffer[RB] = self;
								sendBuffer[ACT] = TIE_NO;
								send(sockChess,sendBuffer,BUFFSIZE,0);
								gameflag = tmpflag;
							}
							break;
						}
						case GIVEUP:{
							initMove();
							if(self==R)
								gameflag = R_WIN;
							else
								gameflag = B_WIN;
							break;
						}
						case UNDO_YES:{
							MessageBox(hwnd,"对方同意悔棋！","提示！", MB_ICONASTERISK|MB_OK);
							backGame(self);
							gameflag = tmpflag;
							break;
						}
						case UNDO_NO:{
							MessageBox(hwnd,"对方不同意悔棋！","提示！", MB_ICONASTERISK|MB_OK);
							gameflag = tmpflag;
							break;
						}
						case TIE_YES:{
							MessageBox(hwnd,"对方同意和局！","提示！", MB_ICONASTERISK|MB_OK);
							gameflag = RB_TIE;
							break;
						}
						case TIE_NO:{
							MessageBox(hwnd,"对方不同意和局！","提示！", MB_ICONASTERISK|MB_OK);
							gameflag = tmpflag;
							break;
						}
					}
					break;
				}
			}
			InvalidateRgn(hwnd,NULL,TRUE);
			UpdateWindow(hwnd);
		}
	}	
}
//初始化网络资源 
BOOL initGameNet()
{
	WORD wVersionRequseted;    //版本请求
	WSADATA wsaData;           //数据返回
	int err;                   //加载版本错误信息

	wVersionRequseted=MAKEWORD(2,2);
	err=WSAStartup(wVersionRequseted,&wsaData);
    if(err!=0)
	{
		MessageBox(hwnd, "加载WinSock库失败","Error!",MB_ICONEXCLAMATION|MB_OK);
		return FALSE;
	}
	if(LOBYTE(wVersionRequseted)!=2||HIBYTE(wVersionRequseted)!=2)
	{
		MessageBox(hwnd, "WinSock库版本错误","Error!",MB_ICONEXCLAMATION|MB_OK);
		WSACleanup();
		return FALSE;
	}
//	ip="121.40.164.151";  	//服务器IP 
	ip="127.0.0.1";
	port=3000;				//端口 
	memset(sendBuffer,0,BUFFSIZE);
	memset(recvBuffer,0,BUFFSIZE);
	
	self = 0;
	netflag = 0;
	return connectServer();
}
//连接服务器 
BOOL connectServer()
{
	
	sockChess = socket(AF_INET,SOCK_STREAM,0);
	if(sockChess == INVALID_SOCKET)return FALSE;
	addrChess.sin_addr.S_un.S_addr=inet_addr(ip);
	addrChess.sin_family=AF_INET;
	addrChess.sin_port=htons(port);
	
	int time = 0;
	while(connect(sockChess,(SOCKADDR*)&addrChess,sizeof(SOCKADDR))!=0)
	{
		time++;
		if(time>RECONNECTTIME){
			closesocket(sockChess);
			return FALSE;
		}
		Sleep(100);
	}
	hThread = CreateThread(NULL, 0, threadRead, NULL, 0, NULL);
	return TRUE;
}
//用户数据处理 
void sendData(int x,int y)
{
	if((520<=x&&x<=580)&&(470<=y&&y<=500))
	{
		if(undoflag == 0)
		{
			return ;
		}
		if(undotimes <= 0)
		{
			return ;
		}
		//MessageBox(hwnd, "悔棋","提示",MB_OK);
		sendBuffer[TYPE] = 0x02;
		sendBuffer[RB] = self;
		sendBuffer[ACT] = UNDO_ASK;
		send(sockChess,sendBuffer,BUFFSIZE,0);
		tmpflag = gameflag;
		gameflag = UNDO;
		undotimes--;
	}
	else if((600<=x&&x<=660)&&(470<=y&&y<=500))
	{
		if(tieflag ==0)
		{
			return ;
		}
		if(tietimes <=0)
		{
			return ;
		}
		//MessageBox(hwnd, "求和","提示",MB_OK);
		sendBuffer[TYPE] = 0x02;
		sendBuffer[RB] = self;
		sendBuffer[ACT] = TIE_ASK;
		send(sockChess,sendBuffer,BUFFSIZE,0);
		tmpflag = gameflag;
		gameflag = TIE;
		tietimes--;
	}
	else if((680<=x&&x<=740)&&(470<=y&&y<=500))
	{
		if(giveupflag == 0)
		{
			return ;
		}
		if(MessageBox(hwnd, "确认认输吗？","提示",MB_ICONEXCLAMATION|MB_YESNO)==IDNO)
			return ;
		sendBuffer[TYPE] = 0x02;
		sendBuffer[RB] = self;
		sendBuffer[ACT] = GIVEUP;
		send(sockChess,sendBuffer,BUFFSIZE,0);
		initMove(); 
		if(self == R)
			gameflag = B_WIN;
		else 
			gameflag = R_WIN;
	}
	else 
	{
		char j = (x+20)/50-1;
		char i = (y+20)/50-1;
		sendBuffer[TYPE] = 0x01;
		sendBuffer[RB] = self;
		sendBuffer[J] = j;
		sendBuffer[I] = i;
		send(sockChess,sendBuffer,BUFFSIZE,0);
		doGame(j,i,self);
	}
}
//释放网络资源 
void closeNet()
{
	TerminateThread(hThread,0);
	CloseHandle(hThread);
	shutdown(sockChess,SD_BOTH);
	closesocket(sockChess);
}
