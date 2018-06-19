#include<graphics.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>

#define FIVE 1           /*五连：OOOOO*/

#define FOUR 2           /*活四：_OOOO_*/
#define SFOUR 3          /*冲四*/
#define SFOURA 4         /*冲四：OOOO_ _OOOO*/
#define SFOURB 5         /*冲四：OOO_O O_OOO*/
#define SFOURC 6         /*冲四：OO_OO*/

#define THREE 7          /*活三：__OOO__ _O_OO_ _OO_O_*/
#define STHREE 8         /*眠三*/
#define STHREEA 9        /*眠三：OOO__ __OOO*/
#define STHREEB 10       /*眠三：O__OO OO__O*/
#define STHREEC 11       /*眠三：O_O_O*/

#define TWO 12           /*活二：___OO___*/
#define STWO 13          /*眠二*/
#define STWOA 14         /*眠二：OO___ __OO*/
#define STWOB 15         /*眠二：__O_O__*/
#define STWOC 16         /*眠二：_O__O_ */

#define NOTYPE 0         /*未定义类型*/

#define BLACK_PLAYER 1   /*黑棋*/
#define WHITE_PLAYER 2   /*白棋*/
#define NOSTONE 0        /*无子*/

#define SIDE_LEN 25      /*边长*/
#define ORIGIN_X 40      /*原点_X*/
#define ORIGIN_Y 40      /*原点_Y*/
#define RADIUS 8         /*半径*/

#define DO 0             /*落子*/
#define UNDO 1           /*悔棋*/
#define RESTART 2        /*重玩*/
#define FAIL 3           /*认输*/
#define HELP 4           /*帮助*/

#define QUIT 0           /*退出*/
#define CONTINUE 1       /*继续*/

/*背景色*/
#define WINBKCOLOR RGB(113, 59, 18)
/*填充色*/
#define TABLEBKCOLOR RGB(233, 150, 122)

#define LEN 15           /*长度*/	
#define MAXINUM 1000000  /*极值*/

typedef struct Point{/*坐标*/
	int X, Y;
}point;

typedef struct GobangChessGame{/*用于表示五子棋当前棋局状态由棋盘数组和棋型数组构成*/
	int table[LEN + 1][LEN + 1];
	int PatternNum[3][20];
}GCG;

struct LuoziZuobiao{/*采用静态栈记录已经落子的坐标*/
	point pos;
	IMAGE image;
}PointNode[LEN * LEN + 1];

int Top = 0;/*执行栈顶第一个元素，初始时栈空间为空，所以初始化为零*/

IMAGE initpicture;/*定义图形对象用于存储初始化游戏界面已备重新开始游戏时重绘开始界面*/

int posvalue[LEN + 1][LEN + 1] = {/*全局变量用以表示棋盘各落子点的权重*/
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
	{ 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int patternB[][20] = {/*定义黑棋的各种棋型*/
	/*五连*/
	{ 5, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER },//OOOOO[0]
	/*活四、冲四*/
	{ 6, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, NOSTONE },//_OOOO_[1]
	{ 5, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, NOSTONE },//OOOO_[2]
	{ 5, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER },//_OOOO[3]
	{ 5, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, BLACK_PLAYER },//OOO_O[4]
	{ 5, BLACK_PLAYER, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER },//O_OOO[5]
	{ 5, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, BLACK_PLAYER, BLACK_PLAYER },//OO_OO[6]
	/*活三、眠三*/
	{ 7, NOSTONE, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, NOSTONE },//__OOO__[7]
	{ 6, NOSTONE, BLACK_PLAYER, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, NOSTONE },//_O_OO_[8]
	{ 6, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, BLACK_PLAYER, NOSTONE },//_OO_O_[9]
	{ 6, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, NOSTONE },//_OO0__[10]
	{ 6, NOSTONE, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, NOSTONE },//__0OO_[11]

	{ 5, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, NOSTONE },//OOO__[12]
	{ 5, NOSTONE, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, BLACK_PLAYER },//__OOO[13]
	{ 5, BLACK_PLAYER, NOSTONE, NOSTONE, BLACK_PLAYER, BLACK_PLAYER },//O__OO[14]
	{ 5, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, NOSTONE, BLACK_PLAYER },//OO__O[15]
	{ 6, BLACK_PLAYER, NOSTONE, BLACK_PLAYER, NOSTONE, BLACK_PLAYER },//O_O_O[16]
	/*活二、眠二*/
	{ 8, NOSTONE, NOSTONE, NOSTONE, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, NOSTONE, NOSTONE },//___OO___[17]
	{ 5, BLACK_PLAYER, BLACK_PLAYER, NOSTONE, NOSTONE, NOSTONE },//OO___[18]
	{ 5, NOSTONE, NOSTONE, NOSTONE, BLACK_PLAYER, BLACK_PLAYER },//___OO[19]
	{ 7, NOSTONE, NOSTONE, BLACK_PLAYER, NOSTONE, BLACK_PLAYER, NOSTONE, NOSTONE },//__O_O__[20]
	{ 6, NOSTONE, BLACK_PLAYER, NOSTONE, NOSTONE, BLACK_PLAYER, NOSTONE }//_O__O_[21]
};

int patternW[][10] = {/*定义白棋的各种棋型*/
	/*五连*/
	{ 5, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER },//OOOOO[0]
	/*活四、冲四*/
	{ 6, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, NOSTONE },//_OOOO_[1]
	{ 5, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, NOSTONE },//OOOO_[2]
	{ 5, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER },//_OOOO[3]
	{ 5, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, WHITE_PLAYER },//OOO_O[4]
	{ 5, WHITE_PLAYER, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER },//O_OOO[5]
	{ 5, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, WHITE_PLAYER, WHITE_PLAYER },//OO_OO[6]
	/*活三、眠三*/
	{ 7, NOSTONE, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, NOSTONE },//__OOO__[7]
	{ 6, NOSTONE, WHITE_PLAYER, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, NOSTONE },//_O_OO_[8]
	{ 6, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, WHITE_PLAYER, NOSTONE },//_OO_O_[9]
	{ 6, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, NOSTONE },//_OO0__[10]
	{ 6, NOSTONE, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, NOSTONE },//__0OO_[11]

	{ 5, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, NOSTONE },//OOO__[12]
	{ 5, NOSTONE, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, WHITE_PLAYER },//__OOO[13]
	{ 5, WHITE_PLAYER, NOSTONE, NOSTONE, WHITE_PLAYER, WHITE_PLAYER },//O__OO[14]
	{ 5, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, NOSTONE, WHITE_PLAYER },//OO__O[15]
	{ 6, WHITE_PLAYER, NOSTONE, WHITE_PLAYER, NOSTONE, WHITE_PLAYER },//O_O_O[16]
	/*活二、眠二*/
	{ 8, NOSTONE, NOSTONE, NOSTONE, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, NOSTONE, NOSTONE },//___OO___[17]
	{ 5, WHITE_PLAYER, WHITE_PLAYER, NOSTONE, NOSTONE, NOSTONE },//OO___[18]
	{ 5, NOSTONE, NOSTONE, NOSTONE, WHITE_PLAYER, WHITE_PLAYER },//___OO[19]
	{ 7, NOSTONE, NOSTONE, WHITE_PLAYER, NOSTONE, WHITE_PLAYER, NOSTONE, NOSTONE },//__O_O__[20]
	{ 6, NOSTONE, WHITE_PLAYER, NOSTONE, NOSTONE, WHITE_PLAYER, NOSTONE }//_O__O_[21]
};
/*----------------------------------------------------------------------*/
int first()/*游戏开场画面*/;
void Init(GCG *pcg);/*初始化函数、绘制游戏界面、初始化棋盘数组、初始化棋型数组*/
int Start(void);/*开始游戏、确定谁先手*/
point Computer(GCG cg);/*电脑落子，该函数返回电脑将要落子的坐标*/
int Estimate(GCG cg, int a, int b, int BLACK_WHITE);/*估值函数，站在电脑的角度返回局面估值*/
void PatternAnalysis(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*棋型分析函数，统计玩家和电脑的各种棋型*/
void Horizontal(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*横向棋型分析函数*/
void Vertical(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*竖向棋型分析函数*/
void LeftSlant(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*左斜向棋型分析函数*/
void RightSlant(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*右斜向棋型分析函数*/
int Pattern(int BLACK_WHITE, int *T);/*棋型匹配函数*/
int Index(int *S, int *T);/*匹配函数*/
point Player(int table[][LEN + 1], int *pchoice);/*玩家下棋，该函数返回玩家落子点，同时带回按钮值*/
void Undo(GCG *pcg);/*悔棋*/
void NewGame(GCG *pcg, int *pwho);/*新游戏*/
void Fail(void);/*认输*/
void Help(void);/*游戏说明*/
void Down(GCG *pcg, int who, point lastpoint);/*落子函数，该函数落子的同时更新棋型表*/
int IsOver(int table[][LEN + 1], point pos);/*游戏结束判断函数*/
int Winner(int who);/*该函数显示胜负*/

/*----------------------------------------------------------------------*/
int main(){/*主函数*/

	if(first())/*如果玩家按了任意键则开始游戏*/
	{
	GCG cg;
	int who = WHITE_PLAYER;
	int choice = -1;
	int flag = 0;
	point lastpoint;
	Init(&cg);
	who = Start();
	do{
		flag = 0;
		if (who == BLACK_PLAYER) {/*轮到黑方即电脑下棋*/
			lastpoint = Computer(cg);
		}
		else{/*轮到玩家即白方下棋*/
			while (true){
				choice = -1;/*初始化按钮选择值为-1*/
				lastpoint = Player(cg.table, &choice);/*轮到白方下棋*/
				if (choice == UNDO){/*玩家选择悔棋*/
					Undo(&cg);
				}
				else if (choice == RESTART){/*玩家选择重玩*/
					NewGame(&cg, &who);
					flag = 1;
					break;
				}
				else if (choice == FAIL){/*玩家选择认输*/
					Fail();
				}
				else if (choice == HELP){/*玩家选择查看说明信息*/
					Help();
				}
				else break;
			}
		}
		if (flag == 0){/*表明开始新游戏，此时lastpoint为无效值故不可调用Down函数*/
			Down(&cg, who, lastpoint);/*落子，更新棋局状态*/
			who = who % 2 + 1;/*轮换*/
		}
		if (IsOver(cg.table, lastpoint)){/*判断游戏是否结束*/
			if (Winner(who) == QUIT){/*已经得出胜负并且玩家选择了退出*/
				break;
			}
			else{/*游戏已经结束，玩家选择继续游戏*/
				NewGame(&cg, &who);
			}
		}
	} while (true);
	}
	return 0;
}
/*----------------------------------------------------------------------*/

int first()/*游戏开场画面*/
{
	initgraph(520, 420);
	setcolor(WINBKCOLOR);/*"五子棋游戏"封面*/
	setfillstyle(BROWN,24);
	fillcircle(180,200,20);
	fillcircle(220,200,20);
	fillcircle(260,200,20);
	fillcircle(300,200,20);
	fillcircle(340,200,20);
	setcolor(TABLEBKCOLOR);
	setbkmode(TRANSPARENT);/*字样背景色设置为透明*/
	outtextxy(173,193,"五");
	outtextxy(213,193,"子");
	outtextxy(253,193,"棋");
	outtextxy(293,193,"游");
	outtextxy(333,193,"戏");
	outtextxy(233,235,"By 高萌");
	setbkmode(OPAQUE);/*字样背景色还原*/

	IMAGE tmp;/*存储图像对象*/
	getimage(&tmp,0,0,textwidth("按任意键进入"),textheight("按任意键进入"));/*保存当前图像*/
	while(1)/*如果不按开始就一直移动*/
	{
		for(int i=0;i<400;i++)/*字样在横坐标0-510范围移动*/
		{
			outtextxy(i,400,"按任意键进入");
			Sleep(10);/*延时*/
			putimage(i,400,&tmp);/*打印被保存图像*/
			if(kbhit())/*按键信息*/
				return 1;

		}
		for(;i>0;i--)/*循环移动*/
		{
			outtextxy(i,400,"按任意键进入");
			Sleep(20); 
			putimage(i,400,&tmp);
			if(kbhit())
				return 1;
		}
	}
}

/*----------------------------------------------------------------------*/
void Init(GCG *pcg){/*初始化函数*/
	int i, j;
	/*创建窗口，修改窗口标题，同时用背景色清屏*/
	initgraph(520, 420);
	//SetWindowText(GetHWnd(), _T("五子棋"));
	setbkcolor(WINBKCOLOR);
	cleardevice();
	/*设置坐标原点*/
	setorigin(ORIGIN_X, ORIGIN_Y);
	/*获取并修改字体样式  windows下的字体*/
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 18;
    //_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;  //字体质量 
	/*设置文字背景模式，文本颜色，字体样式，填充颜色，线条颜色*/
	setbkmode(TRANSPARENT);   //背景设置透明
	settextcolor(WHITE);
	settextstyle(&f);         //设置字体样式
	setfillcolor(TABLEBKCOLOR);   //设置当前填充样式
	setlinecolor(BLACK);
	/*定义棋盘两边的坐标*/
	TCHAR str[] = _T("ABCDEFGHIJKLMNO");
	TCHAR num[] = _T("123456789abcdef");
	/*绘制棋盘区域背景*/
	fillrectangle(0 - RADIUS / 2, 0 - RADIUS / 2, SIDE_LEN * (LEN - 1) + RADIUS / 2, SIDE_LEN * (LEN - 1) + RADIUS / 2);
	/*绘制棋盘*/
	for (i = 1; i <= LEN; i++){
		//Sleep(150);
		outtextxy(-SIDE_LEN, (i - 1) * SIDE_LEN - 9, str[i - 1]);   //用于在指定位置输出字符串。 //横轴
		line(0, (i - 1) * SIDE_LEN, (LEN - 1) * SIDE_LEN, (i - 1) * SIDE_LEN);
		line((i - 1) * SIDE_LEN, 0, (i - 1) * SIDE_LEN, (LEN - 1) * SIDE_LEN);
		outtextxy((i - 1) * SIDE_LEN - 4, -SIDE_LEN - 6, num[i - 1]);  //竖轴
		/* 画小圆 */
		if (i % 4 == 0)
		{
			setfillcolor(BLACK);
			fillcircle((i - 1) * SIDE_LEN, (i - 1) * SIDE_LEN, 2);
			fillcircle((LEN - i) * SIDE_LEN, (i - 1) * SIDE_LEN, 2);
		}
	}
	/*设置按钮字体样式*/
	f.lfHeight = 26;
//	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	/*定义按钮绘制区域所用数据*/
	int width = 90, height = 40, side = 25;
	int a = ORIGIN_X + SIDE_LEN * LEN;
	int b = ORIGIN_Y;
	/*改变坐标原点*/
	setorigin(a, b);
	/*绘制按钮*/
	RECT r1 = { 0, 0 * height + 0 * side, width, 1 * height + 0 * side };  //RECT用来存储成对出现的参数
	RECT r2 = { 0, 1 * height + 1 * side, width, 2 * height + 1 * side };
	RECT r3 = { 0, 2 * height + 2 * side, width, 3 * height + 2 * side };
	RECT r4 = { 0, 3 * height + 3 * side, width, 4 * height + 3 * side };
	rectangle(0, 0 * height + 0 * side, width, 1 * height + 0 * side);
	rectangle(0, 1 * height + 1 * side, width, 2 * height + 1 * side);
	rectangle(0, 2 * height + 2 * side, width, 3 * height + 2 * side);
	rectangle(0, 3 * height + 3 * side, width, 4 * height + 3 * side);
	/* DT_SINGLELINE 使文字显示在一行。回车和换行符都无效。 
	   DT_CENTER 文字水平居中。 
	   DT_VCENTER 文字垂直居中。仅当和 DT_SINGLELINE 一起使用时有效。*/
	drawtext(_T("悔棋"), &r1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	drawtext(_T("重玩"), &r2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	drawtext(_T("认输"), &r3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	drawtext(_T("说明"), &r4, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	/*回归上次坐标原点*/
	setorigin(ORIGIN_X, ORIGIN_Y);
	/*设置线条样式*/

	LINESTYLE style;
	getlinestyle(&style);
	style.style = PS_ENDCAP_SQUARE;
	style.thickness = 2; //thickness 线的宽度，以像素为单位。
	setlinestyle(&style);
	
	/*绘制棋盘外围粗线条*/
	i = 1;
	line(0, (i - 1) * SIDE_LEN, (LEN - 1) * SIDE_LEN, (i - 1) * SIDE_LEN);
	line((i - 1) * SIDE_LEN, 0, (i - 1) * SIDE_LEN, (LEN - 1) * SIDE_LEN);
	i = LEN;
	line(0, (i - 1) * SIDE_LEN, (LEN - 1) * SIDE_LEN, (i - 1) * SIDE_LEN);
	line((i - 1) * SIDE_LEN, 0, (i - 1) * SIDE_LEN, (LEN - 1) * SIDE_LEN);
	/*还原线条样式*/
	style.thickness = 1;  
	setlinestyle(&style);
	/*保存游戏初始界面*/
	getimage(&initpicture, 0 - RADIUS, 0 - RADIUS, SIDE_LEN * (LEN - 1) + 2 * RADIUS + 1, SIDE_LEN * (LEN - 1) + 2 * RADIUS + 1);
	/*初始化棋盘数组*/
	for (i = 1; i <= LEN; i++){
		for (j = 1; j <= LEN; j++){
			pcg->table[i][j] = NOSTONE;
		}
	}
	/*初始化棋型数组*/
	for (i = 0; i <= 2; i++){
		for (j = 0; j <= 19; j++){
			pcg->PatternNum[i][j] = 0;
		}
	}
}
/*----------------------------------------------------------------------*/
int Start(){/*开始函数*/	
	/*定义图片对象保存将要输出提示信息的区域*/
	IMAGE image;
	RECT r1 = { 1 * SIDE_LEN, 2 * SIDE_LEN, SIDE_LEN * (LEN - 2), SIDE_LEN * (LEN - 3) };
	getimage(&image, r1.left, r1.top, r1.right - r1.left, r1.bottom - r1.top);
	/*清除将要输出提示信息的区域*/
	setfillcolor(WINBKCOLOR);
	fillrectangle(1 * SIDE_LEN, 2 * SIDE_LEN, SIDE_LEN * (LEN - 2), SIDE_LEN * (LEN - 3));
	/*设置文字背景样式，输出提示信息，还原文字背景样式*/
	setbkmode(OPAQUE);
	/*  DT_WORDBREAK 自动换行。当文字超过右边界时会自动换行(不拆开单词)。回车符同样可以换行。  */
	drawtext(_T("\n\n       玩家先手？\n\n    是(点击鼠标左键)\n\n    否(点击鼠标右键)"), &r1, DT_WORDBREAK);
	setbkmode(TRANSPARENT);

	/*获取鼠标消息，点击左键返回电脑玩家也就是黑棋先手，反之返回白棋先手*/
	MOUSEMSG m;
	while (true){
		m = GetMouseMsg();
		if (m.uMsg == WM_RBUTTONUP){   //右键是否按下
			/*还原游戏界面*/
			putimage(r1.left, r1.top, &image);
			return BLACK_PLAYER;      //黑棋
		}
		if (m.uMsg == WM_LBUTTONUP){   //左键是否按下
			/*还原游戏界面*/
			putimage(r1.left, r1.top, &image);
			return WHITE_PLAYER;
		}
	}
}
/*----------------------------------------------------------------------*/
point Computer(GCG cg){/*电脑下棋*/
	int i, j, top;
	int lower, upper, max;
	int grade;
	point pos;
	point posArr[225];

	/*检测电脑是否能直接赢棋，若能返回坐标，不能则找到当前最佳下棋点*/
	max = -MAXINUM;
	lower = MAXINUM - 30;
	for (i = 1; i <= LEN; i++){
		for (j = 1; j <= LEN; j++){
			if (cg.table[i][j] == NOSTONE){
				cg.table[i][j] = BLACK_PLAYER;
				grade = Estimate(cg, i, j, WHITE_PLAYER) + posvalue[i][j];
				cg.table[i][j] = NOSTONE;
				if (grade > max){
					max = grade;
					pos.Y = i;
					pos.X = j;
				}
			}
		}
	}
	if (max >= lower) return pos;
	/*没有直接赢棋点，检测对手是否能够一步赢棋，若能则我方必须防守*/
	upper = -MAXINUM + 30;
	top = 0;
	for (i = 1; i <= LEN; i++){
		for (j = 1; j <= LEN; j++){
			if (cg.table[i][j] == NOSTONE){
				cg.table[i][j] = WHITE_PLAYER;
				grade = Estimate(cg, i, j, BLACK_PLAYER);
				cg.table[i][j] = NOSTONE;
				if (grade == upper){
					posArr[top].Y = i;
					posArr[top].X = j;
					top++;
				}
				if (grade < upper){
					upper = grade;
					top = 0;
					posArr[top].Y = i;
					posArr[top].X = j;
					top++;
				}
			}
		}
	}
	if (top == 0) return pos;
	/*存在我方必须防守的点，则找到对己方有利的最佳防守位置*/
	max = -MAXINUM;
	j = 0;
	for (i = 0; i < top; i++){
		cg.table[posArr[i].Y][posArr[i].X] = BLACK_PLAYER;
		grade = Estimate(cg,posArr[i].Y, posArr[i].Y, WHITE_PLAYER) + posvalue[posArr[i].Y][posArr[i].X];
		cg.table[posArr[i].Y][posArr[i].X] = NOSTONE;
		if (grade > max){
			max = grade;
			j = i;
		}
	}
	return posArr[j];
}
/*----------------------------------------------------------------------*/
int Estimate(GCG cg, int a, int b, int BLACK_WHITE){/*估值函数*/
	int black_value = 0, white_value = 0;
	/*分析棋型*/
	PatternAnalysis(cg.table, cg.PatternNum, a, b, DO);
	/*如果有五连返回最值*/
	if (cg.PatternNum[BLACK_PLAYER][FIVE]) return MAXINUM;
	if (cg.PatternNum[WHITE_PLAYER][FIVE]) return -MAXINUM;
	/*两个以上冲四等于一个活四*/
	if (cg.PatternNum[BLACK_PLAYER][SFOUR] > 1) cg.PatternNum[BLACK_PLAYER][FOUR]++;
	if (cg.PatternNum[WHITE_PLAYER][SFOUR] > 1) cg.PatternNum[WHITE_PLAYER][FOUR]++;
	/*定义黑棋和白棋的估值为零*/
	black_value = white_value = 0;
	switch (BLACK_WHITE){
	case BLACK_PLAYER:{/*下一下棋者为黑棋，也就是电脑*/
						  if (cg.PatternNum[BLACK_PLAYER][FOUR])
							  return MAXINUM - 10;
						  if (cg.PatternNum[BLACK_PLAYER][SFOUR])
							  return MAXINUM - 20;
						  if (cg.PatternNum[WHITE_PLAYER][FOUR])
							  return -MAXINUM + 10;
						  if (cg.PatternNum[WHITE_PLAYER][SFOUR] && cg.PatternNum[WHITE_PLAYER][THREE])
							  return -MAXINUM + 20;
						  if (cg.PatternNum[BLACK_PLAYER][THREE] && cg.PatternNum[WHITE_PLAYER][SFOUR] == 0)
							  return MAXINUM - 30;
						  if (cg.PatternNum[WHITE_PLAYER][THREE] > 1 && cg.PatternNum[BLACK_PLAYER][SFOUR] == 0 && cg.PatternNum[BLACK_PLAYER][THREE] == 0)//&& cg.PatternNum[BLACK_PLAYER][STHREE] == 0)
							  return -MAXINUM + 30;

						  if (cg.PatternNum[BLACK_PLAYER][THREE] > 1)
							  black_value += 2000;
						  else{
							  if (cg.PatternNum[BLACK_PLAYER][THREE])
								  black_value += 200;
						  }
						  if (cg.PatternNum[WHITE_PLAYER][THREE] > 1)
							  white_value += 500;
						  else{
							  if (cg.PatternNum[WHITE_PLAYER][THREE])
								  white_value += 100;
						  }
						  white_value += cg.PatternNum[WHITE_PLAYER][STHREE] * 10;
						  black_value += cg.PatternNum[BLACK_PLAYER][STHREE] * 10;
						  white_value += cg.PatternNum[WHITE_PLAYER][TWO] * 4;
						  black_value += cg.PatternNum[BLACK_PLAYER][TWO] * 4;
						  white_value += cg.PatternNum[WHITE_PLAYER][STWO];
						  black_value += cg.PatternNum[BLACK_PLAYER][STWO];
						  break;
	}
	case WHITE_PLAYER:{
						  if (cg.PatternNum[WHITE_PLAYER][FOUR])
							  return -MAXINUM + 10;
						  if (cg.PatternNum[WHITE_PLAYER][SFOUR])
							  return -MAXINUM + 20;
						  if (cg.PatternNum[BLACK_PLAYER][FOUR])
							  return MAXINUM - 10;
						  if (cg.PatternNum[BLACK_PLAYER][SFOUR] && cg.PatternNum[BLACK_PLAYER][THREE])
							  return MAXINUM - 20;
						  if (cg.PatternNum[WHITE_PLAYER][THREE] && cg.PatternNum[BLACK_PLAYER][SFOUR] == 0)
							  return -MAXINUM + 30;
						  if (cg.PatternNum[BLACK_PLAYER][THREE] > 1 && cg.PatternNum[WHITE_PLAYER][SFOUR] == 0 && cg.PatternNum[WHITE_PLAYER][THREE] == 0)// && cg.PatternNum[WHITE_PLAYER][STHREE] == 0)
							  return MAXINUM - 30;
						  if (cg.PatternNum[WHITE_PLAYER][THREE] > 1)
							  white_value += 2000;
						  else{
							  if (cg.PatternNum[WHITE_PLAYER][THREE])
								  white_value += 200;
						  }
						  if (cg.PatternNum[BLACK_PLAYER][THREE] > 1)
							  black_value += 500;
						  else{
							  if (cg.PatternNum[BLACK_PLAYER][THREE])
								  black_value += 100;
						  }
						  white_value += cg.PatternNum[WHITE_PLAYER][STHREE] * 10;
						  black_value += cg.PatternNum[BLACK_PLAYER][STHREE] * 10;
						  white_value += cg.PatternNum[WHITE_PLAYER][TWO] * 4;
						  black_value += cg.PatternNum[BLACK_PLAYER][TWO] * 4;
						  white_value += cg.PatternNum[WHITE_PLAYER][STWO];
						  black_value += cg.PatternNum[BLACK_PLAYER][STWO];
						  break;
	}
	default: break;
	}
	return black_value - white_value;
}
/*----------------------------------------------------------------------*/
void PatternAnalysis(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO){/*棋型分析函数*/
	Horizontal(table, PatternNum, a, b, DO_UNDO);
	Vertical(table, PatternNum, a, b, DO_UNDO);
	LeftSlant(table, PatternNum, a, b, DO_UNDO);
	RightSlant(table, PatternNum, a, b, DO_UNDO);
	for (int BLACK_WHITE = BLACK_PLAYER; BLACK_WHITE <= WHITE_PLAYER; BLACK_WHITE++){/*统计各种眠X棋型的数量*/
		PatternNum[BLACK_WHITE][SFOUR] = PatternNum[BLACK_WHITE][SFOURA] + PatternNum[BLACK_WHITE][SFOURB] + PatternNum[BLACK_WHITE][SFOURC];
		PatternNum[BLACK_WHITE][STHREE] = PatternNum[BLACK_WHITE][STHREEA] + PatternNum[BLACK_WHITE][STHREEB] + PatternNum[BLACK_WHITE][STHREEC];
		PatternNum[BLACK_WHITE][STWO] = PatternNum[BLACK_WHITE][STWOA] + PatternNum[BLACK_WHITE][STWOB] + PatternNum[BLACK_WHITE][STWOC];
	}
}
/*----------------------------------------------------------------------*/
void Horizontal(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO){/*横向棋型分析函数*/
	/*定义数组T，同时将当前落子点对应行上的所有点的值赋给T*/
	int T[LEN + 1] = { 0 };
	int j;
	T[0] = 15;
	for (j = 1; j <= LEN; j++){
		T[j] = table[a][j];
	}
	/*DO表示落子*/
	if (DO_UNDO == DO){
		/*消除上次该行对应的棋型数*/
		T[b] = NOSTONE;
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T)]--;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T)]--;
		/*更新当前该行对应的棋型数*/
		T[b] = table[a][b];
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T)]++;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T)]++;
	}
	else{/*UNDO表示悔棋*/
		/*消除当前该行对应的棋型数*/
		T[b] = table[a][b];
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T)]--;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T)]--;
		/*还原上次该行对应的棋型数*/
		T[b] = NOSTONE;
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T)]++;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T)]++;
	}
}
/*----------------------------------------------------------------------*/
void Vertical(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO){/*竖向分析棋型函数*/
	/*定义数组T，同时将当前落子点对应行上的所有点的值赋给T*/
	int T[LEN + 1] = { 0 };
	int i;
	T[0] = 15;
	for (i = 1; i <= LEN; i++){
		T[i] = table[i][b];
	}
	/*DO表示落子*/
	if (DO_UNDO == DO){
		/*消除上次该行对应的棋型数*/
		T[a] = NOSTONE;
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T)]--;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T)]--;
		/*更新当前该行对应的棋型数*/
		T[a] = table[a][b];
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T)]++;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T)]++;
	}
	else{/*UNDO表示悔棋*/
		/*消除当前该行对应的棋型数*/
		T[a] = table[a][b];
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T)]--;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T)]--;
		/*还原上次该行对应的棋型数*/
		T[a] = NOSTONE;
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T)]++;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T)]++;
	}
}
/*----------------------------------------------------------------------*/
void LeftSlant(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO){/*左斜向棋型分析函数*/
	/*定义数组T，同时将当前落子点对应行上的所有点的值赋给T*/
	int T[LEN + 1] = { 0 };
	int i, j;
	for (i = 0; a + i <= LEN && b - i >= 1; i++){
		T[b - i] = table[a + i][b - i];
	}
	for (j = 1; b + j <= LEN && a - j >= 1; j++){
		T[b + j] = table[a - j][b + j];
	}
	T[b - i] = b + j - (b - i) - 1;
	/*DO表示落子*/
	if (DO_UNDO == DO){
		/*消除上次该行对应的棋型数*/
		T[b] = NOSTONE;
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T + b - i)]--;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T + b - i)]--;
		/*更新当前该行对应的棋型数*/
		T[b] = table[a][b];
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T + b - i)]++;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T + b - i)]++;
	}
	else{/*UNDO表示悔棋*/
		/*消除当前该行对应的棋型数*/
		T[b] = table[a][b];
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T + b - i)]--;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T + b - i)]--;
		/*还原上次该行对应的棋型数*/
		T[b] = NOSTONE;
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T + b - i)]++;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T + b - i)]++;
	}
}
/*----------------------------------------------------------------------*/
void RightSlant(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO){/*分析右斜向棋型函数*/
	/*定义数组T，同时将当前落子点对应行上的所有点的值赋给T*/
	int T[LEN + 1] = { 0 };
	int i, j;
	for (i = 0; a - i >= 1 && b - i >= 1; i++){
		T[b - i] = table[a - i][b - i];
	}
	for (j = 1; a + j <= LEN && b + j <= LEN; j++){
		T[b + j] = table[a + j][b + j];
	}
	T[b - i] = b + j - (b - i) - 1;
	/*DO表示落子*/
	if (DO_UNDO == DO){
		/*消除上次该行对应的棋型数*/
		T[b] = NOSTONE;
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T + b - i)]--;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T + b - i)]--;
		/*更新当前该行对应的棋型数*/
		T[b] = table[a][b];
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T + b - i)]++;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T + b - i)]++;
	}
	else{/*UNDO表示悔棋*/
		/*消除当前该行对应的棋型数*/
		T[b] = table[a][b];
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T + b - i)]--;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T + b - i)]--;
		/*还原上次该行对应的棋型数*/
		T[b] = NOSTONE;
		PatternNum[WHITE_PLAYER][Pattern(WHITE_PLAYER, T + b - i)]++;
		PatternNum[BLACK_PLAYER][Pattern(BLACK_PLAYER, T + b - i)]++;
	}
}
/*----------------------------------------------------------------------*/
int Pattern(int BLACK_WHITE, int *T){/*棋型统计*/
	switch (BLACK_WHITE){
	case BLACK_PLAYER:{
						  if (Index(patternB[0], T))  return FIVE;
						  if (Index(patternB[1], T))  return FOUR;
						  if (Index(patternB[2], T))  return SFOURA;
						  if (Index(patternB[3], T))  return SFOURA;
						  if (Index(patternB[4], T))  return SFOURB;
						  if (Index(patternB[5], T))  return SFOURB;
						  if (Index(patternB[6], T))  return SFOURC;
						  if (Index(patternB[7], T))  return THREE;
						  if (Index(patternB[8], T))  return THREE;
						  if (Index(patternB[9], T))  return THREE;
						  if (Index(patternB[10], T)) return THREE;
						  if (Index(patternB[11], T)) return THREE;
						  if (Index(patternB[12], T)) return STHREEA;
						  if (Index(patternB[13], T)) return STHREEA;
						  if (Index(patternB[14], T)) return STHREEB;
						  if (Index(patternB[15], T)) return STHREEB;
						  if (Index(patternB[16], T)) return STHREEC;
						  if (Index(patternB[17], T)) return TWO;
						  if (Index(patternB[18], T)) return STWOA;
						  if (Index(patternB[19], T)) return STWOA;
						  if (Index(patternB[20], T)) return STWOB;
						  if (Index(patternB[21], T)) return STWOC;
						  break;
	}
	case WHITE_PLAYER:{
						  if (Index(patternW[0], T))  return FIVE;
						  if (Index(patternW[1], T))  return FOUR;
						  if (Index(patternW[2], T))  return SFOURA;
						  if (Index(patternW[3], T))  return SFOURA;
						  if (Index(patternW[4], T))  return SFOURB;
						  if (Index(patternW[5], T))  return SFOURB;
						  if (Index(patternW[6], T))  return SFOURC;
						  if (Index(patternW[7], T))  return THREE;
						  if (Index(patternW[8], T))  return THREE;
						  if (Index(patternW[9], T))  return THREE;
						  if (Index(patternW[10], T)) return THREE;
						  if (Index(patternW[11], T)) return THREE;
						  if (Index(patternW[12], T)) return STHREEA;
						  if (Index(patternW[13], T)) return STHREEA;
						  if (Index(patternW[14], T)) return STHREEB;
						  if (Index(patternW[15], T)) return STHREEB;
						  if (Index(patternW[16], T)) return STHREEC;
						  if (Index(patternW[17], T)) return TWO;
						  if (Index(patternW[18], T)) return STWOA;
						  if (Index(patternW[19], T)) return STWOA;
						  if (Index(patternW[20], T)) return STWOB;
						  if (Index(patternW[21], T)) return STWOC;
						  break;
	}
	}
	return NOTYPE;
}
/*----------------------------------------------------------------------*/
int Index(int *S, int *T){/*匹配函数*/
	int i, j, m;
	if (T[0] < S[0]) return 0;
	for (i = 1, j = 1; i <= T[0] - S[0] + 1; i++){
		if (T[i] == S[j]){
			m = i;
			while (j <= S[0] && T[m] == S[j]){
				m++; j++;
			}
			if (j == S[0] + 1){
				return i;
			}
			else j = 1;
		}
	}
	return 0;
}
/*----------------------------------------------------------------------*/
point Player(int table[][LEN + 1], int *pchoice){/*玩家下棋*/
	/*变量在此处的作用与Init函数中的一致*/
	int width = 90, height = 40, side = 25;
	int V_x = ORIGIN_X + SIDE_LEN * LEN;/*位移量*/
	int V_y = ORIGIN_Y;
	/*以窗口坐标(0,0)为坐标原点得到按钮区域坐标*/
	RECT r[4] = {
		{ 0, 0 * height + 0 * side, width, 1 * height + 0 * side },
		{ 0, 1 * height + 1 * side, width, 2 * height + 1 * side },
		{ 0, 2 * height + 2 * side, width, 3 * height + 2 * side },
		{ 0, 3 * height + 3 * side, width, 4 * height + 3 * side }
	};
	for (int i = 0; i <= 3; i++){
		r[i].left += V_x; r[i].top += V_y;
		r[i].right += V_x; r[i].bottom += V_y;
	}
	/*鼠标消息*/
	point pos = { -1, -1 };
	int a = 0, b = 0;
	MOUSEMSG m;
	while (true){
		m = GetMouseMsg();
		/*如果鼠标左键弹起并且鼠标所指坐标在棋盘区域内而且最近的一个坐标点无子则跳出循环返回该坐标*/
		if (m.uMsg == WM_LBUTTONUP){
			m.x -= ORIGIN_X;
			m.y -= ORIGIN_Y;

			a = m.x / SIDE_LEN;
			if (m.x % SIDE_LEN > SIDE_LEN / 2) a++;

			b = m.y / SIDE_LEN;
			if (m.y % SIDE_LEN > SIDE_LEN / 2) b++;

			if (table[b + 1][a + 1] == NOSTONE && a < LEN && a >= 0 && b < LEN && b >= 0) break;
		}
		/*鼠标在按钮区域内，改变按钮字体颜色*/
		if (m.x <= r[0].right && m.x >= r[0].left && m.y <= r[0].bottom && m.y >= r[0].top){
			settextcolor(YELLOW);
			setorigin(0, 0);
			setfillcolor(RGB(113, 59, 18));
			fillrectangle(r[0].left, r[0].top, r[0].right, r[0].bottom);
			drawtext(_T("悔棋"), r + 0, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			while (true){
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONUP) {/*鼠标左键弹起，给choice赋值，返回无用坐标*/
					settextcolor(WHITE);
					drawtext(_T("悔棋"), r + 0, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					setorigin(ORIGIN_X, ORIGIN_Y);
					*pchoice = UNDO;
					return pos;
				}/*鼠标离开该区域还原原字体颜色*/
				if (m.x > r[0].right || m.x < r[0].left || m.y > r[0].bottom || m.y < r[0].top){
					settextcolor(WHITE);
					drawtext(_T("悔棋"), r + 0, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					setorigin(ORIGIN_X, ORIGIN_Y);
					break;
				}
			}
			setorigin(ORIGIN_X, ORIGIN_Y);
		}
		/*鼠标在按钮区域内，改变按钮字体颜色*/
		else if (m.x <= r[1].right && m.x >= r[1].left && m.y <= r[1].bottom && m.y >= r[1].top){
			settextcolor(YELLOW);
			setorigin(0, 0);
			setfillcolor(RGB(113, 59, 18));
			fillrectangle(r[1].left, r[1].top, r[1].right, r[1].bottom);
			drawtext(_T("重玩"), r + 1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			while (true){
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONUP){/*鼠标左键弹起，给choice赋值，返回无用坐标*/
					settextcolor(WHITE);
					drawtext(_T("重玩"), r + 1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					setorigin(ORIGIN_X, ORIGIN_Y);
					*pchoice = RESTART;
					return pos;
				}/*鼠标离开该区域还原原字体颜色*/
				if (m.x > r[1].right || m.x < r[1].left || m.y > r[1].bottom || m.y < r[1].top){
					settextcolor(WHITE);
					drawtext(_T("重玩"), r + 1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					setorigin(ORIGIN_X, ORIGIN_Y);
					break;
				}
			}
			setorigin(ORIGIN_X, ORIGIN_Y);
		}
		/*鼠标在按钮区域内，改变按钮字体颜色*/
		else if (m.x <= r[2].right && m.x >= r[2].left && m.y <= r[2].bottom && m.y >= r[2].top){
			settextcolor(YELLOW);
			setorigin(0, 0);
			setfillcolor(RGB(113, 59, 18));
			fillrectangle(r[2].left, r[2].top, r[2].right, r[2].bottom);
			drawtext(_T("认输"), r + 2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			while (true){
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONUP) {/*鼠标左键弹起，给choice赋值，返回无用坐标*/
					settextcolor(WHITE);
					drawtext(_T("认输"), r + 2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					setorigin(ORIGIN_X, ORIGIN_Y);
					*pchoice = FAIL;
					return pos;
				}/*鼠标离开该区域还原原字体颜色*/
				if (m.x > r[2].right || m.x < r[2].left || m.y > r[2].bottom || m.y < r[2].top){
					settextcolor(WHITE);
					drawtext(_T("认输"), r + 2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					setorigin(ORIGIN_X, ORIGIN_Y);
					break;
				}
			}
			setorigin(ORIGIN_X, ORIGIN_Y);
		}
		/*鼠标在按钮区域内，改变按钮字体颜色*/
		else if (m.x <= r[3].right && m.x >= r[3].left && m.y <= r[3].bottom && m.y >= r[3].top){
			settextcolor(YELLOW);
			setorigin(0, 0);
			setfillcolor(RGB(113, 59, 18));
			fillrectangle(r[3].left, r[3].top, r[3].right, r[3].bottom);
			drawtext(_T("说明"), r + 3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			while (true){
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONUP){/*鼠标左键弹起，给choice赋值，返回无用坐标*/
					settextcolor(WHITE);
					drawtext(_T("说明"), r + 3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					setorigin(ORIGIN_X, ORIGIN_Y);
					*pchoice = HELP;
					return pos;
				}/*鼠标离开该区域还原原字体颜色*/
				if (m.x > r[3].right || m.x < r[3].left || m.y > r[3].bottom || m.y < r[3].top){
					settextcolor(WHITE);
					drawtext(_T("说明"), r + 3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					setorigin(ORIGIN_X, ORIGIN_Y);
					break;
				}
			}
			setorigin(ORIGIN_X, ORIGIN_Y);
		}
	}
	pos.Y = b + 1; pos.X = a + 1;
	return pos;
}
/*----------------------------------------------------------------------*/
void Undo(GCG *pcg){/*悔棋函数*/
	if (Top <= 1) return;/*如果已经落子的数量小于一则不能再进行悔棋操作*/
	point lastpoint;
	if (Top > 0){
		/*用之前保存的图像覆盖悔棋点图像*/
		lastpoint = PointNode[Top].pos;
		putimage((lastpoint.X - 1) * SIDE_LEN - RADIUS, (lastpoint.Y - 1) * SIDE_LEN - RADIUS, &(PointNode[Top].image));
		/*还原棋型数组的同时将栈指针Top下移*/
		PatternAnalysis(pcg->table, pcg->PatternNum, PointNode[Top].pos.Y, PointNode[Top].pos.X, UNDO);
		pcg->table[PointNode[Top].pos.Y][PointNode[Top].pos.X] = NOSTONE;
		Top--;

	}
	if (Top > 0){
		/*用之前保存的图像覆盖悔棋点图像*/
		lastpoint = PointNode[Top].pos;
		putimage((lastpoint.X - 1) * SIDE_LEN - RADIUS, (lastpoint.Y - 1) * SIDE_LEN - RADIUS, &(PointNode[Top].image));
		/*还原棋型数组的同时将栈指针Top下移*/
		PatternAnalysis(pcg->table, pcg->PatternNum, PointNode[Top].pos.Y, PointNode[Top].pos.X, UNDO);
		pcg->table[PointNode[Top].pos.Y][PointNode[Top].pos.X] = NOSTONE;
		Top--;
	}
}
/*----------------------------------------------------------------------*/
void NewGame(GCG *pcg, int *pwho){
	int i, j;
	/*清除栈空间*/
	Top = 0;
	/*调用开始函数*/
	*pwho = Start();
	/*用之前保存的游戏初始界面清除当前界面开始新的游戏*/
	putimage(0 - RADIUS, 0 - RADIUS, &initpicture);
	/*初始化棋盘数组*/
	for (i = 1; i <= LEN; i++){
		for (j = 1; j <= LEN; j++){
			pcg->table[i][j] = NOSTONE;
		}
	}
	/*初始化棋型数组*/
	for (i = 0; i <= 2; i++){
		for (j = 0; j <= 19; j++){
			pcg->PatternNum[i][j] = 0;
		}
	}
}
/*----------------------------------------------------------------------*/
void Fail(void){/*退出游戏函数*/
	closegraph();
	exit(0);
}
/*----------------------------------------------------------------------*/
void Help(void){
	/*定义图形对象保存将要输出说明信息的区域*/
	IMAGE image;
	RECT r1 = { 1 * SIDE_LEN, 2 * SIDE_LEN, SIDE_LEN * (LEN - 2), SIDE_LEN * (LEN - 3) };
	getimage(&image, r1.left, r1.top, r1.right - r1.left, r1.bottom - r1.top);
	/*绘制填充矩形清除信息输出区域，设置文字背景模式，设置字体样式*/
	fillrectangle(1 * SIDE_LEN, 2 * SIDE_LEN, SIDE_LEN * (LEN - 2), SIDE_LEN * (LEN - 3));
	setbkmode(OPAQUE);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 18;
//	_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	/*输出程序说明文字*/
	drawtext(_T("\n\n    本五子棋程序采用非禁手规则编\n\n写，只要谁先在四个方向上形成五连\n\n或大于五子的长连即获胜。\n\n     （点击右键继续游戏）"), &r1, DT_WORDBREAK);
	/*还原文字背景模式，还原字体样式*/
	setbkmode(TRANSPARENT);
	f.lfHeight = 26;
	settextstyle(&f);
	/*点击鼠标右键继续游戏*/
	MOUSEMSG m;
	while (true){
		m = GetMouseMsg();
		if (m.uMsg == WM_RBUTTONUP){
			break;
		}
	}
	/*还原游戏界面*/
	putimage(r1.left, r1.top, &image);
}
/*----------------------------------------------------------------------*/
void Down(GCG *pcg, int who, point lastpoint){/*落子函数*/
	/*栈指针后移，将落子点坐标及图像保存在静态栈内*/
	Top++;
	PointNode[Top].pos = lastpoint;
	getimage(&(PointNode[Top].image), (lastpoint.X - 1) * SIDE_LEN - RADIUS, (lastpoint.Y - 1) * SIDE_LEN - RADIUS, 2 * RADIUS + 1, 2 * RADIUS + 1);
	/*更新棋盘数组，更新棋型数组，绘制棋子*/
	pcg->table[lastpoint.Y][lastpoint.X] = who;
	PatternAnalysis(pcg->table, pcg->PatternNum, lastpoint.Y, lastpoint.X, DO);
	who == BLACK_PLAYER ? setfillcolor(BLACK) : setfillcolor(WHITE);;
	fillcircle((lastpoint.X - 1) * SIDE_LEN, (lastpoint.Y - 1) * SIDE_LEN, RADIUS);
}
/*----------------------------------------------------------------------*/
int IsOver(int table[][LEN + 1], point pos){/*游戏结束判断函数*/
	int a = pos.Y, b = pos.X;
	int mod = table[a][b];
	int start, end;
	/*判断横向是否有五连*/
	for (start = b; start >= 1; start--){
		if (table[a][start] != mod){
			break;
		}
	}
	for (end = b; end <= LEN; end++){
		if (table[a][end] != mod){
			break;
		}
	}
	if (end - start - 1 >= 5) return 1;
	/*判断竖向是否有五连*/
	for (start = a; start >= 1; start--){
		if (table[start][b] != mod){
			break;
		}
	}
	for (end = a; end <= LEN; end++){
		if (table[end][b] != mod){
			break;
		}
	}
	if (end - start - 1 >= 5) return 1;
	/*判断左斜是否有五连*/
	for (start = 0; a + start <= LEN && b - start >= 1; start++){
		if (table[a + start][b - start] != mod){
			break;
		}
	}
	for (end = 0; a - end >= 1 && b + end <= LEN; end++){
		if (table[a - end][b + end] != mod){
			break;
		}
	}
	if (start + end - 1 >= 5) return 1;
	/*判断右斜是否有五连*/
	for (start = 0; a - start >= 1 && b - start >= 1; start++){
		if (table[a - start][b - start] != mod){
			break;
		}
	}
	for (end = 0; a + end <= LEN && b + end <= LEN; end++){
		if (table[a + end][b + end] != mod){
			break;
		}
	}
	if (start + end - 1 >= 5) return 1;
	return 0;
}
/*----------------------------------------------------------------------*/
int Winner(int who){/*输出输赢*/
	/*显示提示信息*/
	RECT r1 = { 1 * SIDE_LEN, 2 * SIDE_LEN, SIDE_LEN * (LEN - 2), SIDE_LEN * (LEN - 3) };
	setbkmode(OPAQUE);
	if (who == BLACK_PLAYER){
		drawtext(_T("\n\n\n\n   GameOver玩家获胜   \n (右键退出、左键继续) "), &r1, NULL);
	}
	else{
		drawtext(_T("\n\n\n\n   GameOver电脑获胜   \n (右键退出、左键继续) "), &r1, NULL);
	}
	setbkmode(TRANSPARENT);
	/*鼠标消息*/
	MOUSEMSG m;
	while (true){
		m = GetMouseMsg();
		if (m.uMsg == WM_RBUTTONUP){/*右键弹起游戏结束*/
			closegraph();
			return QUIT;
		}
		if (m.uMsg == WM_LBUTTONUP){/*左键弹起继续游戏*/
			return CONTINUE;
		}
	}
}
/*---------------------------------END----------------------------------*/
