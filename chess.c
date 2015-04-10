#include "chess.h"
//#define DEBUG

#ifndef DEBUG
int basemap[10][9] = {
	{ 5, 4, 3, 2, 1, 2, 3, 4, 5},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 6, 0, 0, 0, 0, 0, 6, 0},
	{ 7, 0, 7, 0, 7, 0, 7, 0, 7},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{14, 0,14, 0,14, 0,14, 0,14},
	{ 0,13, 0, 0, 0, 0, 0,13, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{12,11,10, 9, 8, 9,10,11,12}
};
#else
int basemap[10][9] = {
	{ 0, 0, 0, 2, 0, 1, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0,12, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0,0, 12, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 9, 8, 9, 0, 0, 0}
};
#endif

int map[10][9] = {0};
int oldmap_r[10][9] = {0}; 
int oldmap_b[10][9] = {0}; 
int checkmap[10][9] = {0};	
int move[10][9] = {0};
int set_i = -1;
int set_j= -1;
int check_i = -1;
int check_j = -1;

char status[3][20] = {
	{"��"}, 
	{"�췽"},
	{"�ڷ�"}
};
char gamestatus[GAMEFLAG][20] = {
	{"�ȴ�˫������..."},
	{"�ȴ�˫��׼��..."},
	{"�췽�ƶ�����..."},
	{"�ڷ��ƶ�����..."},
	{"�췽ʤ��!"},
	{"�ڷ�ʤ��!"},
	{"�;�!"}, 
	{"���������..."},
	{"����;���..."},
	{"����"}
};
char checkstatus[CHECKFLAG][20] = {
	{"��"}, 
	{"�췽����"},
	{"�ڷ�����"}
};
char netstatus[NETFLAG][20] = {
	{"��"}, 
	{"��"},
	{"��"},
	{"���"}
};

/*HHOOK   hHook;
LRESULT CALLBACK HookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if(nCode==HCBT_ACTIVATE)   
	{   
		SetDlgItemText((HWND)wParam,IDOK,"&���¿�ʼ");
		SetDlgItemText((HWND)wParam,IDCANCEL,"&�˳���Ϸ");
		UnhookWindowsHookEx(hHook);
	}
	return   0;
}*/

//��ʼ����Ϸ��Դ 
BOOL initGameRC(HINSTANCE hInstance)
{
	hChess = hInstance;
	TCHAR pRC[15];
	int i;
	
	hQP = (HBITMAP)LoadImage(hChess,"./rc/qp",IMAGE_BITMAP,790,568,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hQP == NULL) {
		return FALSE;
	}
	hRC[0] = (HBITMAP)LoadImage(hChess,"./rc/sel",IMAGE_BITMAP,50,50,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hRC[0] == NULL) {
		return FALSE;
	}
	hRC[15] = (HBITMAP)LoadImage(hChess,"./rc/move",IMAGE_BITMAP,50,50,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hRC[0] == NULL) {
		return FALSE;
	}
	for(i=1;i<=14;i++)
	{
		wsprintf(pRC,"./rc/%d",i);
		hRC[i] = (HBITMAP)LoadImage(hChess,pRC,IMAGE_BITMAP,50,50,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		if(hRC[i] == NULL) {
			return FALSE;
		}
	}
	
	hUI[0] = (HBITMAP)LoadImage(hChess,"./rc/set",IMAGE_BITMAP,20,80,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[0] == NULL) {
		return FALSE;
	}
	hUI[1] = (HBITMAP)LoadImage(hChess,"./rc/icon/R.bmp",IMAGE_BITMAP,80,80,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[1] == NULL) {
		return FALSE;
	}
	hUI[2] = (HBITMAP)LoadImage(hChess,"./rc/icon/B.bmp",IMAGE_BITMAP,80,80,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[2] == NULL) {
		return FALSE;
	}
	hUI[3] = (HBITMAP)LoadImage(hChess,"./rc/our",IMAGE_BITMAP,80,40,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[3] == NULL) {
		return FALSE;
	}
	hUI[4] = (HBITMAP)LoadImage(hChess,"./rc/other",IMAGE_BITMAP,80,40,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[4] == NULL) {
		return FALSE;
	}
	hUI[5] = (HBITMAP)LoadImage(hChess,"./rc/ready",IMAGE_BITMAP,80,40,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[5] == NULL) {
		return FALSE;
	}
	hUI[6] = (HBITMAP)LoadImage(hChess,"./rc/check",IMAGE_BITMAP,200,100,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[6] == NULL) {
		return FALSE;
	}
	hUI[7] = (HBITMAP)LoadImage(hChess,"./rc/undo",IMAGE_BITMAP,60,30,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[7] == NULL) {
		return FALSE;
	}
	hUI[8] = (HBITMAP)LoadImage(hChess,"./rc/tie",IMAGE_BITMAP,60,30,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[8] == NULL) {
		return FALSE;
	}
	hUI[9] = (HBITMAP)LoadImage(hChess,"./rc/giveup",IMAGE_BITMAP,60,30,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hUI[9] == NULL) {
		return FALSE;
	}
	gameflag = BEGIN;
	return TRUE;
}

void closeRC()
{
	DeleteObject(hQP);
	int i;
	for(i=0;i<=15;i++)
	{
		DeleteObject(hRC[i]);
	}
	for(i=0;i<=5;i++)
	{
		DeleteObject(hUI[i]);
	}
}
//����������תΪ��Ļ���� 
int getX(int j){
	return 30+j*50;
}
int getY(int i){
	return 30+i*50;
}
//�ж�������ɫ 
int isR(int i,int j){
	if(1<=map[i][j]&&map[i][j]<=7)
		return 1;
	else 
		return 0;
} 
int isB(int i,int j){
	if(8<=map[i][j]&&map[i][j]<=14)
		return 1;
	else 
		return 0;
} 
//��ʼ���߷� 
void initMove(){
	set_i = -1;
	set_j = -1;
	memset(move,0,sizeof(move));
}
void initMap(){
	checkflag = 0;
	undoflag = 0;
	tieflag = 0;
	giveupflag = 0;
	undotimes = 0;
	tietimes = 3; 
	memcpy(map,basemap,sizeof(map));
	memcpy(oldmap_r,basemap,sizeof(map));
	memcpy(oldmap_b,basemap,sizeof(map));
}
//�������� 
void saveMap(){
	check_i = set_i;
	check_j = set_j;
	memcpy(checkmap,map,sizeof(map));
}
//�ָ����� 
void loadMap(){
	set_i = check_i;
	set_j = check_j;
	memcpy(map,checkmap,sizeof(map));
}
void setOldMap(int rb)
{
	if(rb == R)
		memcpy(oldmap_r,map,sizeof(map));
	else if(rb ==B)
		memcpy(oldmap_b,map,sizeof(map));
	if(rb==self)
		undotimes = 3;
}
void getOldMap(int rb)
{
	if(rb == R)
		memcpy(map,oldmap_r,sizeof(map));
	else if(rb ==B)
		memcpy(map,oldmap_b,sizeof(map));	
}
void backGame(int rb)
{
	getOldMap(rb);
	initMove();
}
//ѡ������ 
void selCM(int i,int j)
{
	initMove();
	set_i = i;
	set_j = j;
}
//�ƶ����� 
void setCM(int i,int j)
{
	map[i][j] = move[i][j];
	map[set_i][set_j] = 0;
	set_i = -1;
	set_j = -1;
}
//�жϽ��������߷� 
void setMove(int i,int j,int cm,int rb)
{
	int flag,tmp;
	saveMap();
	tmp = move[i][j];
	move[i][j] = cm;
	setCM(i,j);
	if(isCheck(rb)!=0)
	{
		move[i][j] = tmp;
	}
	loadMap();
}
//����˧���߷� 
void moveJ(int i,int j,int rb)
{
	if(rb==R)
	{
		if(((i+1)<=2)&&(!isR(i+1,j)))
			//move[i+1][j] = R_J;
			setMove(i+1,j,R_J,B);
		if(((i-1)>=0)&&(!isR(i-1,j)))
			//move[i-1][j] = R_J;
			setMove(i-1,j,R_J,B);
		if(((j+1)<=5)&&(!isR(i,j+1)))
			//move[i][j+1] = R_J;
			setMove(i,j+1,R_J,B);
		if(((j-1)>=3)&&(!isR(i,j-1)))
			//move[i][j-1] = R_J;
			setMove(i,j-1,R_J,B);
	}
	if(rb==B)
	{
		if(((i+1)<=9)&&(!isB(i+1,j)))
			//move[i+1][j] = B_J;
			setMove(i+1,j,B_J,R);
		if(((i-1)>=7)&&(!isB(i-1,j)))
			//move[i-1][j] = B_J;
			setMove(i-1,j,B_J,R);
		if(((j+1)<=5)&&(!isB(i,j+1)))
			//move[i][j+1] = B_J;
			setMove(i,j+1,B_J,R);
		if(((j-1)>=3)&&(!isB(i,j-1)))
			//move[i][j-1] = B_J;
			setMove(i,j-1,B_J,R);
	}
}
//ʿ���߷� 
void moveS(int i,int j,int rb) 
{
	if(rb==R)
	{
		if(i!=1)
		{
			if(!isR(1,4))
				//move[1][4] = R_S;
				setMove(1,4,R_S,B);
		} 
		else 
		{
			if(!isR(0,3))
				//move[0][3] = R_S;
				setMove(0,3,R_S,B);
			if(!isR(0,5))
				//move[0][5] = R_S;
				setMove(0,5,R_S,B);
			if(!isR(2,3))
				//move[2][3] = R_S;
				setMove(2,3,R_S,B);
			if(!isR(2,5))
				//move[2][5] = R_S;
				setMove(2,5,R_S,B);
		}
	}
	if(rb==B)
	{
		if(i!=8)
		{
			if(!isB(8,4))
				//move[8][4] = B_S;
				setMove(8,4,B_S,R);
		} 
		else 
		{
			if(!isB(7,3))
				//move[7][3] = B_S;
				setMove(7,3,B_S,R);
			if(!isB(7,5))
				//move[7][5] = B_S;
				setMove(7,5,B_S,R);
			if(!isB(9,3))
				//move[9][3] = B_S;
				setMove(9,3,B_S,R);
			if(!isB(9,5))
				//move[9][5] = B_S;
				setMove(9,5,B_S,R);
		}
	}
}
//����߷� 
void moveX(int i,int j,int rb){
	if(rb==R)
	{
		if((i+2)<=4&&(j+2)<=8)
		{
			if(map[i+1][j+1]==0&&!isR(i+2,j+2))
				//move[i+2][j+2] = R_X;
				setMove(i+2,j+2,R_X,B);
		}
		if((i+2)<=4&&(j-2)>=0)
		{
			if(map[i+1][j-1]==0&&!isR(i+2,j-2))
				//move[i+2][j-2] = R_X;
				setMove(i+2,j-2,R_X,B);
		}
		if((i-2)>=0&&(j+2)<=8)
		{
			if(map[i-1][j+1]==0&&!isR(i-2,j+2))
				//move[i-2][j+2] = R_X;
				setMove(i-2,j+2,R_X,B);
		}
		if((i-2)>=0&&(j-2)>=0)
		{
			if(map[i-1][j-1]==0&&!isR(i-2,j-2))
				//move[i-2][j-2] = R_X;
				setMove(i-2,j-2,R_X,B);
		}
	}
	if(rb==B)
	{
		if((i+2)<=9&&(j+2)<=8)
		{
			if(map[i+1][j+1]==0&&!isB(i+2,j+2))
				//move[i+2][j+2] = B_X;
				setMove(i+2,j+2,B_X,R);
		}
		if((i+2)<=9&&(j-2)>=0)
		{
			if(map[i+1][j-1]==0&&!isB(i+2,j-2))
				//move[i+2][j-2] = B_X;
				setMove(i+2,j-2,B_X,R);
		}
		if((i-2)>=5&&(j+2)<=8)
		{
			if(map[i-1][j+1]==0&&!isB(i-2,j+2))
				//move[i-2][j+2] = B_X;
				setMove(i-2,j+2,B_X,R);
		}
		if((i-2)>=5&&(j-2)>=0)
		{
			if(map[i-1][j-1]==0&&!isB(i-2,j-2))
				//move[i-2][j-2] = B_X;
				setMove(i-2,j-2,B_X,R);
		}
	}
} 
//�����߷� 
void moveM(int i,int j,int rb) {
	if(rb==R)
	{
		if(map[(i+1)%10][j]==0)
		{
			if((i+2)<=9&&(j+1)<=8&&!isR(i+2,j+1))
				//move[i+2][j+1] = R_M;
				setMove(i+2,j+1,R_M,B);
			if((i+2)<=9&&(j-1)>=0&&!isR(i+2,j-1))
				//move[i+2][j-1] = R_M;
				setMove(i+2,j-1,R_M,B);
		}
		if(map[(i-1)%10][j]==0)
		{
			if((i-2)>=0&&(j+1)<=8&&!isR(i-2,j+1))
				//move[i-2][j+1] = R_M;
				setMove(i-2,j+1,R_M,B);
			if((i-2)>=0&&(j-1)>=0&&!isR(i-2,j-1))
				//move[i-2][j-1] = R_M;
				setMove(i-2,j-1,R_M,B);
		}
		if(map[i][(j+1)%10]==0)
		{
			if((i+1)<=9&&(j+2)<=8&&!isR(i+1,j+2))
				//move[i+1][j+2] = R_M;
				setMove(i+1,j+2,R_M,B);
			if((i-1)>=0&&(j+2)<=8&&!isR(i-1,j+2))
				//move[i-1][j+2] = R_M;
				setMove(i-1,j+2,R_M,B);
		}
		if(map[i][(j-1)%10]==0)
		{
			if((i+1)<=9&&(j-2)>=0&&!isR(i+1,j-2))
				//move[i+1][j-2] = R_M;
				setMove(i+1,j-2,R_M,B);
			if((i-1)>=0&&(j-2)>=0&&!isR(i-1,j-2))
				//move[i-1][j-2] = R_M;
				setMove(i-1,j-2,R_M,B);
		}
	}
	if(rb==B)
	{
		if(map[(i+1)%10][j]==0)
		{
			if((i+2)<=9&&(j+1)<=8&&!isB(i+2,j+1))
				//move[i+2][j+1] = B_M;
				setMove(i+2,j+1,B_M,R);
			if((i+2)<=9&&(j-1)>=0&&!isB(i+2,j-1))
				//move[i+2][j-1] = B_M;
				setMove(i+2,j-1,B_M,R);
		}
		if(map[(i-1)%10][j]==0)
		{
			if((i-2)>=0&&(j+1)<=8&&!isB(i-2,j+1))
				//move[i-2][j+1] = B_M;
				setMove(i-2,j+1,B_M,R);
			if((i-2)>=0&&(j-1)>=0&&!isB(i-2,j-1))
				//move[i-2][j-1] = B_M;
				setMove(i-2,j-1,B_M,R);
		}
		if(map[i][(j+1)%10]==0)
		{
			if((i+1)<=9&&(j+2)<=8&&!isB(i+1,j+2))
				//move[i+1][j+2] = B_M;
				setMove(i+1,j+2,B_M,R);
			if((i-1)>=0&&(j+2)<=8&&!isB(i-1,j+2))
				//move[i-1][j+2] = B_M;
				setMove(i-1,j+2,B_M,R);
		}
		if(map[i][(j-1)%10]==0)
		{
			if((i+1)<=9&&(j-2)>=0&&!isB(i+1,j-2))
				//move[i+1][j-2] = B_M;
				setMove(i+1,j-2,B_M,R);
			if((i-1)>=0&&(j-2)>=0&&!isB(i-1,j-2))
				//move[i-1][j-2] = B_M;
				setMove(i-1,j-2,B_M,R);
		}
	}
}
//�����߷� 
void moveC(int i,int j,int rb) {
	int c_i,c_j;
	if(rb==R)
	{
		c_i = i+1;
		c_j = j;
		while(c_i<=9&&!isR(c_i,c_j))
		{
			//move[c_i][c_j] = R_C;
			setMove(c_i,c_j,R_C,B);
			if(isB(c_i,c_j))
				break;
			else c_i++;
		}
		c_i = i-1;
		c_j = j;
		while(c_i>=0&&!isR(c_i,c_j))
		{
			//move[c_i][c_j] = R_C;
			setMove(c_i,c_j,R_C,B);
			if(isB(c_i,c_j))
				break;
			else c_i--;
		}
		c_i = i;
		c_j = j+1;
		while(c_j<=8&&!isR(c_i,c_j))
		{
			//move[c_i][c_j] = R_C;
			setMove(c_i,c_j,R_C,B);
			if(isB(c_i,c_j))
				break;
			else c_j++;
		}
		c_i = i;
		c_j = j-1;
		while(c_j>=0&&!isR(c_i,c_j))
		{
			//move[c_i][c_j] = R_C;
			setMove(c_i,c_j,R_C,B);
			if(isB(c_i,c_j))
				break;
			else c_j--;
		}
	}
	if(rb==B)
	{
		c_i = i+1;
		c_j = j;
		while(c_i<=9&&!isB(c_i,c_j))
		{
			//move[c_i][c_j] = B_C;
			setMove(c_i,c_j,B_C,R);
			if(isR(c_i,c_j))
				break;
			else c_i++;
		}
		c_i = i-1;
		c_j = j;
		while(c_i>=0&&!isB(c_i,c_j))
		{
			//move[c_i][c_j] = B_C;
			setMove(c_i,c_j,B_C,R);
			if(isR(c_i,c_j))
				break;
			else c_i--;
		}
		c_i = i;
		c_j = j+1;
		while(c_j<=8&&!isB(c_i,c_j))
		{
			//move[c_i][c_j] = B_C;
			setMove(c_i,c_j,B_C,R);
			if(isR(c_i,c_j))
				break;
			else c_j++;
		}
		c_i = i;
		c_j = j-1;
		while(c_j>=0&&!isB(c_i,c_j))
		{
			//move[c_i][c_j] = B_C;
			setMove(c_i,c_j,B_C,R);
			if(isR(c_i,c_j))
				break;
			else c_j--;
		}
	}
}
//�ڵ��߷� 
void moveP(int i,int j,int rb) {
	int p_i,p_j;
	if(rb==R)
	{
		p_i = i+1;
		p_j = j;
		while(p_i<=9&&!map[p_i][p_j])
		{
			//move[p_i][p_j] = R_P;
			setMove(p_i,p_j,R_P,B);
			p_i++;
		}
		p_i++;
		while(p_i<=9&&!isR(p_i,p_j))
		{
			if(isB(p_i,p_j))
			{
				//move[p_i][p_j] = R_P;
				setMove(p_i,p_j,R_P,B);
				break;
			}
			p_i++;
		}
		p_i = i-1;
		p_j = j;
		while(p_i>=0&&!map[p_i][p_j])
		{
			//move[p_i][p_j] = R_P;
			setMove(p_i,p_j,R_P,B);
			p_i--;
		}
		p_i--;
		while(p_i>=0&&!isR(p_i,p_j))
		{
			if(isB(p_i,p_j))
			{
				//move[p_i][p_j] = R_P;
				setMove(p_i,p_j,R_P,B);
				break;
			}
			p_i--;
		}
		p_i = i;
		p_j = j+1;
		while(p_j<=8&&!map[p_i][p_j])
		{
			//move[p_i][p_j] = R_P;
			setMove(p_i,p_j,R_P,B);
			p_j++;
		}
		p_j++;
		while(p_j<=8&&!isR(p_i,p_j))
		{
			if(isB(p_i,p_j))
			{
				//move[p_i][p_j] = R_P;
				setMove(p_i,p_j,R_P,B);
				break;
			}
			p_j++;
		}
		p_i = i;
		p_j = j-1;
		while(p_j>=0&&!map[p_i][p_j])
		{
			//move[p_i][p_j] = R_P;
			setMove(p_i,p_j,R_P,B);
			p_j--;
		}
		p_j--;
		while(p_j>=0&&!isR(p_i,p_j))
		{
			if(isB(p_i,p_j))
			{
				//move[p_i][p_j] = R_P;
				setMove(p_i,p_j,R_P,B);
				break;
			}
			p_j--;
		}
	}
	if(rb==B)
	{
		p_i = i+1;
		p_j = j;
		while(p_i<=9&&!map[p_i][p_j])
		{
			//move[p_i][p_j] = B_P;
			setMove(p_i,p_j,B_P,R);
			p_i++;
		}
		p_i++;
		while(p_i<=9&&!isB(p_i,p_j))
		{
			if(isR(p_i,p_j))
			{
				//move[p_i][p_j] = B_P;
				setMove(p_i,p_j,B_P,R);
				break;
			}
			p_i++;
		}
		p_i = i-1;
		p_j = j;
		while(p_i>=0&&!map[p_i][p_j])
		{
			//move[p_i][p_j] = B_P;
			setMove(p_i,p_j,B_P,R);
			p_i--;
		}
		p_i--;
		while(p_i>=0&&!isB(p_i,p_j))
		{
			if(isR(p_i,p_j))
			{
				//move[p_i][p_j] = B_P;
				setMove(p_i,p_j,B_P,R);
				break;
			}
			p_i--;
		}
		p_i = i;
		p_j = j+1;
		while(p_j<=8&&!map[p_i][p_j])
		{
			//move[p_i][p_j] = B_P;
			setMove(p_i,p_j,B_P,R);
			p_j++;
		}
		p_j++;
		while(p_j<=8&&!isB(p_i,p_j))
		{
			if(isR(p_i,p_j))
			{
				//move[p_i][p_j] = B_P;
				setMove(p_i,p_j,B_P,R);
				break;
			}
			p_j++;
		}
		p_i = i;
		p_j = j-1;
		while(p_j>=0&&!map[p_i][p_j])
		{
			//move[p_i][p_j] = B_P;
			setMove(p_i,p_j,B_P,R);
			p_j--;
		}
		p_j--;
		while(p_j>=0&&!isB(p_i,p_j))
		{
			if(isR(p_i,p_j))
			{
				//move[p_i][p_j] = B_P;
				setMove(p_i,p_j,B_P,R);
				break;
			}
			p_j--;
		}
	}
}
//�����߷� 
void moveB(int i,int j,int rb) {
	if(rb==R)
	{
		if(i<=4)
		{
			if(((i+1)<=9)&&(!isR(i+1,j)))
				//move[i+1][j] = R_B;
				setMove(i+1,j,R_B,B);
		}
		else {
			if(((i+1)<=9)&&(!isR(i+1,j)))
				//move[i+1][j] = R_B;
				setMove(i+1,j,R_B,B);
			if(((j+1)<=8)&&(!isR(i,j+1)))
				//move[i][j+1] = R_B;
				setMove(i,j+1,R_B,B);
			if(((j-1)>=0)&&(!isR(i,j-1)))
				//move[i][j-1] = R_B;
				setMove(i,j-1,R_B,B);
		}
	}
	if(rb==B)
	{
		if(i>=5)
		{
			if(((i-1)>=0)&&(!isB(i-1,j)))
				//move[i-1][j] = B_B;
				setMove(i-1,j,B_B,R);
		}
		else {
			if(((i-1)>=0)&&(!isB(i-1,j)))
				//move[i-1][j] = B_B;
				setMove(i-1,j,B_B,R);
			if(((j+1)<=8)&&(!isB(i,j+1)))
				//move[i][j+1] = B_B;
				setMove(i,j+1,B_B,R);
			if(((j-1)>=0)&&(!isB(i,j-1)))
				//move[i][j-1] = B_B;
				setMove(i,j-1,B_B,R);
		}
	}
}
//��ȡ�߷� 
void getMove(i,j){
	switch(map[i][j]){
		case R_J : {
			moveJ(i,j,R);
			break;
		}
		case B_J : {
			moveJ(i,j,B);
			break;
		}
		case R_S : {
			moveS(i,j,R);
			break;
		}
		case B_S : {
			moveS(i,j,B);
			break;
		}
		case R_X : {
			moveX(i,j,R);
			break;
		}
		case B_X : {
			moveX(i,j,B);
			break;
		}
		case R_M : {
			moveM(i,j,R);
			break;
		}
		case B_M : {
			moveM(i,j,B);
			break;
		}
		case R_C : {
			moveC(i,j,R);
			break;
		}
		case B_C : {
			moveC(i,j,B);
			break;
		}
		case R_P : {
			moveP(i,j,R);
			break;
		}
		case B_P : {
			moveP(i,j,B);
			break;
		}
		case R_B : {
			moveB(i,j,R);
			break;
		}
		case B_B : {
			moveB(i,j,B);
			break;
		}
		default: break;
	}
}
//�ж��Ƿ񽫾� 
/*int isCheck(int rb){
	int i,j,rj_i,rj_j,bj_i,bj_j;
	initMove(); 
	if(rb==R)
	{ 
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(isR(i,j)){		//��ȡ�����������ӿ����ߵ�λ�� 
					getMove(i,j);
				}
				if(map[i][j]==R_J){		//���˧��λ�� 
					rj_i = i;
					rj_j = j;
				}
				if(map[i][j]==B_J){		//��ǽ���λ�� 
					bj_i = i;
					bj_j = j;
				}
			}
		}
		if(move[bj_i][bj_j]!=0||rj_j==bj_j)
			return R_CHECK;
		else{
			return 0;
		} 
			
	}
	if(rb==B)
	{ 
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(isB(i,j)){		//��ȡ�����������ӿ����ߵ�λ�� 
					getMove(i,j);
				}
				if(map[i][j]==R_J){		//���˧��λ�� 
					rj_i = i;
					rj_j = j;
				}
				if(map[i][j]==B_J){		//��ǽ���λ�� 
					bj_i = i;
					bj_j = j;
				}
			}
		}
		if(move[rj_i][rj_j]!=0||rj_j==bj_j)
			return B_CHECK;
		else
			return 0;
	}
}*/
int isCheck(int rb){
	int i,j,j_i,j_j;
	if(rb==R)
	{
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(map[i][j]==B_J){		//��ǶԷ�����λ�� 
					j_i = i;
					j_j = j;
					break;
				}
			}
		}
		//�ж϶Է����ܷ��ҷ����Ե� 
		i = j_i;
		j = j_j;
		if(((i-1)>=0)&&map[i-1][j]==R_B)
				return R_CHECK;
		if(((j+1)<=8)&&map[i][j+1]==R_B)
				return R_CHECK;
		if(((j-1)>=0)&&map[i][j-1]==R_B)
				return R_CHECK;
		//�ж϶Է����ܷ��ҷ���/���Ե� 		
		i = j_i+1;
		j = j_j;
		while(i<=9)
		{
			if(map[i][j]==R_C)
				return R_CHECK;
			if(map[i][j])
				break;
			i++;
		}
		i = j_i-1;
		j = j_j;
		while(i>=0)
		{
			if(map[i][j]==R_C||map[i][j]==R_J)
				return R_CHECK;
			if(map[i][j])
				break;
			i--;
		}
		i = j_i;
		j = j_j+1;
		while(j<=8)
		{
			if(map[i][j]==R_C)
				return R_CHECK;
			if(map[i][j])
				break;
			j++;
		}
		i = j_i;
		j = j_j-j;
		while(j>=0)
		{
			if(map[i][j]==R_C)
				return R_CHECK;
			if(map[i][j])
				break;
			j--;
		}
		//�ж϶Է����ܷ��ҷ��ڳԵ�
		i = j_i+1;
		j = j_j;
		while(i<=9&&!map[i][j])i++;
		i++;
		while(i<=9)
		{
			if(map[i][j]==R_P)
				return R_CHECK;
			if(map[i][j])
				break;
			i++;
		}
		i = j_i-1;
		j = j_j;
		while(i>=0&&!map[i][j])i--;
		i--;
		while(i>=0)
		{
			if(map[i][j]==R_P)
				return R_CHECK;
			if(map[i][j])
				break;
			i--;
		}
		i = j_i;
		j = j_j+1;
		while(j<=8&&!map[i][j])j++;
		j++;
		while(j<=8)
		{
			if(map[i][j]==R_P)
				return R_CHECK;
			if(map[i][j])
				break;
			j++;
			
		}
		i = j_i;
		j = j_j-1;
		while(j>=0&&!map[i][j])j--;
		j--;
		while(j>=0)
		{
			if(map[i][j]==R_P)
				return R_CHECK;
			if(map[i][j])
				break;
			j--;
		}
		//�ж϶Է����ܷ��ҷ����Ե�
		i = j_i;
		j = j_j;
		if(map[(i+1)%10][(j+1)%9]==0)
		{
			if((i+2)<=9&&(j+1)<=8&&map[i+2][j+1]==R_M)
				return R_CHECK;
			if((i+1)<=9&&(j+2)<=8&&map[i+1][j+2]==R_M)
				return R_CHECK;
		}
		if(map[(i-1)%10][(j+1)%9]==0)
		{
			if((i-2)>=0&&(j+1)<=8&&map[i-2][j+1]==R_M)
				return R_CHECK;
			if((i-1)>=0&&(j+2)<=8&&map[i-1][j+2]==R_M)
				return R_CHECK;
		}
		if(map[(i+1)%10][(j-1)%9]==0)
		{
			if((i+2)<=9&&(j-1)>=0&&map[i+2][j-1]==R_M)
				return R_CHECK;
			if((i+1)<=9&&(j-2)>=0&&map[i+1][j-2]==R_M)
				return R_CHECK;
		}
		if(map[(i-1)%10][(j-1)%9]==0)
		{
			if((i-2)>=0&&(j-1)>=0&&map[i-2][j-1]==R_M)
				return R_CHECK;
			if((i-1)>=0&&(j-2)>=0&&map[i-1][j-2]==R_M)
				return R_CHECK;
		}
		return 0;
	}
	
	if(rb==B)
	{
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(map[i][j]==R_J){		//��ǶԷ�����λ�� 
					j_i = i;
					j_j = j;
					break; 
				}
			}
		}
		//�ж϶Է����ܷ��ҷ����Ե� 
		i = j_i;
		j = j_j;
		if(((i+1)<=9)&&map[i+1][j]==B_B)
				return B_CHECK;
		if(((j+1)<=8)&&map[i][j+1]==B_B)
				return B_CHECK;
		if(((j-1)>=0)&&map[i][j-1]==B_B)
				return B_CHECK;
		//�ж϶Է����ܷ��ҷ���/���Ե� 		
		i = j_i+1;
		j = j_j;
		while(i<=9)
		{
			if(map[i][j]==B_C||map[i][j]==B_J)
				return B_CHECK;
			if(map[i][j])
				break;
			i++;
		}
		i = j_i-1;
		j = j_j;
		while(i>=0)
		{
			if(map[i][j]==B_C)
				return B_CHECK;
			if(map[i][j])
				break;
			i--;
		}
		i = j_i;
		j = j_j+1;
		while(j<=8)
		{
			if(map[i][j]==B_C)
				return B_CHECK;
			if(map[i][j])
				break;
			j++;
		}
		i = j_i;
		j = j_j-1;
		while(j>=0)
		{
			if(map[i][j]==B_C)
				return B_CHECK;
			if(map[i][j])
				break;
			j--;
		}
		//�ж϶Է����ܷ��ҷ��ڳԵ�
		i = j_i+1;
		j = j_j;
		while(i<=9&&!map[i][j])i++;
		i++;
		while(i<=9)
		{
			if(map[i][j]==B_P)
				return B_CHECK;
			if(map[i][j])
				break;
			i++;
		}
		i = j_i-1;
		j = j_j;
		while(i>=0&&!map[i][j])i--;
		i--;
		while(i>=0)
		{
			if(map[i][j]==B_P)
				return B_CHECK;
			if(map[i][j])
				break;
			i--;
		}
		i = j_i;
		j = j_j+1;
		while(j<=8&&!map[i][j])j++;
		j++;
		while(j<=8)
		{
			if(map[i][j]==B_P)
				return B_CHECK;
			if(map[i][j])
				break;
			j++;
		}
		i = j_i;
		j = j_j-1;
		while(j>=0&&!map[i][j])j--;
		j--;
		while(j>=0)
		{
			if(map[i][j]==B_P)
				return B_CHECK;
			if(map[i][j])
				break;
			j--;
		}
		//�ж϶Է����ܷ��ҷ����Ե�
		i = j_i;
		j = j_j;
		if(map[(i+1)%10][(j+1)%9]==0)
		{
			if((i+2)<=9&&(j+1)<=8&&map[i+2][j+1]==B_M)
				return B_CHECK;
			if((i+1)<=9&&(j+2)<=8&&map[i+1][j+2]==B_M)
				return B_CHECK;
		}
		if(map[(i-1)%10][(j+1)%9]==0)
		{
			if((i-2)>=0&&(j+1)<=8&&map[i-2][j+1]==B_M)
				return B_CHECK;
			if((i-1)>=0&&(j+2)<=8&&map[i-1][j+2]==B_M)
				return B_CHECK;
		}
		if(map[(i+1)%10][(j-1)%9]==0)
		{
			if((i+2)<=9&&(j-1)>=0&&map[i+2][j-1]==B_M)
				return B_CHECK;
			if((i+1)<=9&&(j-2)>=0&&map[i+1][j-2]==B_M)
				return B_CHECK;
		}
		if(map[(i-1)%10][(j-1)%9]==0)
		{
			if((i-2)>=0&&(j-1)>=0&&map[i-2][j-1]==B_M)
				return B_CHECK;
			if((i-1)>=0&&(j-2)>=0&&map[i-1][j-2]==B_M)
				return B_CHECK;
		}
		return 0;
	}
}

int isCheckmate(int rb)
{
	int i,j,checkmate_i,checkmate_j;
	//checkmate_i = set_i;
	//checkmate_j = set_j;
	initMove();
	if(rb==R)
	{ 
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(isB(i,j)){		//��ȡ�����������ӿ����ߵ�λ�� 
					set_i = i;
					set_j = j;
					getMove(i,j);
				}
			}
		}
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(move[i][j]!=0){ 
					//set_i = checkmate_i;
					//set_j = checkmate_j;
					return B_SET;
				}
			}
		}
		//set_i = checkmate_i;
		//set_j = checkmate_j;
		return  R_WIN; 
	}
	if(rb==B)
	{ 
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(isR(i,j)){		//��ȡ�����������ӿ����ߵ�λ�� 
					set_i = i;
					set_j = j;
					getMove(i,j);
				}
			}
		}
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(move[i][j]!=0){ 
					//set_i = checkmate_i;
					//set_j = checkmate_j;
					return R_SET; 
				}
			}
		}
		//set_i = checkmate_i;
		//set_j = checkmate_j;
		return  B_WIN; 
	}
}

//����Ļ����ת��Ϊ��Ϸ���� 
void doGame(int j,int i,int rb){
	//������Ϸ״̬�Լ���������������Ӧ 
	switch(gameflag) {
		case BEGIN : {
			if(netflag!=ON_RB)break;
			gameflag = READY;
			break;
		}
		case READY : {
			if(netflag!=ON_RB)break;
			if((j==11||j==12)&&(i==4))
			{
				readyflag = readyflag|rb;
			}
			if(readyflag == ON_RB)
			{
				initMove();
				initMap();
				gameflag = R_SET;
			}
			break;
		} 
		case R_SET : {
			//checkflag = isCheck(B);
			if(rb!=R)break;
			if(0<=i&&i<=9&&0<=j&&j<=8&&isR(i,j))
			{
				selCM(i,j);
				getMove(i,j);
			}
			else
				if(0<=i&&i<=9&&0<=j&&j<=8&&move[i][j]!=0)
				{
					setOldMap(R);
					setCM(i,j);
					gameflag = isCheckmate(R);
					initMove();
					checkflag = isCheck(B);
				}
			break;
		}
		case B_SET : {
			//checkflag = isCheck(R);
			if(rb!=B)break;
			if(0<=i&&i<=9&&0<=j&&j<=8&&isB(i,j))
			{
				selCM(i,j);
				getMove(i,j);
			}
			else
				if(0<=i&&i<=9&&0<=j&&j<=8&&move[i][j]!=0)
				{
					setOldMap(B);
					setCM(i,j);	
					gameflag = isCheckmate(B);
					initMove();
					checkflag = isCheck(B);
				}
			break;
		}  
		default: {
			break;
		};
	}
}
void setAct()
{
	if(gameflag == R_SET)
	{
		if(self == R)
			undoflag = 1;
		else 
			undoflag = 0;
		tieflag = 1;
		giveupflag =1;
	}
	else if(gameflag == B_SET)
	{
		if(self == B)
			undoflag = 1;
		else
			undoflag = 0;
		tieflag = 1;
		giveupflag =1;
	}
	else
	{
		undoflag = 0;
		tieflag = 0;
		giveupflag = 0;
	}
}
//�������� 
void drawChessman(HDC hdc)
{
	int i,j;
	hMemdc=CreateCompatibleDC(hdc);
	for(i=0;i<=MAX_Y;i++){
		for(j=0;j<=MAX_X;j++){
			if(map[i][j]!=0){
				//hMemdc=CreateCompatibleDC(hdc);
				SelectObject(hMemdc,hRC[map[i][j]]);
				TransparentBlt(hdc,getX(j),getY(i),50,50,hMemdc,0,0,50,50,RGB(255,0,255));
				//DeleteDC(hMemdc);
			}
		}
	}
	for(i=0;i<=MAX_Y;i++){
		for(j=0;j<=MAX_X;j++){
			if(move[i][j]!=0){
				//hMemdc=CreateCompatibleDC(hdc);
				SelectObject(hMemdc,hRC[15]);
				BitBlt(hdc,getX(j),getY(i),50,50,hMemdc,0,0,SRCAND);
				//DeleteDC(hMemdc);
			}
		}
	}
	if(set_i>=0&&set_j>=0){
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hRC[0]);
		TransparentBlt(hdc,getX(set_j),getY(set_i),50,50,hMemdc,0,0,50,50,RGB(255,255,255));
		//DeleteDC(hMemdc);
	}
	DeleteDC(hMemdc);
}

void drawStatus(HDC hdc)
{
#ifdef DEBUG
	TCHAR str[30];
	wsprintf(str,"%s ��Ϸ״̬ %s",status[self],gamestatus[gameflag]);
	TextOut(hdc,0,0,str,strlen(str));
	wsprintf(str,"����״̬ %s",checkstatus[checkflag]);
	TextOut(hdc,200,0,str,strlen(str));
	wsprintf(str,"����״̬ %s",netstatus[netflag]);
	TextOut(hdc,300,0,str,strlen(str));
	wsprintf(str,"׼��״̬ %s",netstatus[readyflag]);
	TextOut(hdc,400,0,str,strlen(str));
#endif 
	setAct();
	
	HFONT hFont = CreateFont(
		24,
        0,
        0,
        0,
        //FW_NORMAL,
        FW_BLACK,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        //DEFAULT_PITCH|FF_SWISS,
        DEFAULT_PITCH,
        _T("΢���ź�"));
	HFONT hFontOld = SelectObject(hdc,hFont);
	//SetTextColor(hdc,RGB(0,0,255)); 
	//SetBkColor(hdc,RGB(0,0,0));
	SetBkMode(hdc,TRANSPARENT);
	TextOut(hdc,560,510,gamestatus[gameflag],strlen(gamestatus[gameflag]));
	SelectObject(hdc,hFontOld);
	
	hMemdc=CreateCompatibleDC(hdc);
	if(netflag&R)
	{
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hUI[1]);
		BitBlt(hdc,640,90,80,80,hMemdc,0,0,SRCCOPY);
		//DeleteDC(hMemdc);
	}
	if(netflag&B)
	{
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hUI[2]);
		BitBlt(hdc,640,340,80,80,hMemdc,0,0,SRCCOPY);
		//DeleteDC(hMemdc);
	}
	if(self == R)
	{
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hUI[3]);
		TransparentBlt(hdc,550,90,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
		SelectObject(hMemdc,hUI[4]);
		TransparentBlt(hdc,550,340,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
		//DeleteDC(hMemdc);
	}
	else
	{
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hUI[4]);
		TransparentBlt(hdc,550,90,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
		SelectObject(hMemdc,hUI[3]);
		TransparentBlt(hdc,550,340,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
		//DeleteDC(hMemdc);
	}
	if(readyflag&R)
	{
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hUI[5]);
		TransparentBlt(hdc,550,130,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
		//DeleteDC(hMemdc);
	}
	if(readyflag&B)
	{
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hUI[5]);
		TransparentBlt(hdc,550,380,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
		//DeleteDC(hMemdc);
	}
	if(gameflag == R_SET)
	{
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hUI[0]);
		TransparentBlt(hdc,500,90,20,80,hMemdc,0,0,20,80,RGB(255,255,255));
		//DeleteDC(hMemdc);
	}
	else if(gameflag == B_SET)
	{
		//hMemdc=CreateCompatibleDC(hdc);
		SelectObject(hMemdc,hUI[0]);
		TransparentBlt(hdc,500,340,20,80,hMemdc,0,0,20,80,RGB(255,255,255));
		//DeleteDC(hMemdc);
	}
	if(checkflag != 0)
	{
		SelectObject(hMemdc,hUI[6]);
		TransparentBlt(hdc,155,230,200,100,hMemdc,0,0,200,100,RGB(255,255,255));
		checkflag = 0;
	}
	//if(undoflag != 0&&(self==R&&gameflag==R_SET||self==B&&gameflag==B_SET))
	if(undoflag != 0&&undotimes != 0)
	{
		SelectObject(hMemdc,hUI[7]);
		BitBlt(hdc,520,470,60,30,hMemdc,0,0,SRCCOPY);
	}
	//if(tieflag != 0&&(gameflag==R_SET||gameflag==B_SET))
	if(tieflag != 0&&tietimes != 0)
	{
		SelectObject(hMemdc,hUI[8]);
		BitBlt(hdc,600,470,60,30,hMemdc,0,0,SRCCOPY);
	}
	//if(giveupflag != 0&&((gameflag==R_SET||gameflag==B_SET)))
	if(giveupflag != 0)
	{
		SelectObject(hMemdc,hUI[9]);
		BitBlt(hdc,680,470,60,30,hMemdc,0,0,SRCCOPY);
	}
	DeleteDC(hMemdc);
	if(gameflag == R_WIN)
	{
		gameflag = READY;
		readyflag = OFF;
		/*hHook=SetWindowsHookEx(WH_CBT,(HOOKPROC)HookProc,0,GetCurrentThreadId());
		if(MessageBox(hwnd,"�췽ʤ����","��Ϸ������", MB_ICONASTERISK|MB_OK)!=IDOK)
			PostMessage(hwnd,WM_CLOSE,0,0);*/
		MessageBox(hwnd,"�췽ʤ����","��Ϸ������", MB_ICONASTERISK|MB_OK);
		PostMessage(hwnd,WM_LBUTTONDOWN,0,0);
	}
	else if(gameflag == B_WIN)
	{
		gameflag = READY;
		readyflag = OFF;
		/*hHook=SetWindowsHookEx(WH_CBT,(HOOKPROC)HookProc,0,GetCurrentThreadId());
		if(MessageBox(hwnd,"�ڷ�ʤ����","��Ϸ������", MB_ICONASTERISK|MB_OK)!=IDOK)
			PostMessage(hwnd,WM_CLOSE,0,0);*/
		MessageBox(hwnd,"�ڷ�ʤ����","��Ϸ������", MB_ICONASTERISK|MB_OK);
		PostMessage(hwnd,WM_LBUTTONDOWN,0,0);
	}
	else if(gameflag == RB_TIE)
	{
		gameflag = READY;
		readyflag = OFF;
		MessageBox(hwnd,"�;֣�","��Ϸ������", MB_ICONASTERISK|MB_OK);
		PostMessage(hwnd,WM_LBUTTONDOWN,0,0);
	}
}

void drawPos(HDC hdc,int X,int Y)
{
	TCHAR str[30];
	wsprintf(str,"X=%d Y=%d j=%d i=%d",X,Y,(X-30)/50,(Y-30)/50);
	TextOut(hdc,500,0,str,strlen(str));
}
