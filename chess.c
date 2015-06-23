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
	{ 0, 0, 0, 0, 1, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 4, 0, 6, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 8, 0, 0, 0, 0}
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
	{"无"}, 
	{"红方"},
	{"黑方"}
};
char gamestatus[GAMEFLAG][20] = {
	{"等待双方加入..."},
	{"等待双方准备..."},
	{"红方回合"},
	{"黑方回合"},
	{"红方胜利!"},
	{"黑方胜利!"},
	{"和局!"}, 
	{"请求悔棋中..."},
	{"请求和局中..."}
};
char checkstatus[CHECKFLAG][20] = {
	{"无"}, 
	{"红方将军"},
	{"黑方将军"}
};
char netstatus[NETFLAG][20] = {
	{"无"}, 
	{"红"},
	{"黑"},
	{"红黑"}
};

//初始化游戏绘图资源 
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
//释放绘图资源 
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
//将棋盘坐标转为屏幕坐标 
int getX(int j){
	return 30+j*50;
}
int getY(int i){
	return 30+i*50;
}
//判断棋子颜色 
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
//初始化走法 
void initMove(){
	set_i = -1;
	set_j = -1;
	memset(move,0,sizeof(move));
}
//初始化棋盘 
void initMap(){
	checkflag = 0;
	undoflag = 0;
	tieflag = 0;
	giveupflag = 0;
	undotimes = -1;
	tietimes = 3;
	memcpy(map,basemap,sizeof(map));
	memcpy(oldmap_r,basemap,sizeof(map));
	memcpy(oldmap_b,basemap,sizeof(map));
}
//保存棋盘 
void saveMap(){
	check_i = set_i;
	check_j = set_j;
	memcpy(checkmap,map,sizeof(map));
}
//恢复棋盘 
void loadMap(){
	set_i = check_i;
	set_j = check_j;
	memcpy(map,checkmap,sizeof(map));
}
//保存悔棋棋盘 
void setOldMap(int rb)
{
	if(rb == R)
		memcpy(oldmap_r,map,sizeof(map));
	else if(rb ==B)
		memcpy(oldmap_b,map,sizeof(map));
	if(rb==self)
	{
		backflag = 1;
		if(undotimes == -1)
		undotimes = 3;
	}
}
//还原悔棋棋盘 
void getOldMap(int rb)
{
	if(rb == R)
		memcpy(map,oldmap_r,sizeof(map));
	else if(rb ==B)
		memcpy(map,oldmap_b,sizeof(map));
	if(rb == self)
	backflag = 0;
}
//悔棋 
void backGame(int rb)
{
	getOldMap(rb);
	initMove();
}
//选择棋子 
void selCM(int i,int j)
{
	initMove();
	set_i = i;
	set_j = j;
}
//移动棋子 
void setCM(int i,int j)
{
	map[i][j] = move[i][j];
	map[set_i][set_j] = 0;
	set_i = -1;
	set_j = -1;
}
//判断将军添加走法 
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
//将，帅的走法 
void moveJ(int i,int j,int rb)
{
	if(rb==R)
	{
		if(((i+1)<=2)&&(!isR(i+1,j)))
			setMove(i+1,j,R_J,B);
		if(((i-1)>=0)&&(!isR(i-1,j)))
			setMove(i-1,j,R_J,B);
		if(((j+1)<=5)&&(!isR(i,j+1)))
			setMove(i,j+1,R_J,B);
		if(((j-1)>=3)&&(!isR(i,j-1)))
			setMove(i,j-1,R_J,B);
	}
	if(rb==B)
	{
		if(((i+1)<=9)&&(!isB(i+1,j)))
			setMove(i+1,j,B_J,R);
		if(((i-1)>=7)&&(!isB(i-1,j)))
			setMove(i-1,j,B_J,R);
		if(((j+1)<=5)&&(!isB(i,j+1)))
			setMove(i,j+1,B_J,R);
		if(((j-1)>=3)&&(!isB(i,j-1)))
			setMove(i,j-1,B_J,R);
	}
}
//士的走法 
void moveS(int i,int j,int rb) 
{
	if(rb==R)
	{
		if(i!=1)
		{
			if(!isR(1,4))
				setMove(1,4,R_S,B);
		} 
		else 
		{
			if(!isR(0,3))
				setMove(0,3,R_S,B);
			if(!isR(0,5))
				setMove(0,5,R_S,B);
			if(!isR(2,3))
				setMove(2,3,R_S,B);
			if(!isR(2,5))
				setMove(2,5,R_S,B);
		}
	}
	if(rb==B)
	{
		if(i!=8)
		{
			if(!isB(8,4))
				setMove(8,4,B_S,R);
		} 
		else 
		{
			if(!isB(7,3))
				setMove(7,3,B_S,R);
			if(!isB(7,5))
				setMove(7,5,B_S,R);
			if(!isB(9,3))
				setMove(9,3,B_S,R);
			if(!isB(9,5))
				setMove(9,5,B_S,R);
		}
	}
}
//相的走法 
void moveX(int i,int j,int rb){
	if(rb==R)
	{
		if((i+2)<=4&&(j+2)<=8)
		{
			if(map[i+1][j+1]==0&&!isR(i+2,j+2))
				setMove(i+2,j+2,R_X,B);
		}
		if((i+2)<=4&&(j-2)>=0)
		{
			if(map[i+1][j-1]==0&&!isR(i+2,j-2))
				setMove(i+2,j-2,R_X,B);
		}
		if((i-2)>=0&&(j+2)<=8)
		{
			if(map[i-1][j+1]==0&&!isR(i-2,j+2))
				setMove(i-2,j+2,R_X,B);
		}
		if((i-2)>=0&&(j-2)>=0)
		{
			if(map[i-1][j-1]==0&&!isR(i-2,j-2))
				setMove(i-2,j-2,R_X,B);
		}
	}
	if(rb==B)
	{
		if((i+2)<=9&&(j+2)<=8)
		{
			if(map[i+1][j+1]==0&&!isB(i+2,j+2))
				setMove(i+2,j+2,B_X,R);
		}
		if((i+2)<=9&&(j-2)>=0)
		{
			if(map[i+1][j-1]==0&&!isB(i+2,j-2))
				setMove(i+2,j-2,B_X,R);
		}
		if((i-2)>=5&&(j+2)<=8)
		{
			if(map[i-1][j+1]==0&&!isB(i-2,j+2))
				setMove(i-2,j+2,B_X,R);
		}
		if((i-2)>=5&&(j-2)>=0)
		{
			if(map[i-1][j-1]==0&&!isB(i-2,j-2))
				setMove(i-2,j-2,B_X,R);
		}
	}
} 
//马的走法 
void moveM(int i,int j,int rb) {
	if(rb==R)
	{
		if(map[(i+1)%10][j]==0)
		{
			if((i+2)<=9&&(j+1)<=8&&!isR(i+2,j+1))
				setMove(i+2,j+1,R_M,B);
			if((i+2)<=9&&(j-1)>=0&&!isR(i+2,j-1))
				setMove(i+2,j-1,R_M,B);
		}
		if(map[(i-1)%10][j]==0)
		{
			if((i-2)>=0&&(j+1)<=8&&!isR(i-2,j+1))
				setMove(i-2,j+1,R_M,B);
			if((i-2)>=0&&(j-1)>=0&&!isR(i-2,j-1))
				setMove(i-2,j-1,R_M,B);
		}
		if(map[i][(j+1)%10]==0)
		{
			if((i+1)<=9&&(j+2)<=8&&!isR(i+1,j+2))
				setMove(i+1,j+2,R_M,B);
			if((i-1)>=0&&(j+2)<=8&&!isR(i-1,j+2))
				setMove(i-1,j+2,R_M,B);
		}
		if(map[i][(j-1)%10]==0)
		{
			if((i+1)<=9&&(j-2)>=0&&!isR(i+1,j-2))
				setMove(i+1,j-2,R_M,B);
			if((i-1)>=0&&(j-2)>=0&&!isR(i-1,j-2))
				setMove(i-1,j-2,R_M,B);
		}
	}
	if(rb==B)
	{
		if(map[(i+1)%10][j]==0)
		{
			if((i+2)<=9&&(j+1)<=8&&!isB(i+2,j+1))
				setMove(i+2,j+1,B_M,R);
			if((i+2)<=9&&(j-1)>=0&&!isB(i+2,j-1))
				setMove(i+2,j-1,B_M,R);
		}
		if(map[(i-1)%10][j]==0)
		{
			if((i-2)>=0&&(j+1)<=8&&!isB(i-2,j+1))
				setMove(i-2,j+1,B_M,R);
			if((i-2)>=0&&(j-1)>=0&&!isB(i-2,j-1))
				setMove(i-2,j-1,B_M,R);
		}
		if(map[i][(j+1)%10]==0)
		{
			if((i+1)<=9&&(j+2)<=8&&!isB(i+1,j+2))
				setMove(i+1,j+2,B_M,R);
			if((i-1)>=0&&(j+2)<=8&&!isB(i-1,j+2))
				setMove(i-1,j+2,B_M,R);
		}
		if(map[i][(j-1)%10]==0)
		{
			if((i+1)<=9&&(j-2)>=0&&!isB(i+1,j-2))
				setMove(i+1,j-2,B_M,R);
			if((i-1)>=0&&(j-2)>=0&&!isB(i-1,j-2))
				setMove(i-1,j-2,B_M,R);
		}
	}
}
//车的走法 
void moveC(int i,int j,int rb) {
	int c_i,c_j;
	if(rb==R)
	{
		c_i = i+1;
		c_j = j;
		while(c_i<=9&&!isR(c_i,c_j))
		{
			setMove(c_i,c_j,R_C,B);
			if(isB(c_i,c_j))
				break;
			else c_i++;
		}
		c_i = i-1;
		c_j = j;
		while(c_i>=0&&!isR(c_i,c_j))
		{
			setMove(c_i,c_j,R_C,B);
			if(isB(c_i,c_j))
				break;
			else c_i--;
		}
		c_i = i;
		c_j = j+1;
		while(c_j<=8&&!isR(c_i,c_j))
		{
			setMove(c_i,c_j,R_C,B);
			if(isB(c_i,c_j))
				break;
			else c_j++;
		}
		c_i = i;
		c_j = j-1;
		while(c_j>=0&&!isR(c_i,c_j))
		{
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
			setMove(c_i,c_j,B_C,R);
			if(isR(c_i,c_j))
				break;
			else c_i++;
		}
		c_i = i-1;
		c_j = j;
		while(c_i>=0&&!isB(c_i,c_j))
		{
			setMove(c_i,c_j,B_C,R);
			if(isR(c_i,c_j))
				break;
			else c_i--;
		}
		c_i = i;
		c_j = j+1;
		while(c_j<=8&&!isB(c_i,c_j))
		{
			setMove(c_i,c_j,B_C,R);
			if(isR(c_i,c_j))
				break;
			else c_j++;
		}
		c_i = i;
		c_j = j-1;
		while(c_j>=0&&!isB(c_i,c_j))
		{
			setMove(c_i,c_j,B_C,R);
			if(isR(c_i,c_j))
				break;
			else c_j--;
		}
	}
}
//炮的走法 
void moveP(int i,int j,int rb) {
	int p_i,p_j;
	if(rb==R)
	{
		p_i = i+1;
		p_j = j;
		while(p_i<=9&&!map[p_i][p_j])
		{
			setMove(p_i,p_j,R_P,B);
			p_i++;
		}
		p_i++;
		while(p_i<=9&&!isR(p_i,p_j))
		{
			if(isB(p_i,p_j))
			{
				setMove(p_i,p_j,R_P,B);
				break;
			}
			p_i++;
		}
		p_i = i-1;
		p_j = j;
		while(p_i>=0&&!map[p_i][p_j])
		{
			setMove(p_i,p_j,R_P,B);
			p_i--;
		}
		p_i--;
		while(p_i>=0&&!isR(p_i,p_j))
		{
			if(isB(p_i,p_j))
			{
				setMove(p_i,p_j,R_P,B);
				break;
			}
			p_i--;
		}
		p_i = i;
		p_j = j+1;
		while(p_j<=8&&!map[p_i][p_j])
		{
			setMove(p_i,p_j,R_P,B);
			p_j++;
		}
		p_j++;
		while(p_j<=8&&!isR(p_i,p_j))
		{
			if(isB(p_i,p_j))
			{
				setMove(p_i,p_j,R_P,B);
				break;
			}
			p_j++;
		}
		p_i = i;
		p_j = j-1;
		while(p_j>=0&&!map[p_i][p_j])
		{
			setMove(p_i,p_j,R_P,B);
			p_j--;
		}
		p_j--;
		while(p_j>=0&&!isR(p_i,p_j))
		{
			if(isB(p_i,p_j))
			{
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
			setMove(p_i,p_j,B_P,R);
			p_i++;
		}
		p_i++;
		while(p_i<=9&&!isB(p_i,p_j))
		{
			if(isR(p_i,p_j))
			{
				setMove(p_i,p_j,B_P,R);
				break;
			}
			p_i++;
		}
		p_i = i-1;
		p_j = j;
		while(p_i>=0&&!map[p_i][p_j])
		{
			setMove(p_i,p_j,B_P,R);
			p_i--;
		}
		p_i--;
		while(p_i>=0&&!isB(p_i,p_j))
		{
			if(isR(p_i,p_j))
			{
				setMove(p_i,p_j,B_P,R);
				break;
			}
			p_i--;
		}
		p_i = i;
		p_j = j+1;
		while(p_j<=8&&!map[p_i][p_j])
		{
			setMove(p_i,p_j,B_P,R);
			p_j++;
		}
		p_j++;
		while(p_j<=8&&!isB(p_i,p_j))
		{
			if(isR(p_i,p_j))
			{
				setMove(p_i,p_j,B_P,R);
				break;
			}
			p_j++;
		}
		p_i = i;
		p_j = j-1;
		while(p_j>=0&&!map[p_i][p_j])
		{
			setMove(p_i,p_j,B_P,R);
			p_j--;
		}
		p_j--;
		while(p_j>=0&&!isB(p_i,p_j))
		{
			if(isR(p_i,p_j))
			{
				setMove(p_i,p_j,B_P,R);
				break;
			}
			p_j--;
		}
	}
}
//兵的走法 
void moveB(int i,int j,int rb) {
	if(rb==R)
	{
		if(i<=4)
		{
			if(((i+1)<=9)&&(!isR(i+1,j)))
				setMove(i+1,j,R_B,B);
		}
		else {
			if(((i+1)<=9)&&(!isR(i+1,j)))
				setMove(i+1,j,R_B,B);
			if(((j+1)<=8)&&(!isR(i,j+1)))
				setMove(i,j+1,R_B,B);
			if(((j-1)>=0)&&(!isR(i,j-1)))
				setMove(i,j-1,R_B,B);
		}
	}
	if(rb==B)
	{
		if(i>=5)
		{
			if(((i-1)>=0)&&(!isB(i-1,j)))
				setMove(i-1,j,B_B,R);
		}
		else {
			if(((i-1)>=0)&&(!isB(i-1,j)))
				setMove(i-1,j,B_B,R);
			if(((j+1)<=8)&&(!isB(i,j+1)))
				setMove(i,j+1,B_B,R);
			if(((j-1)>=0)&&(!isB(i,j-1)))
				setMove(i,j-1,B_B,R);
		}
	}
}
//获取走法 
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
//判断是否将军 
int isCheck(int rb){
	int i,j,j_i,j_j;
	if(rb==R)
	{
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(map[i][j]==B_J){		//标记对方将的位置 
					j_i = i;
					j_j = j;
					break;
				}
			}
		}
		//判断对方将能否被我方兵吃到 
		i = j_i;
		j = j_j;
		if(((i-1)>=0)&&map[i-1][j]==R_B)
				return R_CHECK;
		if(((j+1)<=8)&&map[i][j+1]==R_B)
				return R_CHECK;
		if(((j-1)>=0)&&map[i][j-1]==R_B)
				return R_CHECK;
		//判断对方将能否被我方车/将吃到 		
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
		//判断对方将能否被我方炮吃到
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
		//判断对方将能否被我方马吃到
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
				if(map[i][j]==R_J){		//标记对方将的位置 
					j_i = i;
					j_j = j;
					break; 
				}
			}
		}
		//判断对方将能否被我方兵吃到 
		i = j_i;
		j = j_j;
		if(((i+1)<=9)&&map[i+1][j]==B_B)
				return B_CHECK;
		if(((j+1)<=8)&&map[i][j+1]==B_B)
				return B_CHECK;
		if(((j-1)>=0)&&map[i][j-1]==B_B)
				return B_CHECK;
		//判断对方将能否被我方车/将吃到 		
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
		//判断对方将能否被我方炮吃到
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
		//判断对方将能否被我方马吃到
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
//判断将死 
int isCheckmate(int rb)
{
	int i,j,checkmate_i,checkmate_j;
	initMove();
	if(rb==R)
	{ 
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(isB(i,j)){		//获取本方所有棋子可以走的位置 
					set_i = i;
					set_j = j;
					getMove(i,j);
				}
			}
		}
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(move[i][j]!=0){ 
					return B_SET;
				}
			}
		}
		return  R_WIN; 
	}
	if(rb==B)
	{ 
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(isR(i,j)){		//获取本方所有棋子可以走的位置 
					set_i = i;
					set_j = j;
					getMove(i,j);
				}
			}
		}
		for(i=0;i<=MAX_Y;i++){
			for(j=0;j<=MAX_X;j++){
				if(move[i][j]!=0){ 
					return R_SET; 
				}
			}
		}
		return  B_WIN; 
	}
}

//将屏幕坐标转换为游戏动作 
void doGame(int j,int i,int rb){
	//根据游戏状态以及棋盘坐标做出反应 
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
			if(rb!=R)break;		//判断操作方是否是我方
			//判断点击位置是否是我方棋子
			if(0<=i&&i<=9&&0<=j&&j<=8&&isR(i,j))
			{
				selCM(i,j);		//根据点击坐标置set_i、set_j
				getMove(i,j);	//根据点击坐等置move
			}
			else
				//判断点击位置是否存在路径
				if(0<=i&&i<=9&&0<=j&&j<=8&&move[i][j]!=0)
				{
					setOldMap(R);	//保存棋盘以便悔棋操作
					setCM(i,j);		//移动棋子完成走子
					checkflag = isCheck(R);		//判断将军
					gameflag = isCheckmate(R);	//判断将死
					initMove();		//清理set_i、set_j和move
				}
			break;
		}
		case B_SET : {
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
					checkflag = isCheck(B);
					gameflag = isCheckmate(B);
					initMove();
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
	if(backflag == 0)
	{
		undoflag = 0;
	}
}
//绘制棋盘 
void drawChessman(HDC hdc)
{
	int i,j;
	hMemdc=CreateCompatibleDC(hdc);
	//绘制棋子 
	for(i=0;i<=MAX_Y;i++){
		for(j=0;j<=MAX_X;j++){
			if(map[i][j]!=0){
				SelectObject(hMemdc,hRC[map[i][j]]);
				TransparentBlt(hdc,getX(j),getY(i),50,50,hMemdc,0,0,50,50,RGB(255,0,255));
			}
		}
	}
	//绘制棋子移动路径
	for(i=0;i<=MAX_Y;i++){
		for(j=0;j<=MAX_X;j++){
			if(move[i][j]!=0){
				SelectObject(hMemdc,hRC[15]);
				BitBlt(hdc,getX(j),getY(i),50,50,hMemdc,0,0,SRCAND);
			}
		}
	}
	//绘制选择标记 
	if(set_i>=0&&set_j>=0){
		SelectObject(hMemdc,hRC[0]);
		TransparentBlt(hdc,getX(set_j),getY(set_i),50,50,hMemdc,0,0,50,50,RGB(255,255,255));
	}
	DeleteDC(hMemdc);
}

void drawStatus(HDC hdc)
{
#ifdef DEBUG
	TCHAR str[30];
	wsprintf(str,"%s 游戏状态 %s",status[self],gamestatus[gameflag]);
	TextOut(hdc,0,0,str,strlen(str));
	wsprintf(str,"将军状态 %s",checkstatus[checkflag]);
	TextOut(hdc,200,0,str,strlen(str));
	wsprintf(str,"网络状态 %s",netstatus[netflag]);
	TextOut(hdc,300,0,str,strlen(str));
	wsprintf(str,"准备状态 %s",netstatus[readyflag]);
	TextOut(hdc,400,0,str,strlen(str));
#endif 
	setAct();
	
	HFONT hFont = CreateFont(
		20,
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
        _T("微软雅黑"));
	HFONT hFontOld = SelectObject(hdc,hFont);
	//SetTextColor(hdc,RGB(0,0,255)); 
	//SetBkColor(hdc,RGB(0,0,0));
	SetBkMode(hdc,TRANSPARENT);
	TextOut(hdc,560,520,gamestatus[gameflag],strlen(gamestatus[gameflag]));
	SelectObject(hdc,hFontOld);
	
	hMemdc=CreateCompatibleDC(hdc);
	if(netflag&R)
	{
		SelectObject(hMemdc,hUI[1]);
		BitBlt(hdc,640,90,80,80,hMemdc,0,0,SRCCOPY);
	}
	if(netflag&B)
	{
		SelectObject(hMemdc,hUI[2]);
		BitBlt(hdc,640,340,80,80,hMemdc,0,0,SRCCOPY);
	}
	if(self == R)
	{
		SelectObject(hMemdc,hUI[3]);
		TransparentBlt(hdc,550,90,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
		SelectObject(hMemdc,hUI[4]);
		TransparentBlt(hdc,550,340,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
	}
	else
	{
		SelectObject(hMemdc,hUI[4]);
		TransparentBlt(hdc,550,90,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
		SelectObject(hMemdc,hUI[3]);
		TransparentBlt(hdc,550,340,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
	}
	if(readyflag&R)
	{
		SelectObject(hMemdc,hUI[5]);
		TransparentBlt(hdc,550,130,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
	}
	if(readyflag&B)
	{
		SelectObject(hMemdc,hUI[5]);
		TransparentBlt(hdc,550,380,80,40,hMemdc,0,0,80,40,RGB(255,255,255));
	}
	if(gameflag == R_SET)
	{
		SelectObject(hMemdc,hUI[0]);
		TransparentBlt(hdc,500,90,20,80,hMemdc,0,0,20,80,RGB(255,255,255));
	}
	else if(gameflag == B_SET)
	{
		SelectObject(hMemdc,hUI[0]);
		TransparentBlt(hdc,500,340,20,80,hMemdc,0,0,20,80,RGB(255,255,255));
	}
	if(checkflag != 0)
	{
		SelectObject(hMemdc,hUI[6]);
		TransparentBlt(hdc,155,230,200,100,hMemdc,0,0,200,100,RGB(255,255,255));
		checkflag = 0;
	}
	if(undoflag != 0&&undotimes > 0)
	{
		SelectObject(hMemdc,hUI[7]);
		BitBlt(hdc,520,470,60,30,hMemdc,0,0,SRCCOPY);
	}
	if(tieflag != 0&&tietimes > 0)
	{
		SelectObject(hMemdc,hUI[8]);
		BitBlt(hdc,600,470,60,30,hMemdc,0,0,SRCCOPY);
	}
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
		MessageBox(hwnd,"红方胜利！","游戏结束！", MB_ICONASTERISK|MB_OK);
		PostMessage(hwnd,WM_LBUTTONDOWN,0,0);
	}
	else if(gameflag == B_WIN)
	{
		gameflag = READY;
		readyflag = OFF;
		MessageBox(hwnd,"黑方胜利！","游戏结束！", MB_ICONASTERISK|MB_OK);
		PostMessage(hwnd,WM_LBUTTONDOWN,0,0);
	}
	else if(gameflag == RB_TIE)
	{
		gameflag = READY;
		readyflag = OFF;
		MessageBox(hwnd,"和局！","游戏结束！", MB_ICONASTERISK|MB_OK);
		PostMessage(hwnd,WM_LBUTTONDOWN,0,0);
	}
}

void drawPos(HDC hdc,int X,int Y)
{
	TCHAR str[30];
	wsprintf(str,"X=%d Y=%d j=%d i=%d",X,Y,(X-30)/50,(Y-30)/50);
	TextOut(hdc,500,0,str,strlen(str));
}

