/////////////////////////////////////////////////////////
// 程序名称：AyaCrossX（十字消除游戏）
// 编译环境：Visual C++ 6.0 / 2012，EasyX 2013霜降版
// 作    者：ls9512 <http://www.baidu.com/p/ls9512>
// 最后修改：2013-11-2
//

//系统函数库
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <time.h>
//EasyX图形库
#include <graphics.h>


//////////////////////////////以下是 宏定义参数 //////////////////////////////
#define WIN_WIDTH		600					//屏幕宽度
#define WIN_HEIGHT		430					//屏幕高度
#define WIN_BLANKTOP	45					//屏幕顶部空边
#define WIN_BLANK		25					//屏幕边缘留空
#define G_BN			20					//方块横向数量
#define G_BM			12					//方块纵向数量
#define G_BW			20					//方块宽度
#define G_BS			8					//方块间隔宽度
#define G_BSELECT		3					//方块选中大小
#define G_FLOATW		4					//选中浮动大小
#define G_BDWO			1					//消失初始大小
#define G_BDW			6					//消失大小
#define G_CN			10					//颜色数量
#define G_BUL			25					//方块增量程度
#define C_BLOCKPANEL	RGB(255, 255, 221)	//底板颜色
#define C_HPBAR			RGB(255, 170, 255)	//HP条颜色


//////////////////////////////以下是数据结构定义//////////////////////////////
//方块结构
struct Block {
	COLORREF color;				//颜色
	float	 FLOAT;				//浮动大小
	float	 DIS;				//消除延迟
	bool	 isDIS;				//是否消失中
	bool	 isClick;			//是否被点击
};

//点结构
struct Point {
	int x;
	int y;
};

//矩形结构
struct Rect {
	int x;
	int y;
	int w;
	int h;
};


//////////////////////////////以下是  全局变量  //////////////////////////////
int GamePhase = 0;				//游戏阶段 0 准备 1 进行 2 结束 3 帮助界面
int isShowFPS = 1;				//是否显示帧数
int mouseX;						//鼠标位置坐标X
int mouseY;						//鼠标位置坐标Y
int time_max;					//总游戏时间
int time_now;					//当前剩余时间
int level;						//等级
int score;						//分数
bool isMouseDown;				//鼠标按下
Block block[G_BM][G_BN];		//方块数组
COLORREF Defcolor[G_CN];		//候选颜色数组


//////////////////////////////以下是  函数声明  //////////////////////////////
float	GetFPS();						//获取帧数
void	HpSleep(DWORD ms);				//绝对延时
Point	GetMouseLocal(int x, int y);	//获取鼠标所在坐标
void	AddBlock(int n);				//添加指定个数的未选中方块
void	Init();							//初始化
void	DisCheck(int x, int y);			//检测指定位置的消除
int		CountBlock();					//统计方块个数
void	Manager();						//逻辑处理
void	Draw();							//绘图处理
void	StartDraw();					//游戏开始绘图
void	GamingDraw();					//游戏进行中绘图
void	EndDraw();						//游戏结束绘图
void	HelpDraw();						//游戏帮助绘图
void	EndFrame();						//帧结束处理
bool	IsInRect(int x, int y, Rect r);	//是否在矩形内


//////////////////////////////以下是辅助功能函数//////////////////////////////
//计算画面FPS(每秒帧数)
float GetFPS()
{
#define FPS_COUNT 8
	static int i = 0;
	static DWORD oldTime = GetTickCount();
	static float fps;
	if (i > FPS_COUNT)
	{
		i = 0;
		int newTime = GetTickCount();
		int elapsedTime = newTime - oldTime;
		fps = FPS_COUNT / (elapsedTime / 1000.0f);
		oldTime = newTime;
	}
	i++;
	return fps;
}

//绝对延时
void HpSleep(DWORD ms)
{
	static clock_t oldclock = clock();		// 静态变量，记录上一次 tick

	oldclock += ms * CLOCKS_PER_SEC / 1000;	// 更新 tick

	if (clock() > oldclock)					// 如果已经超时，无需延时
		oldclock = clock();
	else
		while (clock() < oldclock)			// 延时
			Sleep(1);						// 释放 CPU 控制权，降低 CPU 占用率
}

//获取鼠标选中点
Point GetMouseLocal(int x, int y)
{
	Point p;
	x = (x - WIN_BLANK) / (G_BW + G_BS);
	y = (y - WIN_BLANK - WIN_BLANKTOP) / (G_BW + G_BS);
	if (x < 0 || x >= G_BN) x = -1;
	if (y < 0 || y >= G_BM) y = -1;
	p.x = x;
	p.y = y;
	return p;
}

//是否在矩形内
bool IsInRect(int x, int y, Rect r)
{
	return ((x >= r.x && x <= r.w) && (y >= r.y && y <= r.h));
}

//添加指定个数的未选中方块
void AddBlock(int n)
{
	//随机生成方块
	int num = 0, x1, y1, x2, y2;
	int num_max = n;
	while (num < num_max)
	{
		x1 = rand() % G_BN;
		y1 = rand() % G_BM;
		x2 = rand() % G_BN;
		y2 = rand() % G_BM;
		if (block[y1][x1].isClick && block[y2][x2].isClick)
		{
			COLORREF color = Defcolor[rand() % G_CN];
			block[y1][x1].color = color;
			block[y1][x1].isClick = false;
			block[y1][x1].FLOAT = 0;
			block[y1][x1].DIS = 0;
			block[y1][x1].isDIS = false;
			block[y2][x2].color = color;
			block[y2][x2].isClick = false;
			block[y2][x2].FLOAT = 0;
			block[y2][x2].DIS = 0;
			block[y2][x2].isDIS = false;
			num += 2;
		}
	}
}

//初始化
void Init()
{
	//预置颜色
	Defcolor[0] = RGB(255, 153, 0);
	Defcolor[1] = RGB(204, 102, 0);
	Defcolor[2] = RGB(27, 118, 255);
	Defcolor[3] = RGB(255, 136, 255);
	Defcolor[4] = RGB(204, 204, 102);
	Defcolor[5] = RGB(104, 204, 204);
	Defcolor[6] = RGB(255, 125, 125);
	Defcolor[7] = RGB(204, 104, 204);
	Defcolor[8] = RGB(190, 190, 190);
	Defcolor[9] = RGB(0, 204, 0);

	//初始化时间
	time_max = 10000;
	time_now = 10000;

	//初始化分数
	score = 0;

	//置全选中
	for (int i = 0; i < G_BM; i++)
	{
		for (int j = 0; j < G_BN; j++)
		{
			block[i][j].isClick = true;
			block[i][j].FLOAT = 0;
			block[i][j].DIS = 0;
		}
	}

	//添加方块
	AddBlock(G_BN * G_BM * 2 / 3);

	//清除鼠标消息队列
	FlushMouseMsgBuffer();
}

//统计方块个数
int CountBlock()
{
	int n = 0;
	for (int i = 0; i < G_BN; i++)
	{
		for (int j = 0; j < G_BM; j++)
		{
			if (!block[j][i].isClick) n++;
		}
	}
	return n;
}

//检测指定位置消除
void DisCheck(int x, int y)
{
	//记录待消除的方块
	Block* b[4];
	int i, j, n = 0;

	//向左
	for (i = x; i >= 0; i--)
	{
		if (!block[y][i].isClick)
		{
			b[n] = &block[y][i];
			n++;
			break;
		}
	}

	//向右
	for (i = x; i < G_BN; i++)
	{
		if (!block[y][i].isClick)
		{
			b[n] = &block[y][i];
			n++;
			break;
		}
	}

	//向上
	for (j = y; j >= 0; j--)
	{
		if (!block[j][x].isClick)
		{
			b[n] = &block[j][x];
			n++;
			break;
		}
	}

	//向下
	for (j = y; j < G_BM; j++)
	{
		if (!block[j][x].isClick)
		{
			b[n] = &block[j][x];
			n++;
			break;
		}
	}

	//消除
	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (b[i]->color == b[j]->color)
			{
				if (!b[i]->isDIS) {
					b[i]->DIS = G_BDWO;
					b[i]->isDIS = true;
				}
				if (!b[j]->isDIS) {
					b[j]->DIS = G_BDWO;
					b[j]->isDIS = true;
				}
			}
		}
	}
	isMouseDown = false;
}

//逻辑处理
void Manager()
{
	//如果在游戏中
	if (GamePhase == 1)
	{
		Point p = GetMouseLocal(mouseX, mouseY);
		int x = p.x, y = p.y;
		//如果选中
		if (p.x != -1 && p.y != -1)
		{
			//如果单击
			if (isMouseDown && block[y][x].isClick)
			{
				//触发消除检测
				DisCheck(x, y);
			}
		}
		//添加新方块添加随机个数新方块
		if (CountBlock() < G_BN * G_BM / 3)
		{
			AddBlock((rand() % 15 + 15) * 2);
			//时间恢复
			time_now += 500;
			if (time_now > time_max) time_now = time_max;
		}
	}
}

//开始绘图
void StartDraw()
{
	settextstyle(52, 30, _T("Impact"));
	//标题
	settextcolor(RGB(255, 17, 102));
	outtextxy(143, 93, _T("AyaCrossX"));
	settextcolor(RGB(255, 153, 238));
	outtextxy(140, 90, _T("AyaCrossX"));
	//版本
	settextstyle(22, 10, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(340, 153, _T("v0.91 By:ls9512"));
	//开始游戏按钮
	Rect r;
	r.x = 240;
	r.y = 220;
	r.w = r.x + 125;
	r.h = r.y + 35;
	setlinecolor(RGB(255, 17, 102));
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(255, 187, 119));
		//开始游戏按键响应
		if (isMouseDown)
		{
			Init();
			GamePhase = 1;
			isMouseDown = false;
		}
	}
	else
		setfillcolor(RGB(255, 255, 204));
	fillrectangle(r.x, r.y, r.w, r.h);
	settextstyle(25, 10, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(r.x + 5, r.y + 5, _T("PLAY GAME"));

	//帮助游戏按钮
	r.x = 240;
	r.y = 270;
	r.w = r.x + 125;
	r.h = r.y + 35;
	setlinecolor(RGB(255, 17, 102));
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(255, 187, 119));
		//帮助按键响应
		if (isMouseDown) GamePhase = 3;
	}
	else
		setfillcolor(RGB(255, 255, 204));
	fillrectangle(r.x, r.y, r.w, r.h);
	settextstyle(25, 10, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(r.x + 5, r.y + 5, _T("HELP INFO"));
	//退出游戏按钮
	r.x = 240;
	r.y = 320;
	r.w = r.x + 125;
	r.h = r.y + 35;
	setlinecolor(RGB(255, 17, 102));
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(255, 187, 119));
		//退出按键响应
		if (isMouseDown) exit(0);
	}
	else
		setfillcolor(RGB(255, 255, 204));
	fillrectangle(r.x, r.y, r.w, r.h);
	settextstyle(25, 10, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(r.x + 5, r.y + 5, _T("EXIT GAME"));
	//说明
	settextstyle(16, 5, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(175, 400, _T("Programing By VC++ & EasyX     Date:2013.11.01"));
}

//游戏中绘图
void GameingDraw()
{
	int x, y;
	COLORREF c, c2;

	//时间条
	x = WIN_BLANK;
	y = WIN_BLANK;
	int HPBARW = (G_BW + G_BS) * G_BN - 200;
	setlinecolor(RGB(255, 17, 255));
	setfillcolor(WHITE);
	fillrectangle(x, y, x + HPBARW, y + 10);
	setfillcolor(C_HPBAR);
	fillrectangle(x, y, x + (int)(HPBARW * (1.0 * time_now / time_max)), y + 10);

	//底板
	setlinecolor(RGB(255, 25, 22));
	setfillcolor(C_BLOCKPANEL);
	x = WIN_BLANK - G_BS;
	y = WIN_BLANK + WIN_BLANKTOP - G_BS;
	fillrectangle(x, y, x + (G_BW + G_BS) * G_BN + G_BS, y + (G_BW + G_BS) * G_BM + G_BS);

	//画方块
	for (int i = 0; i < G_BM; i++)
	{
		for (int j = 0; j < G_BN; j++)
		{
			if (!block[i][j].isClick)
			{
				c = block[i][j].color;
				c = RGB(GetRValue(c) - (BYTE)G_BUL, GetGValue(c) - (BYTE)G_BUL, GetBValue(c) - (BYTE)G_BUL);
				int fw = (int)block[i][j].FLOAT;
				int fd = (int)block[i][j].DIS;
				setlinecolor(c);
				setfillcolor(block[i][j].color);
				x = j * (G_BW + G_BS) + WIN_BLANK - fw - fd;
				y = i * (G_BW + G_BS) + WIN_BLANK + WIN_BLANKTOP - fw - fd;
				fillrectangle(x, y, x + G_BW + 2 * (fw + fd), y + G_BW + 2 * (fw + fd));
				//浮动缩减
				if (block[i][j].FLOAT > 0) block[i][j].FLOAT -= 0.4f;
				//消失延迟
				if (block[i][j].isDIS)
				{
					if (block[i][j].DIS < G_BDW)
						block[i][j].DIS += 0.4f;
					else
					{
						//消除
						block[i][j].isClick = true;
						//加分
						score += 100;
						time_now += 25;
						if (time_now > time_max) time_now = time_max;
					}
				}
			}
		}
	}

	//获取鼠标位置
	Point p = GetMouseLocal(mouseX, mouseY);

	//画出鼠标光标
	if (p.x != -1 && p.y != -1)
	{
		c = RED;
		x = p.x, y = p.y;
		int fw;
		if (block[y][x].isClick) fw = 0;
		else fw = (int)block[y][x].FLOAT;
		x = x * (G_BW + G_BS) + WIN_BLANK - G_BSELECT - fw;
		y = y * (G_BW + G_BS) + WIN_BLANK + WIN_BLANKTOP - G_BSELECT - fw;
		//选中方块加亮
		if (!block[p.y][p.x].isClick)
		{
			//置浮动大小
			block[p.y][p.x].FLOAT = G_FLOATW;
			c2 = block[p.y][p.x].color;
		}
		else
		{
			c2 = C_BLOCKPANEL;
		}
		setlinecolor(c);
		setfillcolor(c2);
		fillrectangle(x, y, x + G_BW + 2 * (G_BSELECT + fw), y + G_BW + 2 * (G_BSELECT + fw));
	}

	//画出分数
	settextstyle(25, 10, _T("Verdana"));
	settextcolor(RGB(155, 51, 68));
	TCHAR b[10];
#if _MSC_VER > 1200
	_stprintf_s(b, _T("%d"), score);
#else
	_stprintf(b, _T("%d"), score);
#endif
	outtextxy(390, 15, _T("分数:"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(440, 16, b);
}

//游戏结束绘图
void EndDraw()
{
	//标题
	settextstyle(52, 30, _T("Impact"));
	settextcolor(RGB(255, 17, 102));
	outtextxy(143, 103, _T("GAME OVER"));
	settextcolor(RGB(215, 193, 238));
	outtextxy(140, 100, _T("GAME OVER"));

	//版本
	settextstyle(34, 12, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	TCHAR b[10];
#if _MSC_VER > 1200
	_stprintf_s(b, _T("%d"), score);
#else
	_stprintf(b, _T("%d"), score);
#endif
	outtextxy(232, 173, _T("分数:"));
	outtextxy(290, 173, b);

	//重新开始游戏按钮
	Rect r;
	r.x = 240;
	r.y = 240;
	r.w = r.x + 125;
	r.h = r.y + 35;
	setlinecolor(RGB(255, 17, 102));
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(255, 187, 119));
		//开始游戏按键响应
		if (isMouseDown)
		{
			Init();
			GamePhase = 1;
		}
	}
	else
		setfillcolor(RGB(255, 255, 204));
	fillrectangle(r.x, r.y, r.w, r.h);
	settextstyle(25, 10, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(r.x + 5, r.y + 5, _T("   REPLAY"));

	//退出
	r.x = 240;
	r.y = 290;
	r.w = r.x + 125;
	r.h = r.y + 35;
	setlinecolor(RGB(255, 17, 102));
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(255, 187, 119));
		//退出按键响应
		if (isMouseDown) exit(0);
	}
	else
		setfillcolor(RGB(255, 255, 204));
	fillrectangle(r.x, r.y, r.w, r.h);
	settextstyle(25, 10, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(r.x + 5, r.y + 5, _T("EXIT GAME"));
}

//游戏帮助绘图
void HelpDraw()
{
	//标题
	settextstyle(52, 30, _T("Impact"));
	settextcolor(RGB(255, 17, 152));
	outtextxy(233, 33, _T("HELP"));
	settextcolor(RGB(215, 193, 238));
	outtextxy(230, 30, _T("HELP"));

	//底板
	setlinecolor(RGB(255, 17, 204));
	setfillcolor(RGB(255, 221, 255));
	fillrectangle(60, 100, 550, 280);

	//版本
	settextstyle(18, 10, _T("黑体"));
	settextcolor(RGB(245, 126, 92));
	outtextxy(70, 110, _T("十字消除是一款经典消除类小游戏。"));
	outtextxy(70, 145, _T("【玩法】"));
	outtextxy(80, 170, _T("1.点击空白处,所在横纵十字线上同色方块会被消除。"));
	outtextxy(80, 195, _T("2.消除方块会累加得分,并恢复一定量的时间."));
	outtextxy(80, 220, _T("3.方块数量少于某数值会出现新方块并回复时间."));
	outtextxy(80, 245, _T("4.时间耗尽时则游戏结束."));

	//返回
	Rect r;
	r.x = 240;
	r.y = 360;
	r.w = r.x + 125;
	r.h = r.y + 35;
	setlinecolor(RGB(255, 17, 102));
	if (IsInRect(mouseX, mouseY, r))
	{
		setfillcolor(RGB(255, 187, 119));
		//返回按键响应
		if (isMouseDown) GamePhase = 0;
	}
	else
		setfillcolor(RGB(255, 255, 204));
	fillrectangle(r.x, r.y, r.w, r.h);
	settextstyle(25, 9, _T("Verdana"));
	settextcolor(RGB(255, 51, 68));
	outtextxy(r.x + 5, r.y + 5, _T("BACK MENU"));
}

//输出帧数
void DrawFPS()
{
	//输出帧数
	if (isShowFPS)
	{
		settextcolor(RED);
		settextstyle(14, 0, _T("宋体"));
		TCHAR s[5];
#if _MSC_VER > 1200
		_stprintf_s(s, _T("%.1f"), GetFPS());
#else
		_stprintf(s, _T("%.1f"), GetFPS());
#endif
		outtextxy(0, 0, s);
	}
}

//绘图
void Draw()
{
	//清屏
	setlinecolor(WHITE);
	setfillcolor(WHITE);
	fillrectangle(0, 0, WIN_WIDTH, WIN_HEIGHT);
	switch (GamePhase)
	{
	case 0:	StartDraw();	break;
	case 1:	GameingDraw();	break;
	case 2:	EndDraw();		break;
	case 3:	HelpDraw();		break;
	}
	DrawFPS();
}

//帧结束处理
void EndFrame()
{
	time_now -= 2;
	//游戏结束
	if (time_now <= 0 && GamePhase == 1)
	{
		GamePhase = 2;
	}
}

//主入口函数
int main()
{
	//置随机数种子
	srand((unsigned)time(NULL));
	//初始化设备，加载图片
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	//设置窗口标题
	SetWindowText(GetHWnd(), _T("AyaCrossX v0.91 By:ls9512"));
	cleardevice();
	//设置黑色背景
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	//开启双缓冲,防止闪屏
	BeginBatchDraw();
	// 鼠标消息变量
	MOUSEMSG mmsg;

	//初始化
	Init();

	while (true)
	{
		//处理鼠标消息
		while (MouseHit())
		{
			mmsg = GetMouseMsg();
			switch (mmsg.uMsg)
			{
			case WM_MOUSEMOVE:		mouseX = mmsg.x; mouseY = mmsg.y;	break;
			case WM_LBUTTONDOWN:	isMouseDown = true;					break;
			case WM_LBUTTONUP:		isMouseDown = false;				break;
			}
		}
		//逻辑处理
		Manager();
		//绘图处理
		Draw();
		//显示缓存的绘制内容
		FlushBatchDraw();
		//帧结束处理
		EndFrame();
		//延迟,帧数控制
		HpSleep(18);
	}

	// 关闭
	EndBatchDraw();
	closegraph();
	return 0;
}