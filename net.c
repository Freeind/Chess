#include "net.h"
DWORD WINAPI threadRead(LPVOID pM)
{
	int count,time;
	while(1){
		count = recv(sockChess,recvBuffer,BUFFSIZE,0);
		if(count == -1)
		{
			shutdown(sockChess,SD_BOTH);
			closesocket(sockChess);
			time = 0;
			while(connect(sockChess,(SOCKADDR*)&addrChess,sizeof(SOCKADDR))!=0)
			{
				Sleep(100);
				time++;
				if(time>=RECONNECTTIME)
				{
					netflag = OFF;
					MessageBox(NULL, "���������жϣ�","����!",MB_ICONEXCLAMATION|MB_OK);
					PostMessage(hwnd,WM_CLOSE,0,0);
				}
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
						case UNDO: {
							tmpflag = gameflag; 
							gameflag = UNDO;
							if(MessageBox(hwnd,"�Է�������壡\n�Ƿ�ͬ�⣿","��ʾ��", MB_ICONASTERISK|MB_YESNO)==IDYES)
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
						case TIE:{
							tmpflag = gameflag;
							gameflag = TIE;
							if(MessageBox(hwnd,"�Է�����;֣�\n�Ƿ�ͬ�⣿","��ʾ��", MB_ICONASTERISK|MB_YESNO)==IDYES)
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
							MessageBox(hwnd,"�Է�ͬ����壡","��ʾ��", MB_ICONASTERISK|MB_OK);
							backGame(self);
							gameflag = tmpflag;
							break;
						}
						case UNDO_NO:{
							MessageBox(hwnd,"�Է���ͬ����壡","��ʾ��", MB_ICONASTERISK|MB_OK);
							gameflag = tmpflag;
							break;
						}
						case TIE_YES:{
							MessageBox(hwnd,"�Է�ͬ��;֣�","��ʾ��", MB_ICONASTERISK|MB_OK);
							gameflag = RB_TIE;
							break;
						}
						case TIE_NO:{
							MessageBox(hwnd,"�Է���ͬ��;֣�","��ʾ��", MB_ICONASTERISK|MB_OK);
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
BOOL initGameNet()
{
	WORD wVersionRequseted;    //�汾����
	WSADATA wsaData;           //���ݷ���
	int err;                   //���ذ汾������Ϣ

	wVersionRequseted=MAKEWORD(2,2);
	err=WSAStartup(wVersionRequseted,&wsaData);
    if(err!=0)
	{
		MessageBox(hwnd, "����WinSock��ʧ��","Error!",MB_ICONEXCLAMATION|MB_OK);
		return FALSE;
	}
	if(LOBYTE(wVersionRequseted)!=2||HIBYTE(wVersionRequseted)!=2)
	{
		MessageBox(hwnd, "WinSock��汾����","Error!",MB_ICONEXCLAMATION|MB_OK);
		WSACleanup();
		return FALSE;
	}
	ip="121.40.164.151";  	//������IP 
	port=3000;				//�˿� 
	memset(sendBuffer,0,BUFFSIZE);
	memset(recvBuffer,0,BUFFSIZE);
	return connectServer();
}
BOOL creatSocket()
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
	return TRUE;
}

BOOL connectServer()
{
	self = 0;
	netflag = 0;
	if(!creatSocket())
	{
		return FALSE;
	}
	hThread = CreateThread(NULL, 0, threadRead, NULL, 0, NULL);
	return TRUE;
}
void sendData(int x,int y)
{
	if((520<=x&&x<=580)&&(470<=y&&y<=500))
	{
		//if(self==R&&gameflag!=R_SET||self==B&&gameflag!=B_SET)
		if(undoflag == 0)
		{
			//MessageBox(hwnd, "���ڲ��ܻ��壡","��ʾ",MB_OK);
			return ;
		}
		if(undotimes <= 0)
		{
			//MessageBox(hwnd, "���ܻ��壡","��ʾ",MB_OK);
			return ;
		}
		//MessageBox(hwnd, "����","��ʾ",MB_OK);
		sendBuffer[TYPE] = 0x02;
		sendBuffer[RB] = self;
		sendBuffer[ACT] = UNDO;
		send(sockChess,sendBuffer,BUFFSIZE,0);
		tmpflag = gameflag;
		gameflag = UNDO;
		undotimes--;
	}
	else if((600<=x&&x<=660)&&(470<=y&&y<=500))
	{
		//if(gameflag!=R_SET&&gameflag!=B_SET)
		if(tieflag ==0)
		{
			//MessageBox(hwnd, "��Ϸ��û��ʼ��","��ʾ",MB_OK);
			return ;
		}
		if(tietimes <=0)
		{
			return ;
		}
		//MessageBox(hwnd, "���","��ʾ",MB_OK);
		sendBuffer[TYPE] = 0x02;
		sendBuffer[RB] = self;
		sendBuffer[ACT] = TIE;
		send(sockChess,sendBuffer,BUFFSIZE,0);
		tmpflag = gameflag;
		gameflag = TIE;
		tietimes--;
	}
	else if((680<=x&&x<=740)&&(470<=y&&y<=500))
	{
		//if(gameflag!=R_SET&&gameflag!=B_SET)
		if(giveupflag == 0)
		{
			//MessageBox(hwnd, "��Ϸ��û��ʼ��","��ʾ",MB_OK);
			return ;
		}
		if(MessageBox(hwnd, "ȷ��������","��ʾ",MB_ICONEXCLAMATION|MB_YESNO)==IDNO)
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
		int j = (x+20)/50-1;
		int i = (y+20)/50-1;
		sendBuffer[TYPE] = 0x01;
		sendBuffer[RB] = self;
		sendBuffer[J] = j;
		sendBuffer[I] = i;
		send(sockChess,sendBuffer,BUFFSIZE,0);
		doGame(j,i,self);
	}
}
void closeNet()
{
	TerminateThread(hThread,0);
	CloseHandle(hThread);
	closesocket(sockChess);
}