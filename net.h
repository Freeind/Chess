#ifndef _NET_H_
#define _NET_H_
#include "chess.h"

#define BUFFSIZE 10

#define RECONNECTTIME 0

#define TYPE 0
#define RB 1
#define NET 2
#define J 3
#define I 4
#define ACT 5

extern HWND hwnd;
char *ip;
int port;
SOCKET sockChess;
SOCKADDR_IN addrChess;
HANDLE hThread;

char recvBuffer[BUFFSIZE];
char sendBuffer[BUFFSIZE];

BOOL initGameNet();

void sendData(int x,int y);

void closeNet(); 

#endif
