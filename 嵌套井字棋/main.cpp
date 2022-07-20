// 程序：嵌套井字棋
// 作者：Gary
// 编译环境：Visual C++ 6.0，EasyX_20211109
// 编写日期：2020-2-13

# include <math.h>
# include <graphics.h>



////////////////////////////// 游戏进程头文件 //////////////////////////////



// 头文件
// 头文件编译开始
#ifndef CIRCLE_H
#define CIRCLE_H

// 定义变量
static double pi = acos(-1);		// 圆周率π
static HWND hOut;					// 画布

// 定义一个结构体，格子
struct Node1
{
	int bigboxnum;					// 所在大格编号
	int smallboxnum;				// 所在小格编号
	int posx, posy;					// 坐标
	int type;						// 格子从属状态
};

// 定义一个结构体，操作手
struct Node2
{
	int nowboxnum;					// 本次落子的大格编号
	int nextboxnum;					// 下次落子的大格编号
	int type;						// 执手判断
};

// 定义一个结构体，按钮
struct Node3
{
	int posx1, posy1, posx2, posy2;	// 坐标
	double r;						// 原按钮半径
	const char* text;					// 按钮文本
	int type;						// 按钮激活状态
};

// 定义一个结构体，主题
struct Node4
{
	COLORREF blockcolor[4];			// 叉叉主体	；叉叉光影		；圈圈主体	；圈圈光影
	COLORREF bkcolor[3];			// 大背景	；落子范围背景	；按钮背景
	COLORREF linecolor[3];			// 小格子线	；大格子线		；落子范围线
	int type;						// 主题模式
};

// 定义一个结构体，记录
struct Node5
{
	int bigboxnum[90];				// 所在大格编号
	int smallboxnum[90];			// 所在小格编号
	int flag;						// 哨兵
	int type;						// 记录开启状态模式
};

// 定义一个类，game
class game
{
public:
	// 函数
	// 游戏进程源文件
	int carry_main();									// 游戏进程主循函数

	void draw_button();								// 按钮绘制函数
	void draw_circle(double size, int posx, int posy);	// 圈圈棋子绘制函数
	void draw_crosses(double size, int posx, int posy); // 十字架棋子绘制函数
	void draw_word(double size, int posx, int posy);	// 标题绘制函数

	void check_big(int back_or_not);					// 大格判断函数
	void check_small(int back_or_not);					// 小格判断函数

	void machine();										// 人机函数
	int machine_check();								// 人机决策树函数
	void mod_scene_change();							// 主题参数变更后，颜色结构体参数更新函数
	void read_mod_scene(int a);							// 主题参数读取函数

	// 准备进程源文件
	void carry_prepare();								// 准备进程主循函数
	void initialization_prepare();						// 准备进程初始化函数	
	void draw_scene_prepare();							// 准备进程绘制函数

	// 开始进程源文件
	void carry_start();								// 开始进程主循函数
	void initialization_start();						// 开始进程初始化函数
	void draw_scene_start();							// 开始进程绘制函数

	// 结构体
	Node1 box[10][10];									// 格子，一百个
	Node2 block;										// 操作手
	Node3 boxm[20];										// 按钮，预制二十个
	Node4 color;										// 颜色	
	Node5 record;										// 记录

	// 变量
	int ma[2];											// 人机输出的两个值，代表人机下棋子的位置
	int num_button;										// 按钮数量
	int exit_carry;										// 主进程主循函数控制参数
	int exit_start;										// 开始进程主循函数控制参数
	int mod_scene;										// 主题模式参数，两种
	int mod_start;										// 游戏模式参数，六种
	int back_or_not;									// 悔棋参数，置一表示是悔棋操作中，跳过闪烁显示
	int eat[9];											// 存临时九宫格状态
};

// 头文件编译结束
#endif




////////////////////////////// 主程序源文件 //////////////////////////////




// 窗口初始化
void initialization()
{
	// 窗口定义，整个程序仅定义一次
	hOut = initgraph(601, 501);
	// 窗口标题
	SetWindowText(hOut, _T("嵌套井字棋"));
}



// 总进程主循函数
void carry()
{
	// 总进程参数初始化
	int exit_carry = 0;
	// 进程控制
	BeginBatchDraw();
	// 创建游戏类对象
	game G;
	// 初始化主题
	G.read_mod_scene(2);
	// 游戏进程
	while (exit_carry == 0)
	{
		// 执行游戏总进程，并接收游戏进程结束时返回值
		// 准备进程时，点击退出按钮结束返回 1
		// 开始进程时，点击返回按钮结束返回 0
		exit_carry = G.carry_main();
	}
	EndBatchDraw();
	// 关闭窗口
	closegraph();
}



// 主函数
int main(void)
{
	// 初始化
	initialization();
	// 总进程主循函数
	carry();
	return 0;
}



////////////////////////////// 游戏进程源文件 //////////////////////////////




// 游戏进程主循函数
int game::carry_main()
{
	// 游戏进程参数初始化
	exit_carry = 0;
	// 准备进程初始化函数
	initialization_prepare();
	// 准备进程主循函数
	carry_prepare();
	// 准备进程结束
	// 准备进程时，点击退出按钮结束，游戏进程参数为 1
	// 准备进程时，正常进入游戏，游戏进程参数为 0
	if (exit_carry == 0)
	{
		// 开始进程初始化函数
		initialization_start();
		// 开始进程主循函数
		carry_start();
	}
	// 开始进程结束
	// 点击返回按钮结束，或者正常游戏结束，游戏进程参数不变
	return exit_carry;
}



// 按钮绘制函数
void game::draw_button()
{
	// 根据颜色结构体设置参数
	setlinecolor(color.linecolor[0]);
	setfillcolor(color.bkcolor[2]);
	setbkcolor(color.bkcolor[2]);
	settextcolor(color.bkcolor[0]);
	settextstyle(30, 15, _T("Consolas"));
	setlinestyle(PS_SOLID, 10);

	// 根据按钮数量参数绘制
	for (int i = 0; i < num_button; i++)
	{
		if (boxm[i].type == 0)
		{
			// 边框
			fillroundrect(boxm[i].posx1, boxm[i].posy1, boxm[i].posx2, boxm[i].posy2, 25, 25);
			// 文字
			outtextxy(boxm[i].posx1 + (boxm[i].posx2 - boxm[i].posx1) / 2 - textwidth(boxm[i].text) / 2, boxm[i].posy1 + 15, boxm[i].text);
		}
	}
	FlushBatchDraw();
}



// 主题模式参数改变后，颜色结构体参数更新函数
void game::mod_scene_change()
{
	mod_scene = mod_scene == 2 ? 1 : 2;
	// 白天模式
	if (mod_scene == 1)
	{
		// 十字架棋子主题颜色
		color.blockcolor[0] = RGB(234, 14, 15);

		// 十字架棋子光影颜色
		color.blockcolor[1] = RGB(247, 125, 128);

		// 圈圈架棋子主题颜色
		color.blockcolor[2] = RGB(31, 16, 235);

		// 圈圈架棋子光影颜色
		color.blockcolor[3] = RGB(137, 128, 237);

		// 背景颜色
		color.bkcolor[0] = RGB(250, 250, 250);
		color.bkcolor[1] = RGB(234, 128, 128);
		color.bkcolor[2] = RGB(88, 88, 88);

		// 线条颜色
		color.linecolor[0] = RGB(125, 125, 125);
		color.linecolor[1] = RGB(2, 2, 2);
		color.linecolor[2] = RGB(215, 24, 24);
	}
	// 夜间模式
	else if (mod_scene == 2)
	{
		// 十字架棋子主题颜色
		color.blockcolor[0] = RGB(234, 123, 5);

		// 十字架棋子光影颜色
		color.blockcolor[1] = RGB(234, 179, 129);

		// 圈圈架棋子主题颜色
		color.blockcolor[2] = RGB(123, 235, 13);

		// 圈圈架棋子光影颜色
		color.blockcolor[3] = RGB(181, 235, 126);

		// 背景颜色
		color.bkcolor[0] = RGB(0, 0, 0);
		color.bkcolor[1] = RGB(233, 180, 128);
		color.bkcolor[2] = RGB(254, 254, 254);

		// 线条颜色
		color.linecolor[0] = RGB(125, 125, 125);
		color.linecolor[1] = RGB(254, 254, 254);
		color.linecolor[2] = RGB(233, 124, 19);
	}
}



// 读取主题模式参数函数，仅针对第一次	
void game::read_mod_scene(int a)
{
	mod_scene = a;
	// 主题模式参数更新后，颜色结构体参数更新
	mod_scene_change();
}



// 十字架绘制函数
void game::draw_crosses(double size, int posx, int posy)
{
	// 绘制
	POINT p[12];

	// 定义一个结构体
	struct Node1
	{
		// 单位坐标
		double x, y;
	};
	Node1 pts[12];

	// 主体
	setfillcolor(color.blockcolor[0]);
	setlinecolor(color.linecolor[1]);
	setlinestyle(PS_SOLID, 2);
	// 单位坐标定位
	pts[0].x = -0.5; pts[0].y = -1.0;
	pts[1].x = 0.0; pts[1].y = -0.5;
	pts[2].x = 0.5; pts[2].y = -1.0;
	pts[3].x = 1.0; pts[3].y = -0.5;
	pts[4].x = 0.5; pts[4].y = 0.0;
	pts[5].x = 1.0; pts[5].y = 0.5;
	pts[6].x = 0.5; pts[6].y = 1.0;
	pts[7].x = 0.0; pts[7].y = 0.5;
	pts[8].x = -0.5; pts[8].y = 1.0;
	pts[9].x = -1.0; pts[9].y = 0.5;
	pts[10].x = -0.5; pts[10].y = 0.0;
	pts[11].x = -1.0; pts[11].y = -0.5;

	// 形参代入
	for (int i = 0; i < 12; i++)
	{
		p[i].x = long(pts[i].x * size + posx);
		p[i].y = long(pts[i].y * size + posy);
	}

	// 绘制
	fillpolygon(p, 12);


	// 光影
	setfillcolor(color.blockcolor[1]);
	setlinecolor(color.blockcolor[1]);
	setlinestyle(PS_SOLID, 1);

	// 单位坐标定位
	pts[0].x = 0.135; pts[0].y = -0.5;
	pts[1].x = 0.5; pts[1].y = -0.865;
	pts[2].x = 0.865; pts[2].y = -0.5;
	pts[3].x = 0.82; pts[3].y = -0.455;
	pts[4].x = 0.5; pts[4].y = -0.78;
	pts[5].x = 0.18; pts[5].y = -0.455;

	// 形参代入
	for (int i = 0; i < 6; i++)
	{
		p[i].x = long(pts[i].x * size + posx);
		p[i].y = long(pts[i].y * size + posy);
	}

	// 绘制
	fillpolygon(p, 6);

	pts[0].x = -0.865; pts[0].y = -0.5;
	pts[1].x = -0.5; pts[1].y = -0.865;
	pts[2].x = -0.135; pts[2].y = -0.5;
	pts[3].x = -0.18; pts[3].y = -0.455;
	pts[4].x = -0.5; pts[4].y = -0.78;
	pts[5].x = -0.82; pts[5].y = -0.455;

	// 形参代入
	for (int i = 0; i < 6; i++)
	{
		p[i].x = long(pts[i].x * size + posx);
		p[i].y = long(pts[i].y * size + posy);
	}

	// 绘制
	fillpolygon(p, 6);

	// 单位坐标定位
	pts[0].x = 0.5; pts[0].y = -0.865 + 1;
	pts[1].x = 0.865; pts[1].y = -0.5 + 1;
	pts[2].x = 0.82; pts[2].y = -0.455 + 1;
	pts[3].x = 0.455; pts[3].y = -0.82 + 1;

	// 形参代入
	for (int i = 0; i < 4; i++)
	{
		p[i].x = long(pts[i].x * size + posx);
		p[i].y = long(pts[i].y * size + posy);
	}

	// 绘制
	fillpolygon(p, 4);

	// 单位坐标定位
	pts[0].x = -0.865; pts[0].y = -0.5 + 1;
	pts[1].x = -0.5; pts[1].y = -0.865 + 1;
	pts[2].x = -0.455; pts[2].y = -0.82 + 1;
	pts[3].x = -0.82; pts[3].y = -0.455 + 1;

	// 形参代入
	for (int i = 0; i < 4; i++)
	{
		p[i].x = long(pts[i].x * size + posx);
		p[i].y = long(pts[i].y * size + posy);
	}

	// 绘制
	fillpolygon(p, 4);
}



// 圈圈绘制函数
void game::draw_circle(double size, int posx, int posy)
{
	// 从半径最大开始，往半径最小进行
	// 最大圆
	setfillcolor(color.blockcolor[2]);
	setlinecolor(color.linecolor[1]);
	setlinestyle(PS_SOLID, 2);

	fillcircle(posx, posy, int(size));

	// 光影
	setfillcolor(color.blockcolor[3]);
	setlinecolor(color.blockcolor[3]);
	setlinestyle(PS_SOLID, 1);

	fillpie(int(-0.80 * size + posx), int(-0.80 * size + posy), int(0.80 * size + posx), int(0.80 * size + posy), 0, pi / 2.0);
	fillpie(int(-0.70 * size + posx), int(-0.70 * size + posy), int(0.70 * size + posx), int(0.70 * size + posy), 0, pi / 2.0);

	// 中圆
	setfillcolor(color.blockcolor[2]);
	setlinecolor(color.blockcolor[2]);
	setlinestyle(PS_SOLID, 1);

	fillcircle(posx, posy, int(0.70 * size));

	// 最小圆
	setfillcolor(color.bkcolor[0]);
	setlinecolor(color.linecolor[1]);
	setlinestyle(PS_SOLID, 2);

	fillcircle(posx, posy, int(0.39 * size));
}



// 小格判断函数
void game::check_small(int back_or_not)
{
	int i, j, k, t;
	i = block.nowboxnum;
	j = block.type;
	// 小格判断。若小格三连，则对应大格有归属，则需要进行大格判断
	// 三行分别是包含左上角点的三条线，包含中心点的三条线，包含右下角点的两条线
	if ((box[i][0].type == j && ((box[i][1].type == j && box[i][2].type == j) || (box[i][3].type == j && box[i][6].type == j) || (box[i][4].type == j && box[i][8].type == j)))
		|| (box[i][4].type == j && ((box[i][1].type == j && box[i][7].type == j) || (box[i][3].type == j && box[i][5].type == j) || (box[i][2].type == j && box[i][6].type == j)))
		|| (box[i][8].type == j && ((box[i][2].type == j && box[i][5].type == j) || (box[i][6].type == j && box[i][7].type == j))))
	{
		// 判断成功
		// 非悔棋状态
		if (back_or_not == 0)
		{
			// 闪烁显示
			// 绘制小棋子
			for (k = 0; k < 2; k++)
			{
				// 格子绘制
				setfillcolor(color.bkcolor[1]);
				setlinecolor(color.linecolor[1]);
				setlinestyle(PS_SOLID, 5);
				for (t = 0; t < 9; t++)
				{
					if (box[i][t].type == j)
					{
						fillrectangle(box[i][t].posx, box[i][t].posy, box[i][t].posx + 50, box[i][t].posy + 50);
					}
				}
				FlushBatchDraw();
				Sleep(200);

				// 棋子绘制
				for (t = 0; t < 9; t++)
				{
					if (box[i][t].type == j)
					{
						if (box[i][t].type == 1) { draw_crosses(25, box[i][t].posx + 25, box[i][t].posy + 25); }
						else if (box[i][t].type == 2) { draw_circle(25, box[i][t].posx + 25, box[i][t].posy + 25); }
					}
				}
				FlushBatchDraw();
				Sleep(200);
			}
		}

		// 更改小格子参数（属性）
		for (k = 0; k <= 8; k++)
		{
			box[i][k].type = j;
		}

		// 更改大格子参数（属性）
		box[i][9].type = j;

		// 非悔棋状态
		if (back_or_not == 0)
		{
			// 闪烁
			// 绘制大棋子
			for (k = 0; k < 2; k++)
			{
				// 格子绘制
				setfillcolor(color.bkcolor[1]);
				setlinecolor(color.linecolor[1]);
				setlinestyle(PS_SOLID, 5);
				fillrectangle(box[i][0].posx, box[i][0].posy, box[i][8].posx + 50, box[i][8].posy + 50);
				FlushBatchDraw();
				Sleep(200);

				// 棋子绘制
				if (box[i][9].type == 1) { draw_crosses(75, box[i][4].posx + 25, box[i][4].posy + 25); }
				else if (box[i][9].type == 2) { draw_circle(75, box[i][4].posx + 25, box[i][4].posy + 25); }
				FlushBatchDraw();
				Sleep(200);
			}
		}

		// 大格判断
		check_big(back_or_not);
	}
	// 小格判断。若小格满且无归属，则对应大格不能为零了，也不能是 1 和 2
	// k = 0 无空位
	k = 0;
	for (j = 0; j < 9; j++)
	{
		if (box[i][j].type == 0)
		{
			// k = 1 有空位，则大格中的小格未满
			k = 1;
			break;
		}
	}
	// 大格参数为 3，即不属于任何一方
	if (k == 0 && box[i][9].type == 0)
	{
		box[i][9].type = 3;
	}
}



// 大格判断函数
void game::check_big(int back_or_not)
{
	int j, k, t;
	j = block.type;

	// 大格判断，若大格三连则立即结束
	// 三行分别是包含左上角点的三条线，包含中心点的三条线，包含右下角点的两条线
	if ((box[0][9].type == j && ((box[1][9].type == j && box[2][9].type == j) || (box[3][9].type == j && box[6][9].type == j) || (box[4][9].type == j && box[8][9].type == j)))
		|| (box[4][9].type == j && ((box[1][9].type == j && box[7][9].type == j) || (box[3][9].type == j && box[5][9].type == j) || (box[2][9].type == j && box[6][9].type == j)))
		|| (box[8][9].type == j && ((box[2][9].type == j && box[5][9].type == j) || (box[6][9].type == j && box[7][9].type == j))))
	{
		// 更改棋盘参数（属性）
		box[9][9].type = j;

		// 非悔棋状态
		if (back_or_not == 0)
		{
			// 闪烁显示
			// 绘制大棋子
			for (k = 0; k < 2; k++)
			{
				// 格子绘制
				setfillcolor(color.bkcolor[1]);
				setlinecolor(color.linecolor[1]);
				setlinestyle(PS_SOLID, 5);

				for (t = 0; t < 9; t++)
				{
					if (box[t][9].type == j)
					{
						fillrectangle(box[t][0].posx, box[t][0].posy, box[t][8].posx + 50, box[t][8].posy + 50);
					}
				}
				FlushBatchDraw();
				Sleep(200);

				// 棋子绘制
				for (t = 0; t < 9; t++)
				{
					if (box[t][9].type == j)
					{
						if (box[t][9].type == 1) { draw_crosses(75, box[t][4].posx + 25, box[t][4].posy + 25); }
						else if (box[t][9].type == 2) { draw_circle(75, box[t][4].posx + 25, box[t][4].posy + 25); }
					}
				}
				FlushBatchDraw();
				Sleep(200);
			}

			// 闪烁
			// 绘制超大棋子
			for (k = 0; k < 2; k++)
			{
				// 格子绘制
				setfillcolor(color.bkcolor[1]);
				setlinecolor(color.linecolor[1]);
				setlinestyle(PS_SOLID, 5);
				fillrectangle(box[0][0].posx, box[0][0].posy, box[8][8].posx + 50, box[8][8].posy + 50);
				FlushBatchDraw();
				Sleep(200);

				// 棋子绘制
				if (box[9][9].type == 1) { draw_crosses(225, box[4][4].posx + 25, box[4][4].posy + 25); }
				else if (box[9][9].type == 2) { draw_circle(225, box[4][4].posx + 25, box[4][4].posy + 25); }
				FlushBatchDraw();
				Sleep(200);
			}
		}
	}
}



// 人机决策数函数
int game::machine_check()
{
	int i, j, k, b, c;

	// 定义一个结构体，决策树
	struct Node
	{
		int eat[9]; // 九宫格状态
		int eat_changenum; // 子结点改变了父结点哪个空格状态
		int gamenum; // 估计值
	};
	Node tree[9];

	// 初始化
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			tree[i].eat[j] = eat[j];
			tree[i].eat_changenum = i;
		}
		tree[i].eat[i] = block.type;
		tree[i].gamenum = 0;
	}

	b = block.type;
	c = b == 1 ? 2 : 1;

	// 特殊模式和嵌套模式
	if (exit_start == -1 || exit_start == 0)
	{
		for (i = 0; i < 9; i++)
		{
			// 己方绝杀点
			if (// 上水平线
				(tree[i].eat[0] == b && tree[i].eat[1] == b && tree[i].eat_changenum == 2) ||
				(tree[i].eat[0] == b && tree[i].eat[2] == b && tree[i].eat_changenum == 1) ||
				(tree[i].eat[1] == b && tree[i].eat[2] == b && tree[i].eat_changenum == 0) ||
				// 中水平线
				(tree[i].eat[3] == b && tree[i].eat[4] == b && tree[i].eat_changenum == 5) ||
				(tree[i].eat[3] == b && tree[i].eat[5] == b && tree[i].eat_changenum == 4) ||
				(tree[i].eat[4] == b && tree[i].eat[5] == b && tree[i].eat_changenum == 3) ||
				// 下水平线
				(tree[i].eat[6] == b && tree[i].eat[7] == b && tree[i].eat_changenum == 8) ||
				(tree[i].eat[6] == b && tree[i].eat[8] == b && tree[i].eat_changenum == 7) ||
				(tree[i].eat[7] == b && tree[i].eat[8] == b && tree[i].eat_changenum == 6) ||
				// 左垂直线
				(tree[i].eat[0] == b && tree[i].eat[3] == b && tree[i].eat_changenum == 6) ||
				(tree[i].eat[0] == b && tree[i].eat[6] == b && tree[i].eat_changenum == 3) ||
				(tree[i].eat[3] == b && tree[i].eat[6] == b && tree[i].eat_changenum == 0) ||
				// 中垂直线
				(tree[i].eat[1] == b && tree[i].eat[4] == b && tree[i].eat_changenum == 7) ||
				(tree[i].eat[1] == b && tree[i].eat[7] == b && tree[i].eat_changenum == 4) ||
				(tree[i].eat[4] == b && tree[i].eat[7] == b && tree[i].eat_changenum == 1) ||
				// 右垂直线
				(tree[i].eat[2] == b && tree[i].eat[5] == b && tree[i].eat_changenum == 8) ||
				(tree[i].eat[2] == b && tree[i].eat[8] == b && tree[i].eat_changenum == 5) ||
				(tree[i].eat[5] == b && tree[i].eat[8] == b && tree[i].eat_changenum == 2) ||
				// 左上右下斜线
				(tree[i].eat[0] == b && tree[i].eat[4] == b && tree[i].eat_changenum == 8) ||
				(tree[i].eat[0] == b && tree[i].eat[8] == b && tree[i].eat_changenum == 4) ||
				(tree[i].eat[4] == b && tree[i].eat[8] == b && tree[i].eat_changenum == 0) ||
				// 右上左下斜线
				(tree[i].eat[2] == b && tree[i].eat[4] == b && tree[i].eat_changenum == 6) ||
				(tree[i].eat[2] == b && tree[i].eat[6] == b && tree[i].eat_changenum == 4) ||
				(tree[i].eat[4] == b && tree[i].eat[6] == b && tree[i].eat_changenum == 2))
			{
				tree[i].gamenum += 1000;
			}
			// 敌方绝杀点
			else if (// 上水平线
				(tree[i].eat[0] == c && tree[i].eat[1] == c && tree[i].eat_changenum == 2) ||
				(tree[i].eat[0] == c && tree[i].eat[2] == c && tree[i].eat_changenum == 1) ||
				(tree[i].eat[1] == c && tree[i].eat[2] == c && tree[i].eat_changenum == 0) ||
				// 中水平线
				(tree[i].eat[3] == c && tree[i].eat[4] == c && tree[i].eat_changenum == 5) ||
				(tree[i].eat[3] == c && tree[i].eat[5] == c && tree[i].eat_changenum == 4) ||
				(tree[i].eat[4] == c && tree[i].eat[5] == c && tree[i].eat_changenum == 3) ||
				// 下水平线
				(tree[i].eat[6] == c && tree[i].eat[7] == c && tree[i].eat_changenum == 8) ||
				(tree[i].eat[6] == c && tree[i].eat[8] == c && tree[i].eat_changenum == 7) ||
				(tree[i].eat[7] == c && tree[i].eat[8] == c && tree[i].eat_changenum == 6) ||
				// 左垂直线
				(tree[i].eat[0] == c && tree[i].eat[3] == c && tree[i].eat_changenum == 6) ||
				(tree[i].eat[0] == c && tree[i].eat[6] == c && tree[i].eat_changenum == 3) ||
				(tree[i].eat[3] == c && tree[i].eat[6] == c && tree[i].eat_changenum == 0) ||
				// 中垂直线
				(tree[i].eat[1] == c && tree[i].eat[4] == c && tree[i].eat_changenum == 7) ||
				(tree[i].eat[1] == c && tree[i].eat[7] == c && tree[i].eat_changenum == 4) ||
				(tree[i].eat[4] == c && tree[i].eat[7] == c && tree[i].eat_changenum == 1) ||
				// 右垂直线
				(tree[i].eat[2] == c && tree[i].eat[5] == c && tree[i].eat_changenum == 8) ||
				(tree[i].eat[2] == c && tree[i].eat[8] == c && tree[i].eat_changenum == 5) ||
				(tree[i].eat[5] == c && tree[i].eat[8] == c && tree[i].eat_changenum == 2) ||
				// 左上右下斜线
				(tree[i].eat[0] == c && tree[i].eat[4] == c && tree[i].eat_changenum == 8) ||
				(tree[i].eat[0] == c && tree[i].eat[8] == c && tree[i].eat_changenum == 4) ||
				(tree[i].eat[4] == c && tree[i].eat[8] == c && tree[i].eat_changenum == 0) ||
				// 右上左下斜线
				(tree[i].eat[2] == c && tree[i].eat[4] == c && tree[i].eat_changenum == 6) ||
				(tree[i].eat[2] == c && tree[i].eat[6] == c && tree[i].eat_changenum == 4) ||
				(tree[i].eat[4] == c && tree[i].eat[6] == c && tree[i].eat_changenum == 2))
			{
				tree[i].gamenum += 1000;
			}
			// 中心点
			else if (tree[i].eat_changenum == 4)
			{
				tree[i].gamenum += 10;
			}
			// 对角点
			else if (tree[i].eat[4] == b)
			{
				if (tree[i].eat[0] == c && tree[i].eat_changenum == 8) { tree[i].gamenum += 10; }
				if (tree[i].eat[2] == c && tree[i].eat_changenum == 6) { tree[i].gamenum += 10; }
				if (tree[i].eat[6] == c && tree[i].eat_changenum == 2) { tree[i].gamenum += 10; }
				if (tree[i].eat[8] == c && tree[i].eat_changenum == 0) { tree[i].gamenum += 10; }
				if (tree[i].eat_changenum == 1 && ((tree[i].eat[0] == c && tree[i].eat[8] == c) ||
					(tree[i].eat[2] == c && tree[i].eat[6] == c)))
				{
					tree[i].gamenum += 100;
				}
			}
			// 特殊点
			else if (tree[i].eat_changenum == 2 && tree[i].eat[4] == c && tree[i].eat[8] == c)
			{
				tree[i].gamenum += 100;
			}
			// 点已被占
			if ((ma[0] == -1 && box[tree[i].eat_changenum][9].type != 0) || (ma[0] != -1 && box[ma[0]][tree[i].eat_changenum].type != 0))
			{
				tree[i].gamenum = -10000;
			}
		}
	}
	// 嵌套模式的小格选择
	if (exit_start == 0 && ma[0] != -1)
	{
		for (i = 0; i < 9; i++)
		{
			if (box[tree[i].eat_changenum][9].type != 0)
			{
				tree[i].gamenum -= 100;
			}
			if (// 上水平线
				(box[0][9].type == c && box[1][9].type == c && (tree[i].eat_changenum == 2 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[0][9].type == c && box[2][9].type == c && (tree[i].eat_changenum == 1 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[1][9].type == c && box[2][9].type == c && (tree[i].eat_changenum == 0 || box[tree[i].eat_changenum][9].type != 0)) ||
				// 中水平线
				(box[3][9].type == c && box[4][9].type == c && (tree[i].eat_changenum == 5 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[3][9].type == c && box[5][9].type == c && (tree[i].eat_changenum == 4 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[4][9].type == c && box[5][9].type == c && (tree[i].eat_changenum == 3 || box[tree[i].eat_changenum][9].type != 0)) ||
				// 下水平线
				(box[6][9].type == c && box[7][9].type == c && (tree[i].eat_changenum == 8 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[6][9].type == c && box[8][9].type == c && (tree[i].eat_changenum == 7 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[7][9].type == c && box[8][9].type == c && (tree[i].eat_changenum == 6 || box[tree[i].eat_changenum][9].type != 0)) ||
				// 左垂直线
				(box[0][9].type == c && box[3][9].type == c && (tree[i].eat_changenum == 6 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[0][9].type == c && box[6][9].type == c && (tree[i].eat_changenum == 3 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[3][9].type == c && box[6][9].type == c && (tree[i].eat_changenum == 0 || box[tree[i].eat_changenum][9].type != 0)) ||
				// 中垂直线
				(box[1][9].type == c && box[4][9].type == c && (tree[i].eat_changenum == 7 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[1][9].type == c && box[7][9].type == c && (tree[i].eat_changenum == 4 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[4][9].type == c && box[7][9].type == c && (tree[i].eat_changenum == 1 || box[tree[i].eat_changenum][9].type != 0)) ||
				// 右垂直线
				(box[2][9].type == c && box[5][9].type == c && (tree[i].eat_changenum == 8 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[2][9].type == c && box[8][9].type == c && (tree[i].eat_changenum == 5 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[5][9].type == c && box[8][9].type == c && (tree[i].eat_changenum == 2 || box[tree[i].eat_changenum][9].type != 0)) ||
				// 左上右下斜线
				(box[0][9].type == c && box[4][9].type == c && (tree[i].eat_changenum == 8 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[0][9].type == c && box[8][9].type == c && (tree[i].eat_changenum == 4 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[4][9].type == c && box[8][9].type == c && (tree[i].eat_changenum == 0 || box[tree[i].eat_changenum][9].type != 0)) ||
				// 右上左下斜线
				(box[2][9].type == c && box[4][9].type == c && (tree[i].eat_changenum == 6 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[2][9].type == c && box[6][9].type == c && (tree[i].eat_changenum == 4 || box[tree[i].eat_changenum][9].type != 0)) ||
				(box[4][9].type == c && box[6][9].type == c && (tree[i].eat_changenum == 2 || box[tree[i].eat_changenum][9].type != 0)))
			{
				tree[i].gamenum -= 1000;
			}
			for (j = 0; j < 9; j++)
			{
				if (box[tree[i].eat_changenum][j].type == c)
				{
					tree[i].gamenum -= 100;
				}
			}
		}
	}

	// 给最大值
	k = -0xffffff; j = 0;
	for (i = 0; i < 9; i++)
	{
		if (tree[i].gamenum > k && ((ma[0] == -1 && box[tree[i].eat_changenum][9].type == 0) || (ma[0] != -1 && box[ma[0]][tree[i].eat_changenum].type == 0)))
		{
			k = tree[i].gamenum;
			j = tree[i].eat_changenum;
		}
	}

	// 错误输出，则纠正
	if (ma[0] != -1 && box[ma[0]][j].type != 0)
	{
		for (i = 0; i < 9; i++)
		{
			if (box[i][9].type == 0)
			{
				ma[0] = i;
				break;
			}
		}
		for (j = 0; j < 9; j++)
		{
			if (box[ma[0]][j].type == 0)
			{
				break;
			}
		}
	}

	// 返回决策值
	return j;
}



// 人机函数
void game::machine()
{
	int i;
	// ai 的任务就是返回两个数
	ma[0] = -1; ma[1] = -1;
	// 大格无空位
	if (box[block.nextboxnum][9].type != 0)
	{
		// 初始化临时九宫格
		for (i = 0; i < 9; i++)
		{
			eat[i] = box[i][9].type;
		}
		// 人机决策数函数
		ma[0] = machine_check();
	}
	// 大格有空位
	else if (box[block.nextboxnum][9].type == 0)
	{
		// 普通模式被限制
		ma[0] = block.nextboxnum;

		// 特殊模式无限制
		if (block.nextboxnum == 9 && box[block.nowboxnum][9].type == 0)
		{
			// 上一步在哪，则这一步在哪
			ma[0] = block.nowboxnum;
		}
		else if (block.nextboxnum == 9 && box[block.nowboxnum][9].type != 0)
		{
			// 上一步是某个大格的最后一步，则决策下一步下哪个大格
			// 初始化临时九宫格
			for (i = 0; i < 9; i++)
			{
				eat[i] = box[i][9].type;
			}
			// 人机决策数函数
			ma[0] = machine_check();
		}
	}

	// 小格判断
	for (i = 0; i < 9; i++)
	{
		// 初始化临时九宫格
		eat[i] = box[ma[0]][i].type;
	}
	// 人机决策数函数
	ma[1] = machine_check();
}



////////////////////////////// 准备进程源文件 //////////////////////////////



// 准备进程主循函数
void game::carry_prepare()
{
	// 鼠标定义
	ExMessage m;
	// 准备进程控制参数初始化
	int	exit_prepare = 0;
	// 绘制界面
	draw_scene_prepare();
	// 主循开始
	while (exit_prepare == 0)
	{
		if (MouseHit())
		{
			m = getmessage(EM_MOUSE | EM_KEY);
			// 左键单击判断
			if (m.message == WM_LBUTTONDOWN)
			{
				int i = 0;
				// 判断是否点击了按钮，按钮状态是否为激活
				for (i = 0; i < 10; i++)
				{
					if (m.x > boxm[i].posx1 && m.y > boxm[i].posy1 && m.x < boxm[i].posx2 && m.y < boxm[i].posy2 && boxm[i].type == 0)
					{
						break;
					}
				}

				// 开始游戏按钮
				if (i == 0)
				{
					// 进入选择游戏模式界面
					// 按钮激活状态变更
					boxm[0].type = 1;
					boxm[1].type = 1;
					boxm[2].type = 1;
					boxm[9].type = 1;
					boxm[3].type = 0;
					boxm[4].type = 0;
					boxm[5].type = 0;
					boxm[6].type = 0;
					// 开始进程控制参数为 0，进入普通模式
					exit_start = 0;
				}
				// 主题更换按钮
				else if (i == 1)
				{
					// 进入主题更换界面
					// 按钮激活状态变更
					boxm[0].type = 1;
					boxm[1].type = 1;
					boxm[2].type = 1;
					boxm[9].type = 1;
					boxm[6].type = 0;
					boxm[7].type = 0;
					boxm[8].type = 0;
				}
				// 退出按钮
				else if (i == 2)
				{
					// 总进程控制参数置一，跳过开始进程
					exit_carry = 1;
					// 准备进程控制参数置一，结束主循
					exit_prepare = 1;
				}
				// 选择模式，进入游戏
				else if (i == 3 || i == 4 || i == 5)
				{
					// 普通模式加特殊模式，双人，人机先，人机后共 6 种
					// 开始进程控制参数有 0（普通模式）-1（特殊模式）
					// 游戏模式控制参数有 0（双人）1（人机先）2（人机后）
					mod_start = i - 3;
					// 准备进程控制参数置一，结束主循，进入开始进程
					exit_prepare = 1;
				}
				// 后退按钮
				else if (i == 6)
				{
					// 从主题选择界面返回
					// 按钮激活状态变更
					boxm[0].type = 0;
					boxm[1].type = 0;
					boxm[2].type = 0;
					boxm[9].type = 0;
					boxm[3].type = 1;
					boxm[4].type = 1;
					boxm[5].type = 1;
					boxm[6].type = 1;
					boxm[7].type = 1;
					boxm[8].type = 1;
				}
				// 模式一按钮
				else if (i == 7 && mod_scene == 2)
				{
					// 主题变换函数，颜色结构体变更
					mod_scene_change();
				}
				// 模式二按钮
				else if (i == 8 && mod_scene == 1)
				{
					// 主题变换函数，颜色结构体变更
					mod_scene_change();
				}
				// 特殊模式按钮
				else if (i == 9)
				{
					// 进入选择特殊模式界面
					// 按钮激活状态变更
					boxm[0].type = 1;
					boxm[1].type = 1;
					boxm[2].type = 1;
					boxm[9].type = 1;
					boxm[3].type = 0;
					boxm[4].type = 0;
					boxm[5].type = 0;
					boxm[6].type = 0;
					// 开始进程控制参数为 -1，进入特殊模式
					exit_start = -1;
				}
				if (i < 10 && exit_prepare == 0)
				{
					// 点击按钮后，根据按钮激活参数，重新绘制界面
					draw_scene_prepare();
					FlushBatchDraw();
				}
			}
		}
	}

}



// 准备进程初始化函数	
void game::initialization_prepare()
{
	// 按钮设置初始化
	// 按钮数量参数初始化
	num_button = 10;

	// 按钮结构体参数初始化
	boxm[0].posx1 = 200; boxm[0].posy1 = 120; boxm[0].posx2 = 400; boxm[0].posy2 = 180; boxm[0].text = _T("开始游戏"); boxm[0].type = 0;
	boxm[1].posx1 = 200; boxm[1].posy1 = 220; boxm[1].posx2 = 400; boxm[1].posy2 = 280; boxm[1].text = _T("主题更换"); boxm[1].type = 0;
	boxm[9].posx1 = 200; boxm[9].posy1 = 320; boxm[9].posx2 = 400; boxm[9].posy2 = 380; boxm[9].text = _T("特殊模式"); boxm[9].type = 0;
	boxm[2].posx1 = 200; boxm[2].posy1 = 420; boxm[2].posx2 = 400; boxm[2].posy2 = 480; boxm[2].text = _T("退出"); boxm[2].type = 0;

	boxm[3].posx1 = 200; boxm[3].posy1 = 120; boxm[3].posx2 = 400; boxm[3].posy2 = 180; boxm[3].text = _T("双人模式"); boxm[3].type = 1;
	boxm[4].posx1 = 200; boxm[4].posy1 = 220; boxm[4].posx2 = 400; boxm[4].posy2 = 280; boxm[4].text = _T("人机先手"); boxm[4].type = 1;
	boxm[5].posx1 = 200; boxm[5].posy1 = 320; boxm[5].posx2 = 400; boxm[5].posy2 = 380; boxm[5].text = _T("人机后手"); boxm[5].type = 1;

	boxm[6].posx1 = 200; boxm[6].posy1 = 420; boxm[6].posx2 = 400; boxm[6].posy2 = 480; boxm[6].text = _T("后退"); boxm[6].type = 1;

	boxm[7].posx1 = 200; boxm[7].posy1 = 120; boxm[7].posx2 = 400; boxm[7].posy2 = 180; boxm[7].text = _T("白天主题"); boxm[7].type = 1;
	boxm[8].posx1 = 200; boxm[8].posy1 = 220; boxm[8].posx2 = 400; boxm[8].posy2 = 280; boxm[8].text = _T("夜间主题"); boxm[8].type = 1;
}



// 准备进程绘制函数
void game::draw_scene_prepare()
{
	// 画布绘制
	setbkcolor(color.bkcolor[0]);
	cleardevice();
	// 按钮绘制
	draw_button();
	// 标题绘制
	draw_word(50, 200, 20);
	FlushBatchDraw();
}



// 标题绘制函数
void game::draw_word(double size, int posx, int posy)
{
	int i, j;
	const char* text;

	// 参数设置，填充透明
	setbkmode(TRANSPARENT);
	settextstyle(50, 20, _T("Consolas"));
	text = _T("嵌套井字棋");

	// 背景色的反色
	settextcolor(color.linecolor[1]);
	// 范围绘制，构造阴影
	for (i = posx - 3; i <= posx + 3; i++)
	{
		for (j = posy - 3; j <= posy + 3; j++)
		{
			outtextxy(i, j, text);
		}
	}

	// 背景色
	settextcolor(color.bkcolor[0]);
	// 在阴影中绘制
	outtextxy(posx, posy, text);

	// 恢复填充
	setbkmode(OPAQUE);
	FlushBatchDraw();
}



////////////////////////////// 开始进程源文件 //////////////////////////////



// 开始进程主循函数
void game::carry_start()
{
	// 鼠标定义
	ExMessage m;
	int i, j, k, x1, y1;
	// 记录初始化
	record.flag = 0;
	// 操作手参数重置
	block.type = 1;
	// 悔棋参数设置
	back_or_not = 0;
	// 绘制
	draw_scene_start();
	// 嵌套模式
	while (exit_start == 0)
	{
		if (MouseHit())
		{
			m = getmessage(EM_MOUSE | EM_KEY);
			// 左键单击判断
			if (m.message == WM_LBUTTONDOWN || (mod_start == 1 && block.type == 1) || (mod_start == 2 && block.type == 2))
			{
				// 单击在棋盘范围
				if ((m.x < 475 && m.x > 25 && m.y > 25 && m.y < 475) || (mod_start == 1 && block.type == 1) || (mod_start == 2 && block.type == 2))
				{
					// 人机模式
					if ((mod_start == 1 && block.type == 1) || (mod_start == 2 && block.type == 2))
					{
						k = 0;
						machine();
						i = ma[0];
						j = ma[1];
					}
					// 双人模式
					else if (m.x < 475 && m.x > 25 && m.y > 25 && m.y < 475)
					{
						// 判断大格内有无可以落子的地方
						// k = 0 无空位
						k = 0;
						for (i = 0; i < 9; i++)
						{
							if (box[block.nextboxnum][i].type == 0)
							{
								// k = 1 有空位，则必须下大格内
								k = 1;
								break;
							}
						}

						// 识别点击了哪个格子
						i = (m.x - 25) / 150 + (m.y - 25) / 150 * 3;
						x1 = m.x; y1 = m.y;
						while (x1 > 175)
						{
							x1 = x1 - 150;
						}
						while (y1 > 175)
						{
							y1 = y1 - 150;
						}
						j = (x1 - 25) / 50 + (y1 - 25) / 50 * 3;
					}

					// 有效点击
					if (((k == 1 && i == block.nextboxnum) || k == 0) && box[i][j].type == 0)
					{
						// 记录参数更新
						record.bigboxnum[record.flag] = i;
						record.smallboxnum[record.flag] = j;
						record.flag += 1;

						// 对小格子参数更新
						box[i][j].type = block.type;

						// 操作手参数更新
						block.nowboxnum = i;
						block.nextboxnum = j;

						// 闪烁显示
						// 绘制小棋子
						i = block.nowboxnum;
						j = block.nextboxnum;
						for (k = 0; k < 2; k++)
						{
							// 格子绘制
							setfillcolor(color.bkcolor[1]);
							setlinecolor(color.linecolor[1]);
							setlinestyle(PS_SOLID, 5);
							fillrectangle(box[i][j].posx, box[i][j].posy, box[i][j].posx + 50, box[i][j].posy + 50);
							FlushBatchDraw();
							Sleep(200);

							// 棋子绘制
							if (box[i][j].type == 1) { draw_crosses(25, box[i][j].posx + 25, box[i][j].posy + 25); }
							else if (box[i][j].type == 2) { draw_circle(25, box[i][j].posx + 25, box[i][j].posy + 25); }
							FlushBatchDraw();
							Sleep(200);
						}

						// 判断棋盘形势
						check_small(back_or_not);

						// 判断结束后，操作手参数更新
						block.type = block.type == 1 ? 2 : 1;

						// 绘制
						draw_scene_start();

						// 判断棋盘是否填满
						k = 0;
						for (i = 0; i <= 8; i++)
						{
							for (j = 0; j <= 8; j++)
							{
								if (box[i][j].type == 0)
								{
									k = 1;
									break;
								}
							}
							if (k == 1)break;
						}

						// 最后一步未触发胜利，且棋盘已满
						if (exit_start == 0 && k == 0)
						{
							// 正常退出，胜者为后手方，返回开始选择函数
							MessageBox(hOut, "圈圈后手胜利", "来自小豆子的提醒", MB_OK);
							exit_start = 1;
						}
						// 胜负已分
						else if (box[9][9].type != 0)
						{
							// 正常退出，弹出胜者，返回开始选择函数
							if (block.type == 2)
							{
								MessageBox(hOut, "叉叉十字架胜利", "来自小豆子的提醒", MB_OK);
							}
							else
							{
								MessageBox(hOut, "圈圈胜利", "来自小豆子的提醒", MB_OK);
							}
							exit_start = 1;
						}
					}
				}
				else
				{
					// 判断是否点击了按钮
					for (i = 0; i < 4; i++)
					{
						if (m.x > boxm[i].posx1 && m.y > boxm[i].posy1 && m.x < boxm[i].posx2 && m.y < boxm[i].posy2 && boxm[i].type == 0)
						{
							break;
						}
					}

					// 重开按钮
					if (i == 0)
					{
						// 开始进程初始化函数，主要是格子参数的初始化
						initialization_start();
						// 记录初始化
						record.flag = 0;
						// 操作手参数重置
						block.type = 1;
					}
					// 悔棋按钮
					else if (i == 1)
					{
						// 判断能不能悔棋
						if ((mod_start == 0 && record.flag > 0) || (record.flag > 1))
						{
							// 根据模式，判断悔几步，双人一步，人机两步
							if (mod_start == 0) { record.flag -= 1; }
							else { record.flag -= 2; }
							// 重置
							initialization_start();
							// 从头推导
							// 悔棋参数设置
							back_or_not = 1;
							for (k = 0; k < record.flag; k++)
							{
								// 赋值
								i = record.bigboxnum[k];
								j = record.smallboxnum[k];
								// 对小格子参数更新
								box[i][j].type = block.type;
								// 操作手参数更新
								block.nowboxnum = i;
								block.nextboxnum = j;
								// 判断棋盘形势
								check_small(back_or_not);
								// 判断结束后，操作手参数更新
								block.type = block.type == 1 ? 2 : 1;
								// 悔棋，棋盘必不满
							}
							back_or_not = 0;
						}
					}
					// 主题切换按钮
					else if (i == 2)
					{
						mod_scene_change();
					}
					// 返回按钮
					else if (i == 3)
					{
						exit_start = 1;
					}
					// 绘制
					draw_scene_start();
				}
			}
		}
	}
	// 特殊模式
	while (exit_start == -1)
	{
		if (MouseHit())
		{
			m = getmessage(EM_MOUSE | EM_KEY);
			// 左键单击判断
			if (m.message == WM_LBUTTONDOWN || (mod_start == 1 && block.type == 1) || (mod_start == 2 && block.type == 2))
			{
				// 单击在棋盘范围
				if ((m.x < 475 && m.x > 25 && m.y > 25 && m.y < 475) || (mod_start == 1 && block.type == 1) || (mod_start == 2 && block.type == 2))
				{
					// 人机模式
					if ((mod_start == 1 && block.type == 1) || (mod_start == 2 && block.type == 2))
					{
						k = 0;
						machine();
						i = ma[0];
						j = ma[1];
					}
					// 双人模式
					else if (m.x < 475 && m.x > 25 && m.y > 25 && m.y < 475)
					{
						// 识别点击了哪个格子
						i = (m.x - 25) / 150 + (m.y - 25) / 150 * 3;
						x1 = m.x; y1 = m.y;
						while (x1 > 175)
						{
							x1 = x1 - 150;
						}
						while (y1 > 175)
						{
							y1 = y1 - 150;
						}
						j = (x1 - 25) / 50 + (y1 - 25) / 50 * 3;
					}

					// 有效点击
					if (box[i][j].type == 0)
					{
						// 记录参数更新
						record.bigboxnum[record.flag] = i;
						record.smallboxnum[record.flag] = j;
						record.flag += 1;

						// 对小格子参数更新
						box[i][j].type = block.type;

						// 操作手参数更新
						block.nowboxnum = i;
						block.nextboxnum = j;

						// 闪烁显示
						// 绘制小棋子
						i = block.nowboxnum;
						j = block.nextboxnum;
						for (k = 0; k < 2; k++)
						{
							// 格子绘制
							setfillcolor(color.bkcolor[1]);
							setlinecolor(color.linecolor[1]);
							setlinestyle(PS_SOLID, 5);
							fillrectangle(box[i][j].posx, box[i][j].posy, box[i][j].posx + 50, box[i][j].posy + 50);
							FlushBatchDraw();
							Sleep(200);

							// 棋子绘制
							if (box[i][j].type == 1) { draw_crosses(25, box[i][j].posx + 25, box[i][j].posy + 25); }
							else if (box[i][j].type == 2) { draw_circle(25, box[i][j].posx + 25, box[i][j].posy + 25); }
							FlushBatchDraw();
							Sleep(200);
						}

						// 判断棋盘形势
						check_small(back_or_not);

						// 判断结束后，操作手参数更新
						block.type = block.type == 1 ? 2 : 1;

						// 绘制
						// 每次落子范围都是全局，所以这个参数一直为 9
						// 普通模式这个值只有第一次是 9，两个模式最大的区别
						block.nextboxnum = 9;
						draw_scene_start();

						// 判断棋盘是否填满
						k = 0;
						for (i = 0; i <= 8; i++)
						{
							for (j = 0; j <= 8; j++)
							{
								if (box[i][j].type == 0)
								{
									k = 1;
									break;
								}
							}
							if (k == 1)break;
						}
						// 最后一步未触发胜利，且棋盘已满
						if (exit_start == -1 && k == 0)
						{
							// 正常退出，胜者为后手方，返回开始选择函数
							MessageBox(hOut, "圈圈后手胜利", "来自小豆子的提醒", MB_OK);
							exit_start = 1;
						}
						// 胜负已分
						else if (box[9][9].type != 0)
						{

							// 正常退出，弹出胜者，返回开始选择函数
							if (block.type == 2)
							{
								MessageBox(hOut, "叉叉十字架胜利", "来自小豆子的提醒", MB_OK);
							}
							else
							{
								MessageBox(hOut, "圈圈胜利", "来自小豆子的提醒", MB_OK);
							}
							exit_start = 1;
						}
					}
				}
				else
				{
					// 判断是否点击了按钮
					for (i = 0; i < 4; i++)
					{
						if (m.x > boxm[i].posx1 && m.y > boxm[i].posy1 && m.x < boxm[i].posx2 && m.y < boxm[i].posy2 && boxm[i].type == 0)
						{
							break;
						}
					}

					// 重开按钮
					if (i == 0)
					{
						// 开始模式初始化函数
						initialization_start();
						// 记录初始化
						record.flag = 0;
						// 操作手参数重置
						block.type = 1;
					}
					// 悔棋按钮
					else if (i == 1)
					{
						// 判断能不能悔棋
						if ((mod_start == 0 && record.flag > 0) || (record.flag > 1))
						{
							// 根据模式，判断悔几步，双人一步，人机两步
							if (mod_start == 0) { record.flag -= 1; }
							else { record.flag -= 2; }
							// 重置
							initialization_start();
							// 从头推导
							// 悔棋参数设置
							back_or_not = 1;
							for (k = 0; k < record.flag; k++)
							{
								// 赋值
								i = record.bigboxnum[k];
								j = record.smallboxnum[k];
								// 对小格子参数更新
								box[i][j].type = block.type;
								// 操作手参数更新
								block.nowboxnum = i;
								block.nextboxnum = j;
								// 判断棋盘形势
								check_small(back_or_not);
								// 判断结束后，操作手参数更新
								block.type = block.type == 1 ? 2 : 1;
								// 悔棋，棋盘必不满
							}
							back_or_not = 0;
						}
					}
					// 主题切换按钮
					else if (i == 2)
					{
						mod_scene_change();
					}
					// 返回按钮
					else if (i == 3)
					{
						exit_start = 1;
					}
					// 绘制
					// 同理，这个值一直是 9
					block.nextboxnum = 9;
					draw_scene_start();
				}
			}
		}
	}
}



// 开始进程初始化函数
void game::initialization_start()
{
	int i, j;
	// 画布初始化
	setbkcolor(color.bkcolor[0]);
	cleardevice();
	// 格子初始化，81 个小格子，9 个大格子，来保存 9 个小井字棋状态；9 个小格子，1 个大格子，来保存 1 个大井字棋状态。共 100 个
	for (i = 0; i <= 8; i++)
	{
		for (j = 0; j <= 8; j++)
		{
			// 81 个小格的初始化
			box[i][j].bigboxnum = i;
			box[i][j].smallboxnum = j;
			box[i][j].type = 0;
			box[i][j].posx = i % 3 * 150 + j % 3 * 50 + 25;
			box[i][j].posy = i / 3 * 150 + j / 3 * 50 + 25;
		}
		// 9 个大格的初始化
		box[i][9].type = 0;
	}

	// 第 10 个大格的初始化，为了解决第一次点击，操作手参数不足的问题，第一次点击完就没有效果了
	for (j = 0; j < 9; j++)
	{
		box[9][j].type = 1;
	}

	// 棋盘参数初始化，即超大格参数初始化
	box[9][9].type = 0;

	// 操作手参数初始化，第一次是 9
	block.nowboxnum = 4;
	block.nextboxnum = 9;
	block.type = 1;

	// 按钮设置初始化
	num_button = 4;

	boxm[0].posx1 = 500; boxm[0].posy1 = 120; boxm[0].posx2 = 580; boxm[0].posy2 = 180; boxm[0].text = _T("重开"); boxm[0].type = 0;
	boxm[1].posx1 = 500; boxm[1].posy1 = 220; boxm[1].posx2 = 580; boxm[1].posy2 = 280; boxm[1].text = _T("悔棋"); boxm[1].type = 0;
	boxm[2].posx1 = 500; boxm[2].posy1 = 320; boxm[2].posx2 = 580; boxm[2].posy2 = 380; boxm[2].text = _T("主题"); boxm[2].type = 0;
	boxm[3].posx1 = 500; boxm[3].posy1 = 420; boxm[3].posx2 = 580; boxm[3].posy2 = 480; boxm[3].text = _T("返回"); boxm[3].type = 0;

}



// 开始进程绘制函数
void game::draw_scene_start()
{
	int i, j, k;

	// 对下次落子状态的判断
	k = 0;
	for (i = 0; i <= 8; i++)
	{
		if (box[block.nextboxnum][i].type == 0)
		{
			k = 1;
			break;
		}
	}

	// 绘制主背景
	setbkcolor(color.bkcolor[0]);
	cleardevice();

	// 绘制下次落子范围
	setfillcolor(color.bkcolor[1]);
	setlinecolor(color.linecolor[2]);
	setlinestyle(PS_SOLID, 5);
	// 对应大格内有可以落子的地方，则必须在对应大格落子
	if (k == 1)
	{
		fillrectangle(box[block.nextboxnum][0].posx, box[block.nextboxnum][0].posy, box[block.nextboxnum][0].posx + 150, box[block.nextboxnum][0].posy + 150);
	}
	// 对应大格内无可以落子的地方，则随意落子
	else if (k == 0)
	{
		fillrectangle(box[0][0].posx, box[0][0].posy, box[0][0].posx + 450, box[0][0].posy + 450);
	}

	// 绘制小格子线
	setlinecolor(color.linecolor[0]);
	setlinestyle(PS_SOLID, 5);
	for (i = 0; i < 9; i += 3)
	{
		// 横线
		line(box[i][3].posx, box[i][3].posy, box[i + 2][5].posx + 50, box[i + 2][5].posy);
		line(box[i][6].posx, box[i][6].posy, box[i + 2][8].posx + 50, box[i + 2][8].posy);
		// 竖线
		line(box[i / 3][1].posx, box[i / 3][1].posy, box[i / 3 + 6][7].posx, box[i / 3 + 6][7].posy + 50);
		line(box[i / 3][2].posx, box[i / 3][2].posy, box[i / 3 + 6][8].posx, box[i / 3 + 6][8].posy + 50);
	}

	// 绘制大格子线
	setlinecolor(color.linecolor[1]);
	for (i = 0; i < 9; i += 3)
	{
		// 横线
		line(box[i][0].posx, box[i][0].posy, box[i + 2][2].posx + 50, box[i + 2][2].posy);
		// 竖线
		line(box[i / 3][0].posx, box[i / 3][0].posy, box[i / 3 + 6][6].posx, box[i / 3 + 6][6].posy + 50);
	}
	i = 6;
	// 横线
	line(box[i][0].posx, box[i][0].posy + 150, box[i + 2][2].posx + 50, box[i + 2][2].posy + 150);
	// 竖线
	line(box[i / 3][0].posx + 150, box[i / 3][0].posy, box[i / 3 + 6][6].posx + 150, box[i / 3 + 6][6].posy + 50);

	// 绘制小棋子
	for (i = 0; i <= 8; i++)
	{
		for (j = 0; j <= 8; j++)
		{
			if (box[i][j].type == 1) { draw_crosses(25, box[i][j].posx + 25, box[i][j].posy + 25); }
			else if (box[i][j].type == 2) { draw_circle(25, box[i][j].posx + 25, box[i][j].posy + 25); }
		}
	}

	// 绘制大棋子
	for (i = 0; i <= 8; i++)
	{
		if (box[i][9].type != 0 && box[i][9].type != 3)
		{
			// 格子绘制
			setfillcolor(color.bkcolor[0]);
			setlinecolor(color.linecolor[1]);
			setlinestyle(PS_SOLID, 5);
			fillrectangle(box[i][0].posx, box[i][0].posy, box[i][8].posx + 50, box[i][8].posy + 50);

			// 棋子绘制
			if (box[i][9].type == 1) { draw_crosses(75, box[i][4].posx + 25, box[i][4].posy + 25); }
			else if (box[i][9].type == 2) { draw_circle(75, box[i][4].posx + 25, box[i][4].posy + 25); }
		}

	}

	// 绘制超大棋子
	if (box[9][9].type != 0)
	{
		// 格子绘制
		setfillcolor(color.bkcolor[0]);
		setlinecolor(color.linecolor[1]);
		setlinestyle(PS_SOLID, 5);
		fillrectangle(box[0][0].posx, box[0][0].posy, box[8][8].posx + 50, box[8][8].posy + 50);

		// 棋子绘制
		if (box[9][9].type == 1) { draw_crosses(225, box[4][4].posx + 25, box[4][4].posy + 25); }
		else if (box[9][9].type == 2) { draw_circle(225, box[4][4].posx + 25, box[4][4].posy + 25); }
	}

	// 绘制按钮
	draw_button();

	FlushBatchDraw();
}
