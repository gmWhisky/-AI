# -AI
程序模拟的是整个下棋的过程，结构体类型 GCG用于保存当前比赛的棋局状态，由两个数组构成，一个是表示棋盘落子状态的数组table,另一个是用于保存玩家和电脑棋型的二维数组PatternNum。程序运行时由玩家，电脑交替进行下棋，在每次落子之后都会更新结构体变量cg中对应的棋盘数组和棋型数组，这一过程和真实的下棋过程是十分相似的。
在落子点的产生上，玩家落子实现起来并无难度，只要玩家自己选择的落子点无子即可，而对于电脑下棋点的产生就需要借助于试下和静态估值函数。
静态估值函数，是与游戏本身关系最为紧密的部分，它根据电脑和玩家的各种棋型给彼此打分，最后用电脑的得分减去玩家的得分得到静态估值函数的返回值。很容易看出，该函数是站在电脑的角度上编写的，即返回值为正时表示对电脑有利，且数值越大优势越大。反之，如果返回值为负则表示对电脑不利，且数值越小劣势越大。
试下分两步进行，第一步是电脑先在所有可落子点上试下，与此同时调用静态估值函数找到估值最大的落子点，随后检测该点的估值是否超过某一上限，若超过则表示下在该点电脑可获胜，若不是则进入第二步，让玩家依次在可落子点上试下与此同时记录下所有小于某一估值的落子点，如果这样的点不存在则返回起上次黑棋试下时估值最大的点，若存在，则找出所有这样的点中对电脑最有力的落子点。
输赢判断函数的实现相对简单，只要在当前落子点四个方向上进行判断即可。
在下棋的过程中我还用到了一个静态栈PointNode用于保存每次落子的坐标以及对应的图像，以备悔棋时使用。
**详细设计**
1.数据结构
	typedef struct Point{
		int X, Y;
	}point;
/*该结构体用于保存坐标*/
	typedef struct GobangChessGame{
		int table[LEN + 1][LEN + 1];
		int PatternNum[3][20];
	}GCG;
/*该结构体用于表示五子棋当前棋局状态，由棋盘数组和棋型数组构成*/
	struct{
		point pos;
		IMAGE image;
	}PointNode[LEN * LEN + 1];
	int Top = 0;
/*采用静态栈记录已经落子的坐标以及对应的图形，栈顶指针置为零*/
	IMAGE initpicture;
/*定义图形对象用于存储初始化游戏界面已备重新开始游戏时重绘开始界面*/
	int posvalue[LEN + 1][LEN + 1] 
/*全局变量用以表示棋盘各落子点的权重*/
	int patternB[][20]
/*定义黑棋的各种棋型*/
	int patternW[][10] 
/*定义白棋的各种棋型*/
2.函数说明
	void Init(GCG *pcg);/*该函数为初始化函数，实现功能为：绘制游戏界面、初始化棋盘数组、初始化棋型数组*/
	int Start(void);/*该函数为游戏开始函数，该函数实现功能为：确定谁为先手并将结果返回*/
	point Computer(GCG cg);/*返回电脑落子点函数，参数cg接收当前棋局状态，功能为：返回电脑最佳落子点 */
	int Estimate(GCG cg, int a, int b, int BLACK_WHITE);/*局面静态估值函数，该函数的参数为当前棋局状态，a,b为落子点，BLACK_WHITE保存的是下一棋轮到谁下，函数功能：返回局面估值*/
	void PatternAnalysis(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*棋型分析函数，参数table为棋盘数组，表示棋盘落子状态，PatternNum为棋型数组记录玩家和电脑棋型，参数a,b表示下棋点，DO_UNDO记录是执行悔棋操作还是下棋操作。函数功能:更新棋型数组*/
	void Horizontal(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*横向棋型分析函数，函数功能：更新横向棋型数组*/
	void Vertical(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*竖向棋型分析函数，函数功能：更新竖向棋型数组*/
	void LeftSlant(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*左斜向棋型分析函数，函数功能：更新左斜向棋型数组*/
	void RightSlant(int table[][LEN + 1], int PatternNum[][20], int a, int b, int DO_UNDO);/*右斜向棋型分析函数，函数功能：更新右斜向棋型数组*/
	int Pattern(int BLACK_WHITE, int *T);/*棋型匹配函数，函数功能：确定棋型*/
	int Index(int *S, int *T);/*匹配函数*/
	point Player(int table[][LEN + 1], int *pchoice);/*玩家下棋，函数功能：返回玩家落子点，同时带回按钮值*/
	void Undo(GCG *pcg);/*悔棋函数*/
	void NewGame(GCG *pcg, int *pwho);/*新游戏，函数功能：重置棋局状态开始新游戏*/
	void Fail(void);/*认输，游戏结束退出程序*/
	void Help(void);/*游戏说明*/
	void Down(GCG *pcg, int who, point lastpoint);/*落子函数，函数功能：实现落子，同时更新棋型表*/
	int IsOver(int table[][LEN + 1], point pos);/*游戏结束判断函数，分析pos点四个方向上是否有五连或者长连，函数功能：判断游戏是否结束*/
	int Winner(int who);/*该函数显示胜负*/
