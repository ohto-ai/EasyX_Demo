///////////////////////////////////////////////////
// 程序名称：军旗
// 编译环境：Visual C++ 6.0，EasyX_20130506(beta)
// 作　　者：shareli，QQ：953445224
// 最后修改：2012-2-12
//
#include <graphics.h>
#include <time.h>

#define	CHESIZE	40		// 棋盘尺寸，不能随意调整
#define	RESETX	170
#define	RESETY	350		// 重置原点

typedef enum			// 要用到的棋子ID
{
	si, jun, shi, lv, tuan,
	ying, lian, pai, ban, gong,
	fei, chao, zha, qi, lei, bian,
	xian, sheng, shen
}CHESSID;

typedef enum			// 攻击类型
{
	comatt, preatt, noatt
}ATTSTYLE;

typedef enum			// 当前游戏方和棋子所属方
{
	blue, red, white
}TEAM;

typedef enum			// 选中与未选中
{
	alchoose, unchoose
}CHOOSESTATE;

typedef enum			// 区域状态
{
	unknow, empty, exist
}STATE;

typedef struct			// 坐标
{
	int x;
	int y;
}COOR;

typedef struct			// 棋子
{
	CHESSID    id;		// 棋子的ID
	int        power;	// 棋子的等级
	TEAM       team;	// 所属方
	const char* image;	// 该棋子的图片，考虑到运行问题，本程序用字代替
	int        scoopc;	// 工兵是挖到的地雷数
}CHESS;

typedef struct			// 区域
{
	COOR crdld;			// 区域的左下坐标
	CHESS chess;		// 区域的棋子
	STATE  state;		// 区域状态
}AREA;

typedef struct			// 用户的选择信息
{
	int i;
	int j;
	CHOOSESTATE state;	// 选择状态
}CHOOSE;

IMAGE image;
AREA area[6][6];		// 定义棋盘大小
CHESS datachess[19];	// 几种基本棋子类型
CHOOSE choose;			// 用户选择信息
MOUSEMSG mmsg;			// 鼠标信息
TEAM  user;				// 执棋方
int lockchessboard = 0;	// 是否锁定棋盘
int i;					// 当前鼠标所在区域的坐标
int j;
const char* str[] = { "工","班","排","连","营","团","旅","师","军","司","棋","炸","变","雷","飞","超","升","神","仙" };

void init();
void initchessbute();	// 给初始化棋子基本参数
void initvalue();
void drawboard();		// 画棋盘
void randomarr(int*);	// 实现棋的随机排列
void judge();
void getpreij();		// 获得当前鼠标所在区域坐标
int  checkij();			// 检查当鼠标所在区域
void open();			// 打开所在区域
int  whemove();			// 判断是否能移动
void move();			// 移动
int  judgeunknow();		// 检测当前未翻开棋子数
ATTSTYLE  wheattack();	// 判断是否能攻击
void kill();			// 杀死当前选择的棋
void killself();		// 自杀
void perishtogether();	// 同归于尽
void getteam();			// 用作改变棋子类型时，对棋子所属方赋值
void userchange();		// 交换执棋方
void judgebunko();		// 判断输赢
void choosearea();		// 选定区域
void cancelchoose();	// 取消选定
void change();			// 变身
void bluewin();			// 蓝方胜利
void redwin();			// 红方胜利
void gamehelp();		// 规则说明
void quit();			// 退出游戏
void peace();			// 和棋
void surrender();		// 投降
void resetchessboard();	// 重置

// 下面几个函数为判断棋子的攻击类型
ATTSTYLE judgegong();	// 判断工兵
ATTSTYLE judgecom();	// 判普通人物
ATTSTYLE judgezha();	// 判断炸弹

int main()				// 主函数
{
	init();

	while (true)
	{
		mmsg = GetMouseMsg();
		getpreij();

		if (mmsg.uMsg == WM_LBUTTONDOWN)   //单击左键
		{
			judge();
		}
		else if (mmsg.uMsg == WM_RBUTTONDOWN
			&& choose.state == alchoose) //单击右键
		{
			cancelchoose();
		}
		else if (mmsg.uMsg == WM_MBUTTONDOWN
			&& choose.state == alchoose
			&& area[choose.i][choose.j].chess.id != zha) //单击中键
		{
			killself();
			cancelchoose();
			userchange();
			judgebunko();
		}
	}
	return 0;
}


void init()
{
	initgraph(640, 480);

	setorigin(RESETX, RESETY);		// 重置原点
	setaspectratio(1, -1);			// 把 y 轴上方设为正半轴

	drawboard();
	initvalue();

}

void drawboard()					// 画棋盘
{
	int i1;

	setlinecolor(WHITE);
	for (i1 = 0; i1 < 7; i1++)
	{
		line(i1 * CHESIZE, 0, i1 * CHESIZE, CHESIZE * 6);
	}

	for (i1 = 0; i1 < 7; i1++)
	{
		line(0, i1 * CHESIZE, CHESIZE * 6, i1 * CHESIZE);
	}


	setlinecolor(WHITE);
	setfillcolor(RED);
	rectangle(-10, -10, CHESIZE * 6 + 10, CHESIZE * 6 + 10);
	floodfill(-1, -1, WHITE);

	rectangle(7 * CHESIZE, CHESIZE, 9 * CHESIZE, 6 * CHESIZE);
	line(7 * CHESIZE, 5 * CHESIZE, 9 * CHESIZE, 5 * CHESIZE);
	line(7 * CHESIZE, 4 * CHESIZE, 9 * CHESIZE, 4 * CHESIZE);
	line(7 * CHESIZE, 3 * CHESIZE, 9 * CHESIZE, 3 * CHESIZE);
	line(7 * CHESIZE, 2 * CHESIZE, 9 * CHESIZE, 2 * CHESIZE);
	setaspectratio(1, 1);
	settextstyle(35, 18, "黑体");
	settextcolor(RED);
	outtextxy(7 * CHESIZE + 2, -6 * CHESIZE + 2, "帮助");
	settextcolor(BROWN);
	outtextxy(7 * CHESIZE + 2, -5 * CHESIZE + 2, "投降");
	settextcolor(GREEN);
	outtextxy(7 * CHESIZE + 2, -4 * CHESIZE + 2, "和棋");
	settextcolor(YELLOW);
	outtextxy(7 * CHESIZE + 2, -3 * CHESIZE + 2, "重置");
	settextcolor(CYAN);
	outtextxy(7 * CHESIZE + 2, -2 * CHESIZE + 2, "退出");

	settextcolor(LIGHTMAGENTA);
	settextstyle(50, 20, "黑体");
	outtextxy(CHESIZE, -CHESIZE * 8, "两国军旗");

	setaspectratio(1, -1);
}

void initchessbute()				// 设置棋子基本参数
{
	datachess[0].id = gong;
	datachess[0].power = 1;
	datachess[0].image = str[0];
	datachess[0].scoopc = 0;

	datachess[1].id = ban;
	datachess[1].power = 2;
	datachess[1].image = str[1];
	datachess[1].scoopc = 0;

	datachess[2].id = pai;
	datachess[2].power = 3;
	datachess[2].image = str[2];
	datachess[2].scoopc = 0;

	datachess[3].id = lian;
	datachess[3].power = 4;
	datachess[3].image = str[3];
	datachess[3].scoopc = 0;

	datachess[4].id = ying;
	datachess[4].power = 5;
	datachess[4].image = str[4];
	datachess[4].scoopc = 0;

	datachess[5].id = tuan;
	datachess[5].power = 6;
	datachess[5].image = str[5];
	datachess[5].scoopc = 0;

	datachess[6].id = lv;
	datachess[6].power = 7;
	datachess[6].image = str[6];
	datachess[6].scoopc = 0;

	datachess[7].id = shi;
	datachess[7].power = 8;
	datachess[7].image = str[7];
	datachess[7].scoopc = 0;

	datachess[8].id = jun;
	datachess[8].power = 9;
	datachess[8].image = str[8];
	datachess[8].scoopc = 0;

	datachess[9].id = si;
	datachess[9].power = 10;
	datachess[9].image = str[9];
	datachess[9].scoopc = 0;

	datachess[10].id = qi;
	datachess[10].power = 100;
	datachess[10].image = str[10];
	datachess[10].scoopc = 0;

	datachess[11].id = zha;
	datachess[11].power = 99;
	datachess[11].image = str[11];
	datachess[11].scoopc = 0;

	datachess[12].id = bian;
	datachess[12].power = 0;
	datachess[12].image = str[12];
	datachess[12].scoopc = 0;

	datachess[13].id = lei;
	datachess[13].power = 98;
	datachess[13].image = str[13];
	datachess[13].scoopc = 0;

	datachess[14].id = fei;
	datachess[14].power = 9;
	datachess[14].image = str[14];
	datachess[14].scoopc = 0;

	datachess[15].id = chao;
	datachess[15].power = 11;
	datachess[15].image = str[15];
	datachess[15].scoopc = 0;

	datachess[16].id = sheng;
	datachess[16].power = 10;
	datachess[16].image = str[16];
	datachess[16].scoopc = 0;

	datachess[17].id = shen;
	datachess[17].power = 11;
	datachess[17].image = str[17];
	datachess[17].scoopc = 0;

	datachess[18].id = xian;
	datachess[18].power = 11;
	datachess[18].image = str[18];
	datachess[18].scoopc = 0;
}

void initvalue()					// 初始化值
{
	CHESS chess[36];
	int random[36];
	int count;
	int i1, j1;

	initchessbute();

	randomarr(random);

	for (i1 = 0; i1 <= 11; i1++)
	{
		chess[i1] = datachess[i1];
		chess[i1].team = red;
	}
	chess[i1] = datachess[11];
	chess[i1].team = red;
	chess[i1 + 1] = datachess[0];
	chess[i1 + 1].team = red;

	for (i1 = 0; i1 <= 11; i1++)
	{
		chess[i1 + 14] = datachess[i1];
		chess[i1 + 14].team = blue;
	}
	chess[i1 + 14] = datachess[11];
	chess[i1 + 14].team = blue;
	chess[i1 + 15] = datachess[0];
	chess[i1 + 15].team = blue;

	for (i1 = 0; i1 < 4; i1++)
	{
		chess[i1 + 28] = datachess[12];
		chess[i1 + 28].team = white;
		chess[i1 + 32] = datachess[13];
		chess[i1 + 32].team = white;
	}

	setfillcolor(YELLOW);
	for (count = 0, i1 = 0; i1 < 6; i1++)
	{
		for (j1 = 0; j1 < 6; j1++, count++)
		{
			area[i1][j1].chess = chess[random[count]];
			area[i1][j1].crdld.x = i1 * CHESIZE + 1;
			area[i1][j1].crdld.y = j1 * CHESIZE + 1;
			area[i1][j1].state = unknow;
			floodfill(area[i1][j1].crdld.x, area[i1][j1].crdld.y, WHITE);
		}
	}
	user = red;
	choose.state = unchoose;

}

void randomarr(int random[])		// 得到0~36数字的随机排列
{
	int i1, j1;
	int flag = 0;

	srand(time(NULL));
	random[0] = rand() % 36;

	for (i1 = 1; i1 < 36; i1++)
	{
		while (1)
		{
			random[i1] = rand() % 36;
			for (j1 = 0; j1 < i1; j1++)
			{
				if (random[j1] == random[i1])
				{
					flag = 1;
					break;
				}
			}
			if (flag)
			{
				flag = 0;
			}
			else
			{
				break;
			}
		}
	}
}

void judge()						// 判断当前要进行的操作
{
	ATTSTYLE  attstyle;				// 攻击类型

	getpreij();
	if (checkij())
	{
		if (area[i][j].state == unknow && choose.state == unchoose)	// 打开
		{
			open();
			userchange();
		}
		else if (area[i][j].state == empty)
		{
			if (choose.state == alchoose)						// 移动
			{
				if (whemove())
				{
					move();
					cancelchoose();
					userchange();
				}
			}
		}
		else
		{
			if (choose.state == unchoose)
			{
				if (area[i][j].chess.team == user && area[i][j].chess.id != qi)  //选定  
				{
					choosearea();
				}
			}
			else
			{
				if (area[i][j].state != unknow)		// 攻击
				{
					attstyle = wheattack();
					if (attstyle == comatt)
					{
						kill();
						cancelchoose();
						userchange();
					}
					else if (attstyle == preatt)
					{
						perishtogether();
						cancelchoose();
						userchange();
					}
					else
					{
						;
					}
				}
			}
		}

		if (!judgeunknow())		// 在所有棋子都翻开的情况下判断输赢
		{
			judgebunko();
		}
	}
}

int  judgeunknow()
{
	int i1, i2;
	int num = 0;

	for (i1 = 0; i1 < 6; i1++)
	{
		for (i2 = 0; i2 < 6; i2++)
		{
			if (area[i1][i2].state == unknow)
			{
				num++;
			}
		}
	}

	return num;
}

// 选择区域
void choosearea()
{
	choose.i = i;
	choose.j = j;
	choose.state = alchoose;

	setlinecolor(GREEN);
	rectangle(choose.i * CHESIZE, choose.j * CHESIZE, choose.i * CHESIZE + CHESIZE, choose.j * CHESIZE + CHESIZE);
}

// 取消选定
void cancelchoose()
{
	setlinecolor(WHITE);
	rectangle(choose.i * CHESIZE, choose.j * CHESIZE, choose.i * CHESIZE + CHESIZE, choose.j * CHESIZE + CHESIZE);
	choose.state = unchoose;
}

// 当前鼠标所在区域
void getpreij()
{
	i = (mmsg.x - RESETX) / CHESIZE;
	j = -(mmsg.y - RESETY) / CHESIZE;
}

// 检查鼠标是否在有效区域内
int  checkij()
{
	if ((i == 7 || i == 8) && j == 5)
	{
		gamehelp();
		return 0;
	}
	else if ((i == 7 || i == 8) && j == 4)
	{
		if (!lockchessboard)
		{
			surrender();
		}
		return 0;
	}
	else if ((i == 7 || i == 8) && j == 3)
	{
		if (!lockchessboard)
		{
			peace();
		}
		return 0;
	}
	else if ((i == 7 || i == 8) && j == 2)
	{
		resetchessboard();
		lockchessboard = 0;
		return 0;
	}
	else if ((i == 7 || i == 8) && j == 1)
	{
		quit();
		return 0;
	}
	else
	{
		if (!lockchessboard)
		{
			if ((i >= 0 && i <= 5 && j >= 0 && j <= 5 && (mmsg.x - RESETX) > 0 && -(mmsg.y - RESETY) > 0))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
}

// 打开操作
void open()
{
	setfillcolor(BLACK);
	floodfill(area[i][j].crdld.x, area[i][j].crdld.y, WHITE);

	setaspectratio(1, 1);
	if (area[i][j].chess.team == blue)
	{
		settextcolor(BLUE);
	}
	else if (area[i][j].chess.team == red)
	{
		settextcolor(RED);
	}
	else
	{
		settextcolor(MAGENTA);
	}
	settextstyle(35, 18, "黑体");
	outtextxy(area[i][j].crdld.x, -area[i][j].crdld.y - CHESIZE + 2, area[i][j].chess.image);
	area[i][j].state = exist;
	setaspectratio(1, -1);
}

// 判断是否能移动
int whemove()
{
	if (area[choose.i][choose.j].chess.id == fei || area[choose.i][choose.j].chess.id == sheng
		|| area[choose.i][choose.j].chess.id == shen)
	{
		if (choose.i == i && abs(choose.j - j) <= 5 || choose.j == j && abs(choose.i - i) <= 5)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (area[choose.i][choose.j].chess.id == xian)
	{
		return 1;
	}
	else
	{
		if (choose.i == i && abs(choose.j - j) == 1 || choose.j == j && abs(choose.i - i) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

// 移动
void move()
{
	setfillcolor(BLACK);
	floodfill(area[choose.i][choose.j].crdld.x, area[choose.i][choose.j].crdld.y, GREEN);

	setaspectratio(1, 1);
	if (area[choose.i][choose.j].chess.id == gong && area[choose.i][choose.j].chess.scoopc > 0)
	{
		if (area[choose.i][choose.j].chess.team == blue)
		{
			settextcolor(LIGHTBLUE);
		}
		else
		{
			settextcolor(LIGHTRED);
		}
	}
	else
	{
		if (user == blue)
		{
			settextcolor(BLUE);
		}
		else
		{
			settextcolor(RED);
		}
	}
	settextstyle(35, 18, "黑体");
	outtextxy(area[i][j].crdld.x, -area[i][j].crdld.y - CHESIZE + 2, area[choose.i][choose.j].chess.image);

	area[choose.i][choose.j].state = empty;
	area[i][j].state = exist;
	area[i][j].chess = area[choose.i][choose.j].chess;
	setaspectratio(1, -1);
}

// 判断是否能攻击，并返回攻击类型
ATTSTYLE  wheattack()
{
	if (whemove())
	{
		if (area[choose.i][choose.j].chess.id == gong)
		{
			return judgegong();
		}
		else if (area[choose.i][choose.j].chess.id == zha)
		{
			return judgezha();
		}
		else
		{
			return judgecom();
		}
	}
	else
	{
		return noatt;
	}

}

// 判断工兵
ATTSTYLE judgegong()
{
	if (area[i][j].chess.team != white)
	{
		if (area[choose.i][choose.j].chess.team != area[i][j].chess.team)
		{
			if (area[i][j].chess.id == gong || area[i][j].chess.id == zha)
			{
				return preatt;
			}
			else if (area[i][j].chess.id == qi)
			{
				if (area[choose.i][choose.j].chess.scoopc == 0)
				{
					return noatt;
				}
				else if (area[choose.i][choose.j].chess.scoopc == 1)
				{
					area[choose.i][choose.j].chess = datachess[14];
					getteam();
					return comatt;
				}
				else if (area[choose.i][choose.j].chess.scoopc == 2)
				{
					area[choose.i][choose.j].chess = datachess[16];
					getteam();
					return comatt;
				}
				else if (area[choose.i][choose.j].chess.scoopc == 3)
				{
					area[choose.i][choose.j].chess = datachess[17];
					getteam();
					return comatt;
				}
				else
				{
					area[choose.i][choose.j].chess = datachess[18];
					getteam();
					return comatt;
				}
			}
			else
			{
				return noatt;
			}
		}
		else
		{
			return noatt;
		}
	}
	else
	{
		if (area[i][j].chess.id == lei)
		{
			area[choose.i][choose.j].chess.scoopc++;
			return comatt;
		}
		else
		{
			change();
			return comatt;
		}
	}
}

// 判断炸弹
ATTSTYLE judgezha()
{
	if (area[choose.i][choose.j].chess.team != area[i][j].chess.team)
	{
		if (area[i][j].chess.id != qi)
		{
			return preatt;
		}
		else
		{
			return noatt;
		}
	}
	else
	{
		return noatt;
	}
}

// 判断普通人物  
ATTSTYLE judgecom()
{
	if (area[i][j].chess.team != white)
	{
		if (area[choose.i][choose.j].chess.team != area[i][j].chess.team)
		{
			if (area[choose.i][choose.j].chess.power == area[i][j].chess.power || area[i][j].chess.id == zha)
			{
				return preatt;
			}
			else if (area[choose.i][choose.j].chess.power > area[i][j].chess.power)
			{
				return comatt;
			}
			else
			{
				return noatt;
			}
		}
		else
		{
			return noatt;
		}
	}
	else
	{
		if (area[i][j].chess.id == lei)
		{
			return noatt;
		}
		else
		{
			change();
			return comatt;
		}
	}
}

// 变身
void change()
{
	int x;
	x = rand() % 50;
	if (x == 6)
	{
		area[choose.i][choose.j].chess = datachess[15];
		getteam();
	}
	else
	{
		x = rand() % 4;
		if (x == 3)
		{
			x = rand() % 2;
			if (x == 0)
			{
				area[choose.i][choose.j].chess = datachess[7];
			}
			else
			{
				area[choose.i][choose.j].chess = datachess[8];
			}

			getteam();
		}
		else
		{
			x = rand() % 6;
			area[choose.i][choose.j].chess = datachess[x];

			getteam();
		}
	}
}

// 对棋子所属方赋值
void getteam()
{
	if (user == blue)
	{
		area[choose.i][choose.j].chess.team = blue;
	}
	else
	{
		area[choose.i][choose.j].chess.team = red;
	}
}

// 杀死对方
void kill()
{
	move();
}

// 自杀
void killself()
{
	setfillcolor(BLACK);
	floodfill(area[choose.i][choose.j].crdld.x, area[choose.i][choose.j].crdld.y, GREEN);
	area[choose.i][choose.j].state = empty;
}

// 同归于尽
void perishtogether()
{
	setfillcolor(BLACK);
	cancelchoose();
	floodfill(area[choose.i][choose.j].crdld.x, area[choose.i][choose.j].crdld.y, WHITE);
	floodfill(area[i][j].crdld.x, area[i][j].crdld.y, WHITE);

	area[choose.i][choose.j].state = empty;
	area[i][j].state = empty;
}

// 切换执棋方
void userchange()
{
	if (user == blue)
	{
		user = red;
		setfillcolor(RED);
		floodfill(-1, -1, WHITE);
	}
	else
	{
		user = blue;
		setfillcolor(BLUE);
		floodfill(-1, -1, WHITE);
	}
}

// 判断输赢
void judgebunko()
{
	int i1, j1;
	int num1 = 0, num2 = 0;

	for (i1 = 0; i1 < 6; i1++)
	{
		for (j1 = 0; j1 < 6; j1++)
		{
			if (area[i1][j1].state != empty)
			{
				if (area[i1][j1].chess.team == red && area[i1][j1].chess.id != qi)
				{
					num1++;
				}
				else if (area[i1][j1].chess.team == blue && area[i1][j1].chess.id != qi)
				{
					num2++;
				}
			}
		}
	}

	if (num1 == 0 && num2 != 0)
	{
		bluewin();
	}

	if (num2 == 0 && num1 != 0)
	{
		redwin();
	}

	if (num1 == 0 && num2 == 0)
	{
		peace();
	}
}

// 蓝方胜
void bluewin()
{
	setaspectratio(1, 1);
	settextcolor(BLUE);
	settextstyle(50, 20, "黑体");
	outtextxy(CHESIZE, -CHESIZE * 8, "蓝方胜利");
	setaspectratio(1, -1);
	setfillcolor(BLUE);
	floodfill(-1, -1, WHITE);

	lockchessboard = 1;              //锁定棋盘
}

// 红方胜
void redwin()
{
	setaspectratio(1, 1);
	settextcolor(RED);
	settextstyle(50, 20, "黑体");
	outtextxy(CHESIZE, -CHESIZE * 8, "红方胜利");
	setaspectratio(1, -1);
	setfillcolor(RED);
	floodfill(-1, -1, WHITE);

	lockchessboard = 1;
}

// 和棋
void peace()
{
	setaspectratio(1, 1);
	settextcolor(GREEN);
	settextstyle(50, 20, "黑体");
	outtextxy(CHESIZE, -CHESIZE * 8, "握手言和");
	setaspectratio(1, -1);
	setfillcolor(GREEN);
	floodfill(-1, -1, WHITE);

	lockchessboard = 1;
}

// 投降
void surrender()
{
	if (user == blue)
	{
		redwin();
	}
	else
	{
		bluewin();
	}
}

// 重置
void resetchessboard()
{
	cleardevice();
	init();
}

// 游戏说明
void gamehelp()
{
	getimage(&image, -10, -10, 500, 350);
	cleardevice();

	setorigin(50, 0);
	setaspectratio(1, 1);

	settextcolor(RED);
	settextstyle(14, 0, "黑体");
	outtextxy(-50, 0, "注:单击鼠标左键回到游戏界面");

	settextcolor(WHITE);
	settextstyle(24, 0, "黑体");
	outtextxy(230, 5, "游戏说明");
	settextstyle(12, 0, "宋体");
	outtextxy(0, 35, "棋盘大小:6*6;              棋子总数:36;            敌对双方:红,蓝");
	outtextxy(0, 60, "棋子类别:红棋(红方操作,14个) 蓝棋(蓝方操作,14个) 紫棋(功能棋,8个)");
	outtextxy(0, 85, "红棋(蓝棋)类型:司令,军长,师长,旅长,团长,营长,连长,班长,军旗,工兵*2,炸弹*2.");
	outtextxy(0, 100, "紫棋类型:地雷*4,变身棋*4.  注:'*'后面表示该棋的数量,没注则只有一个");
	outtextxy(0, 125, "规则说明:1.司令最大,工兵最小,大的吃小的,一样就同归于尽,");
	outtextxy(textwidth("规则说明:1."), 140, "炸弹能炸紫棋和敌方除军旗外所有的棋(炸弹也会消失).");
	outtextxy(textwidth("规则说明:"), 155, "2.工兵可挖地雷,挖完后可扛对方棋变身(挖的雷越多,变成的人物越厉害).");
	outtextxy(textwidth("规则说明:"), 170, "3.人物棋可吃变,吃后能变成工兵~军长中的一种,有一定几率变成隐藏BOSS.");
	outtextxy(textwidth("规则说明:"), 185, "4.人物棋可自杀(算一次操作).");
	outtextxy(textwidth("规则说明:"), 200, "5.执棋方进行完一次有效操作后,就换对方执棋(边框颜色表当前执棋方).");
	outtextxy(textwidth("规则说明:"), 215, "6.一方棋子(军旗除外)全被消灭,就算输; 同时全部没有,则和棋.");
	outtextxy(0, 240, "执棋方能进行的操作:操作1:打开棋子(算一次操作).");
	outtextxy(textwidth("执棋方能进行的操作:"), 255, "操作2:攻击.");
	outtextxy(textwidth("执棋方能进行的操作:"), 270, "操作3:移动.");
	outtextxy(textwidth("执棋方能进行的操作:"), 285, "操作4:工兵(已挖雷)扛旗.");
	outtextxy(textwidth("执棋方能进行的操作:"), 300, "操作5:吃变身卡.");
	outtextxy(textwidth("执棋方能进行的操作:"), 315, "操作6:自杀.");
	outtextxy(0, 340, "实施游戏操作说明(鼠标操作):实施操作1:选择要打开棋子所在的区域,单击.");
	outtextxy(textwidth("实施游戏操作说明(鼠标操作):"), 355, "实施操作2~5:单击选中主动方(棋子边框会变绿)");
	outtextxy(textwidth("实施游戏操作说明(鼠标操作):实施操作2~5:"), 370, "再单击选中被动方.");
	outtextxy(textwidth("实施游戏操作说明(鼠标操作):"), 385, "实施操作6:选中己方棋子,单机鼠标的中键.");
	settextcolor(RED);
	outtextxy(textwidth("实施游戏操作说明(鼠标操作):"), 400, "注:要进行其他操作,必先撤销当前选定(单击右键撤销)");
	settextcolor(WHITE);
	setlinecolor(WHITE);
	line(-30, 420, 570, 420);
	outtextxy(0, 425, "人物棋等级一览(等高杀等小):工1 班2 连3 营4 团5 旅6 师7");
	outtextxy(textwidth("人物棋等级一览(等高杀等小):"), 440, "军8 飞8 司9 升9 神10 仙10");
	outtextxy(0, 455, "注:'飞' '升' '神' '仙' 都为工兵挖雷后扛旗所变,'飞''升''神'能直线飞,'仙'能满天飞");

	while (true)
	{
		mmsg = GetMouseMsg();

		if (mmsg.uMsg == WM_LBUTTONDOWN)
		{
			break;
		}
	}

	cleardevice();
	setorigin(RESETX, RESETY);
	setaspectratio(1, -1);
	putimage(-10, -10, &image);
}

// 退出游戏
void quit()
{
	closegraph();
}