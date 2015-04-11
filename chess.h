#ifndef _CHESS_H_
#define _CHESS_H_
#include <tchar.h>
#include <windows.h>

#define MAX_X 8	//棋盘宽度 
#define MAX_Y 9//棋盘长度 

#define R 1 	//红方 
#define B 2 	//黑方 

#define K	0	//无棋子 
#define R_J 1	//红帅 
#define R_S 2	//红仕 
#define R_X 3	//红相 
#define R_M 4	//红马 
#define R_C 5	//红车 
#define R_P 6	//红炮 
#define R_B 7	//红兵 
#define B_S 9	//黑士 
#define B_J 8	//黑将 
#define B_X 10	//黑象 
#define B_M 11	//黑马 
#define B_C 12	//黑车 
#define B_P 13	//黑炮 
#define B_B 14	//黑兵 

#define BEGIN 0		//等待双方加入 
#define READY 1		//等待双方准备 
#define R_SET 2		//红方回合 
#define B_SET 3		//黑方回合  
#define R_WIN 4		//红方胜利 
#define B_WIN 5		//黑方胜利 
#define RB_TIE 6 	//和局 
#define UNDO 7		//悔棋 
#define TIE 8		//和局 
#define GAMEFLAG 9

#define R_CHECK 1	//红方将军 
#define B_CHECK 2	//黑方将军 
#define CHECKFLAG 3

#define OFF 0		//双方都不在线 
#define ON_R 1		//红方在线 
#define ON_B 2		//黑方在线 
#define ON_RB 3		//双方都在线 
#define NETFLAG 4

#define UNDO_ASK 0	//请求悔棋 
#define UNDO_YES 1	//同意悔棋 
#define UNDO_NO 2 	//不同意悔棋 
#define TIE_ASK 3	//请求和局 
#define TIE_YES 4	//同意和局 
#define TIE_NO 5	//不同意和局 
#define GIVEUP 6	//认输 

extern HWND hwnd;	//窗口句柄 
HINSTANCE  hChess;	//主程序句柄 
HBITMAP  hQP;		//棋盘背景图片 
HBITMAP  hRC[16];	//棋子图片 
HBITMAP  hUI[10];	//界面图片 
HDC hMemdc; 		//绘图用 

int self;		//我方标记 
int gameflag;	//游戏状态标记 
int tmpflag;	//临时游戏状态标记 
int checkflag;	//将军标记 
int netflag;	//网络连接标记 
int readyflag;	//准备标记 
int undoflag;	//悔棋显示标记 
int tieflag;	//求和显示标记 
int giveupflag;	//认输显示标记 
int backflag;	//悔棋标记 
int undotimes;	//可悔棋次数 
int tietimes;	//可求和次数 

BOOL initGameRC(HINSTANCE hInstance);

void closeRC(); 

void drawChessman(HDC hdc);

void drawStatus(HDC hdc);

void drawPos(HDC hdc,int X,int Y);

void doGame(int j,int i,int rb);

void backGame(int rb);

#endif
