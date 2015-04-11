#ifndef _CHESS_H_
#define _CHESS_H_
#include <tchar.h>
#include <windows.h>

#define MAX_X 8	//���̿�� 
#define MAX_Y 9//���̳��� 

#define R 1 	//�췽 
#define B 2 	//�ڷ� 

#define K	0	//������ 
#define R_J 1	//��˧ 
#define R_S 2	//���� 
#define R_X 3	//���� 
#define R_M 4	//���� 
#define R_C 5	//�쳵 
#define R_P 6	//���� 
#define R_B 7	//��� 
#define B_S 9	//��ʿ 
#define B_J 8	//�ڽ� 
#define B_X 10	//���� 
#define B_M 11	//���� 
#define B_C 12	//�ڳ� 
#define B_P 13	//���� 
#define B_B 14	//�ڱ� 

#define BEGIN 0		//�ȴ�˫������ 
#define READY 1		//�ȴ�˫��׼�� 
#define R_SET 2		//�췽�غ� 
#define B_SET 3		//�ڷ��غ�  
#define R_WIN 4		//�췽ʤ�� 
#define B_WIN 5		//�ڷ�ʤ�� 
#define RB_TIE 6 	//�;� 
#define UNDO 7		//���� 
#define TIE 8		//�;� 
#define GAMEFLAG 9

#define R_CHECK 1	//�췽���� 
#define B_CHECK 2	//�ڷ����� 
#define CHECKFLAG 3

#define OFF 0		//˫���������� 
#define ON_R 1		//�췽���� 
#define ON_B 2		//�ڷ����� 
#define ON_RB 3		//˫�������� 
#define NETFLAG 4

#define UNDO_ASK 0	//������� 
#define UNDO_YES 1	//ͬ����� 
#define UNDO_NO 2 	//��ͬ����� 
#define TIE_ASK 3	//����;� 
#define TIE_YES 4	//ͬ��;� 
#define TIE_NO 5	//��ͬ��;� 
#define GIVEUP 6	//���� 

extern HWND hwnd;	//���ھ�� 
HINSTANCE  hChess;	//�������� 
HBITMAP  hQP;		//���̱���ͼƬ 
HBITMAP  hRC[16];	//����ͼƬ 
HBITMAP  hUI[10];	//����ͼƬ 
HDC hMemdc; 		//��ͼ�� 

int self;		//�ҷ���� 
int gameflag;	//��Ϸ״̬��� 
int tmpflag;	//��ʱ��Ϸ״̬��� 
int checkflag;	//������� 
int netflag;	//�������ӱ�� 
int readyflag;	//׼����� 
int undoflag;	//������ʾ��� 
int tieflag;	//�����ʾ��� 
int giveupflag;	//������ʾ��� 
int backflag;	//������ 
int undotimes;	//�ɻ������ 
int tietimes;	//����ʹ��� 

BOOL initGameRC(HINSTANCE hInstance);

void closeRC(); 

void drawChessman(HDC hdc);

void drawStatus(HDC hdc);

void drawPos(HDC hdc,int X,int Y);

void doGame(int j,int i,int rb);

void backGame(int rb);

#endif
