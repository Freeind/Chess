#include<windows.h>
#include<stdio.h>

#define R 1 	//红方 
#define B 2 	//黑方 

#define BUFFSIZE 10

#define OFF 0
#define ON_R 1
#define ON_B 2
#define ON_RB 3

#define TYPE 0
#define RB 1
#define NET 2
#define J 3
#define I 4
#define ACT 5


SOCKET sockServer,sockClient0,sockClient1;
SOCKADDR_IN addrServer,addrClient0,addrClient1;
int length0,length1;
int status = OFF;
void sendStatus()
{
	char recvBuffer[BUFFSIZE];
	recvBuffer[TYPE] = 0x00;
	recvBuffer[NET] = status;
	recvBuffer[RB] = R;
	send(sockClient0,recvBuffer,BUFFSIZE,0);
	recvBuffer[RB] = B;
	send(sockClient1,recvBuffer,BUFFSIZE,0);
}

DWORD WINAPI threadClient0(LPVOID pM)
{
	int count;
	char recvBuffer[BUFFSIZE];
	
	printf("Waiting Client0 connect...\n");
	sockClient0 = accept(sockServer,(SOCKADDR*)&addrClient0,&length0);
	if(sockClient0 == INVALID_SOCKET)
	{
		printf("Connect Client0 failed errno %d\n",WSAGetLastError());
	}
	else
	{
		status = status^R;
		printf("[%s]Client0 connected!\n",inet_ntoa(addrClient0.sin_addr));
		sendStatus();
	}
	
	while(1){
		memset(recvBuffer,0,BUFFSIZE);
		count = recv(sockClient0,recvBuffer,BUFFSIZE,0);
		if(count == -1)
		{
			status = status^R;
			shutdown(sockClient0,SD_BOTH);
			closesocket(sockClient0);
			sockClient0 = 0;
			sendStatus();
			printf("[%s]Client0 disconnect!\n",inet_ntoa(addrClient0.sin_addr));
			sockClient0 = accept(sockServer,(SOCKADDR*)&addrClient0,&length0);
			if(sockClient0 == INVALID_SOCKET)
			{
				printf("Connect Client1 failed errno %d",WSAGetLastError());
			}
			else
			{
				status = status^R;
				printf("[%s]Client0 connected!\n",inet_ntoa(addrClient0.sin_addr));
				sendStatus();
			}
			continue;
		}
		printf("[%s]TYPE:%u RB:%2d j:%2d i:%2d act:%2d\n",
			inet_ntoa(addrClient0.sin_addr),
			recvBuffer[TYPE],
			recvBuffer[RB],
			recvBuffer[J],
			recvBuffer[I],
			recvBuffer[ACT]);
		count = send(sockClient1,recvBuffer,BUFFSIZE,0);
	}
}

DWORD WINAPI threadClient1(LPVOID pM)
{
	int count;
	char recvBuffer[BUFFSIZE];
	
	printf("Waiting Client1 connect...\n");	
	sockClient1 = accept(sockServer,(SOCKADDR*)&addrClient1,&length1);
	if(sockClient1 == INVALID_SOCKET)
	{
		printf("Connect Client1 failed errno %d",WSAGetLastError());
	}
	else
	{
		status = status^B;
		printf("[%s]Client1 connected!\n",inet_ntoa(addrClient1.sin_addr));
		sendStatus();
	}
	
	while(1){
		memset(recvBuffer,0,BUFFSIZE);
		count = recv(sockClient1,recvBuffer,BUFFSIZE,0);
		if(count == -1)
		{
			status = status^B;
			shutdown(sockClient1,SD_BOTH);
			closesocket(sockClient1);
			sockClient1 = 0;
			sendStatus();
			printf("[%s]Client1 disconnect!\n",inet_ntoa(addrClient1.sin_addr));
			sockClient1 = accept(sockServer,(SOCKADDR*)&addrClient1,&length1);
			if(sockClient1 == INVALID_SOCKET)
			{
				printf("Connect Client1 failed errno %d",WSAGetLastError());
				break;
			}
			else
			{
				status = status^B;
				printf("[%s]Client1 connected!\n",inet_ntoa(addrClient1.sin_addr));
				sendStatus();
			}
			continue;
		}
		printf("[%s]TYPE:%u RB:%2d j:%2d i:%2d act:%2d\n",
			inet_ntoa(addrClient1.sin_addr),
			recvBuffer[TYPE],
			recvBuffer[RB],
			recvBuffer[J],
			recvBuffer[I],
			recvBuffer[ACT]);
		count = send(sockClient0,recvBuffer,BUFFSIZE,0);
	}
}

int main()
{
	WORD wVersionRequseted;    //版本请求
	WSADATA wsaData;           //数据返回
	int err;                   //加载版本错误信息
	
	char recvBuffer[BUFFSIZE];

	wVersionRequseted=MAKEWORD(1,1);
	err=WSAStartup(wVersionRequseted,&wsaData);
    if(err!=0)
	{
		printf("加载WinSock库失败!");
		return -1;
	}
	if(LOBYTE(wVersionRequseted)!=1||HIBYTE(wVersionRequseted)!=1)
	{
		printf("WinSock库版本错误!");
		WSACleanup();
		return -1;
	}
	sockServer=socket(AF_INET,SOCK_STREAM,0);
	memset(&addrServer,0,sizeof(SOCKADDR_IN));
	memset(&addrClient0,0,sizeof(SOCKADDR_IN));
	memset(&addrClient1,0,sizeof(SOCKADDR_IN));
	addrServer.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(3000);
	length0=sizeof(SOCKADDR);
	length1=sizeof(SOCKADDR);
	
	bind(sockServer,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
	listen(sockServer,5);

	HANDLE hThread0 = CreateThread(NULL, 0, threadClient0, NULL, 0, NULL);
	HANDLE hThread1 = CreateThread(NULL, 0, threadClient1, NULL, 0, NULL);
	WaitForSingleObject(hThread0, INFINITE);
	WaitForSingleObject(hThread1, INFINITE);
	closesocket(sockServer);
	WSACleanup();
	return 0;
}
