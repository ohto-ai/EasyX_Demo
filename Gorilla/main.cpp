// 程序名称：扔香蕉的大猩猩
// 编译环境：Visual C++ 6.0 / 2010，EasyX_20130130(beta)
// 作　　者：yangw80 <yw80@qq.com>
// 最后修改：2013-3-3 (2012-9-23)
// 本游戏是仿照微软很古老的一个游戏写的。
// 游戏说明：你的任务是使用香蕉击中你的对手。
// 　　　　　可以调整扔的角度和力度，同时香蕉还受风速、重力加速度和楼宇的影响。
//
#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <math.h>


// 定义常量
#define	PI				3.1415926535	// 圆周率
#define	SCRWIDTH		320				// 屏幕宽度
#define SCRHEIGHT		480				// 屏幕高度
#define	GRAVITY			9.8				// 重力加速度
#define	BACKATTR		BLUE			// 背景的颜色
#define	OBJECTCOLOR		0x55AAFF		// 对手的颜色
#define	EXPLOSIONCOLOR	0x5500FF		// 爆炸的颜色
#define	SUNATTR			0x00FFFF		// 太阳的颜色
#define	SUNHEIGHT		40				// 太阳的高度
#define	SUNHAPPY		true			// 太阳高兴
#define	SUNSHOCK		false			// 太阳受惊


// 全局变量
IMAGE	g_imgBanana[4];		// 香蕉图片
IMAGE	g_imgGorD;			// 大猩猩(双手放下)
IMAGE	g_imgGorL;			// 大猩猩(左边的手抬起)
IMAGE	g_imgGorR;			// 大猩猩(右边的手抬起)
POINT	g_ptGorilla[2];		// 两个游戏者的位置
int		g_iLastBuilding;	// 最后一栋楼的编号
int		g_iWind;			// 风力
bool	g_bSunHit;			// 是否击中太阳


// 函数定义
void Init();												// 初始化
void Intro();												// 游戏介绍
void PlayGame(LPCTSTR player1, LPCTSTR player2);			// 主游戏函数
void MakeCityScape(POINT* aryBCoor);						// 创建随机的游戏场景
void PlaceGorillas(POINT* aryBCoor);						// 将游戏者放到楼宇顶端
void DoSun(bool smile);										// 绘制太阳
bool DoShot(int idPlayer, int x, int y, int* win);			// 接收游戏者输入，实现扔香蕉攻击对方
int  PlotShot(int startX, int startY, double angle, int velocity, int idPlayer);	// 进行香蕉攻击，使香蕉划过屏幕
void DrawBanana(int x, int y, int r, bool d);				// 绘制香蕉
void DoExplosion(int x, int y);								// 香蕉攻击后的爆炸效果
int  ExplodeGorilla(int x, int y);							// 游戏者死亡后爆炸
void VictoryDance(int idPlayer);							// 绘制跳舞的大猩猩(胜利后执行)



// 主函数
int main()
{
	Init();
	Intro();
	PlayGame(_T("朱广锋"), _T("周欢"));

	return 0;
}



// 初始化变量
void Init()
{
	initgraph(SCRWIDTH, SCRHEIGHT);	// 创建绘图窗口

	// 初始化香蕉图案
	IMAGE tmp;
	loadimage(&tmp, _T("rc/Banana.gif"));
	SetWorkingImage(&tmp);
	getimage(&g_imgBanana[0], 0, 0, 9, 7);
	getimage(&g_imgBanana[1], 9, 0, 9, 7);
	getimage(&g_imgBanana[2], 18, 0, 9, 7);
	getimage(&g_imgBanana[3], 27, 0, 9, 7);

	// 初始化大猩猩图案
	loadimage(&tmp, _T("rc/Gorilla.gif"), 0, 0, true);
	SetWorkingImage(&tmp);
	getimage(&g_imgGorD, 0, 0, 30, 30);
	getimage(&g_imgGorL, 30, 0, 30, 30);
	getimage(&g_imgGorR, 60, 0, 30, 30);
	SetWorkingImage(NULL);
}



// 显示游戏介绍
void Intro()
{
	settextstyle(24, 0, _T("宋体"));

	// 在屏幕中央输出字符串
	RECT r = { 0, 40, 640, 80 };
	drawtext(_T("扔香蕉的大猩猩"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	settextstyle(16, 0, _T("System"));
	r.top = 120;
	r.bottom = 480;
	drawtext(_T("这个游戏模仿的微软在 20 多年前的一个小游戏，\n不知道谁也有印象呢？\n\n")
		_T("你的任务是用香蕉击中你的对手。\n你可以通过鼠标调整投掷香蕉的角度和力度，\n")
		_T("香蕉会受重力加速度的影响。\n同时，请注意屏幕底部表示风力的箭头，")
		_T("香蕉同样会受风力影响。\n风力的箭头越长，表示风力越强。\n")
		_T("还有，周围的楼宇会阻挡你的香蕉。\n"),
		&r, DT_CENTER | DT_VCENTER);

	r.top = 400;
	drawtext(_T("按任意键继续"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	_getch();
}



// 主游戏函数
// 参数：
//		player1, player2：游戏者名称
void PlayGame(LPCTSTR player1, LPCTSTR player2)
{
	POINT	aryBCoor[31];				// 楼宇群的坐标
	int		aryScore[2] = { 0, 0 };		// 两个游戏者的得分
	TCHAR	sScore[20];					// 保存得分的字符串
	int		player = 0;					// 攻击者

	setbkcolor(BACKATTR);

	while (true)
	{
		cleardevice();
		srand((unsigned int)time(NULL));
		MakeCityScape(aryBCoor);
		PlaceGorillas(aryBCoor);
		DoSun(SUNHAPPY);

		bool bHit = false;
		while (bHit == false)
		{
			setcolor(WHITE);
			RECT r = { 0, 0, SCRWIDTH, 20 };
			drawtext(player1, &r, DT_LEFT | DT_SINGLELINE);
			drawtext(player2, &r, DT_RIGHT | DT_SINGLELINE);
			r.top = SCRHEIGHT - 40;
			r.bottom = SCRHEIGHT - 20;
			_stprintf(sScore, _T("%d >Score< %d"), aryScore[0], aryScore[1]);
			drawtext(sScore, &r, DT_CENTER | DT_SINGLELINE);

			int win;

			// 进行攻击。击中任意游戏者即返回 true。同时，更新 win 为胜利者
			bHit = DoShot(player, g_ptGorilla[player].x, g_ptGorilla[player].y, &win);

			// 如果太阳被击中，重绘太阳
			if (g_bSunHit)		DoSun(SUNHAPPY);

			// 如果击中对手，更新分数
			if (bHit == true)	aryScore[win]++;

			// 交替攻击
			player = 1 - player;

			Sleep(100);
		}
		Sleep(1000);
	};
}



// 创建随机的游戏场景
// 参数：
//		aryBCoor[]：存储每一栋楼的左上角坐标
void MakeCityScape(POINT* aryBCoor)
{
	int x = -10;

	// 设置随机的楼群倾斜的趋势
	int slope = rand() % 6;
	int iNewHt;				// 新楼的高度
	switch (slope)
	{
	case 0:	iNewHt = 15;	break;		// 逐渐升高
	case 1: iNewHt = 130;	break;		// 逐渐降低
	case 2:
	case 3:
	case 4: iNewHt = 15;	break;		// 倒 "V" 型(比较常见)
	case 5: iNewHt = 130;	break;		// "V" 型
	}

	int iBottomLine = 465;			// 建筑的最低端
	int iHtInc = 10;				// 高度增加值
	int iDefBWidth = 37;			// 默认的建筑宽度
	int iRandomHeight = 120;		// 随机的高度差异
	int iWWidth = 3;				// 窗户宽度
	int iWHeight = 6;				// 窗户高度
	int iWDifV = 15;				// 窗户的垂直间距
	int iWDifH = 10;				// 窗户的水平间距

	int iCurBuilding = 0;

	do
	{
		switch (slope)
		{
		case 0:		iNewHt += iHtInc;	break;
		case 1:		iNewHt -= iHtInc;	break;
		case 2:
		case 3:
		case 4:		if (x > SCRWIDTH / 2)	iNewHt -= 2 * iHtInc;
			  else					iNewHt += 2 * iHtInc;
			break;
		case 5:		if (x > SCRWIDTH / 2)	iNewHt += 2 * iHtInc;
			  else					iNewHt -= 2 * iHtInc;
			break;
		}

		// 设置楼宇宽度，并检查是否超出屏幕
		int iBWidth = iDefBWidth + rand() % iDefBWidth;

		// 设置楼宇高度，并检查楼宇是否超出屏幕下方
		int iBHeight = iNewHt + rand() % iRandomHeight;
		if (iBHeight < iHtInc)
			iBHeight = iHtInc;

		// 检查楼宇是否太高
		if (iBottomLine - iBHeight <= 25)
			iBHeight = 20;

		// 保存楼的坐标
		aryBCoor[iCurBuilding].x = x;
		aryBCoor[iCurBuilding].y = iBottomLine - iBHeight;

		// 绘制楼宇
		COLORREF aryBuildingColor[3] = { CYAN, LIGHTGRAY, RED };	// 定义楼宇的三种颜色
		int colorID = rand() % 3;
		setcolor(BACKATTR);
		rectangle(x - 1, iBottomLine + 1, x + iBWidth + 1, iBottomLine - iBHeight - 1);
		setfillcolor(aryBuildingColor[colorID]);
		bar(x, iBottomLine, x + iBWidth, iBottomLine - iBHeight);

		// 绘制窗户
		int c = x + 3;
		do
		{
			for (int i = iBHeight - 3; i >= 7; i -= iWDifV)
			{
				int winColor;
				if (rand() % 4 == 0)
					winColor = DARKGRAY;
				else
					winColor = YELLOW;

				setfillcolor(winColor);
				bar(c, iBottomLine - i, c + iWWidth, iBottomLine - i + iWHeight);
			}
			c += iWDifH;
		} while (c < x + iBWidth - 3);

		x += iBWidth + 2;

		iCurBuilding++;
	} while (x < SCRWIDTH - 1);

	g_iLastBuilding = iCurBuilding - 1;		// 保存最后一栋楼的编号

	// 设置随机风力
	g_iWind = rand() % 61 - 30;

	// 绘制风向箭头
	if (g_iWind != 0)
	{
		int windLine = g_iWind * 3 * (SCRWIDTH / 320);
		setcolor(EXPLOSIONCOLOR);
		int arrowDir = (g_iWind > 0) ? -2 : 2;
		line(SCRWIDTH / 2, SCRHEIGHT - 5, SCRWIDTH / 2 + windLine, SCRHEIGHT - 5);
		line(SCRWIDTH / 2 + windLine, SCRHEIGHT - 5, SCRWIDTH / 2 + windLine + arrowDir, SCRHEIGHT - 5 - 2);
		line(SCRWIDTH / 2 + windLine, SCRHEIGHT - 5, SCRWIDTH / 2 + windLine + arrowDir, SCRHEIGHT - 5 + 2);
	}
}



// 将游戏者放到楼宇顶端(从边缘数第二个或第三个楼宇上)
// 参数：
//		aryBCoor[]：楼宇数组。保存每栋楼的左上角坐标
void PlaceGorillas(POINT* aryBCoor)
{
	for (int i = 0; i <= 1; i++)
	{
		int iBNum = (i == 0) ? rand() % 2 + 1 : g_iLastBuilding - 1 - rand() % 2;
		int iBWidth = aryBCoor[iBNum + 1].x - aryBCoor[iBNum].x;

		g_ptGorilla[i].x = aryBCoor[iBNum].x + iBWidth / 2 - g_imgGorD.getwidth() / 2;
		g_ptGorilla[i].y = aryBCoor[iBNum].y - g_imgGorD.getheight();
		putimage(g_ptGorilla[i].x, g_ptGorilla[i].y, &g_imgGorD);
	}
}



// 绘制太阳
// 参数：
//		smile：太阳是否微笑
void DoSun(bool smile)
{
	// 设置太阳的位置
	int x = SCRWIDTH / 2;
	int y = SUNHEIGHT - 15;

	// 绘制太阳
	// 脸
	setcolor(SUNATTR);
	setfillcolor(SUNATTR);
	fillcircle(x, y, 12);

	// 光芒
	for (double a = 0; a < PI * 2; a += PI / 8)
		line(x, y, int(x + cos(a) * 20 + 0.5), int(y + sin(a) * 16 + 0.5));

	// 嘴
	setcolor(BACKATTR);
	setfillcolor(BACKATTR);
	if (smile)		// 绘制笑脸
		arc(x - 8, y - 8, x + 8, y + 8, (210 * PI / 180), (330 * PI / 180));
	else			// 绘制受惊表情("o" 型嘴)
		fillcircle(x, y + 5, 3);

	// 眼睛
	fillcircle(x - 3, y - 2, 1);
	fillcircle(x + 3, y - 2, 1);
}



// 接收游戏者输入，实现扔香蕉攻击对方
// 参数：
//		idPlayer：游戏者(准备扔香蕉的)
//		x, y：游戏者的位置
bool DoShot(int idPlayer, int x, int y, int* win)
{
	// 清空鼠标消息缓冲区
	FlushMouseMsgBuffer();

	// 攻击的起始位置
	int startx = x + (idPlayer == 1 ? g_imgGorD.getwidth() : 0);
	int starty = y;
	// 角度辅助线的位置
	int mx = startx, my = starty - 90;
	int oldmx = mx, oldmy = my;

	double angle = PI / 2;	// 投掷角度
	int velocity = 2;		// 投掷力度

	setwritemode(R2_XORPEN);
	setcolor(RED);
	line(startx, starty, mx, my);

	// 鼠标输入攻击角度
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();

		if (msg.uMsg == WM_MOUSEMOVE)
		{
			if (msg.y > y)
			{
				mx = startx + (msg.x > startx ? 90 : -90);
				my = starty;
				angle = msg.x > startx ? 0 : PI;
			}
			else if (msg.x != startx)
			{
				angle = atan(double(starty - msg.y) / (msg.x - startx));
				if (angle < 0)	angle += PI;
				mx = startx + int(cos(angle) * 90 + 0.5);
				my = starty - int(sin(angle) * 90 + 0.5);
			}
			else
			{
				mx = msg.x;
				my = y - 90;
				angle = PI / 2;
			}
			line(startx, starty, oldmx, oldmy);
			line(startx, starty, mx, my);
			oldmx = mx;
			oldmy = my;
		}
		else if (msg.uMsg == WM_LBUTTONDOWN)
			break;
	}
	line(startx, starty, oldmx, oldmy);

	// 鼠标输入攻击力度
	setlinestyle(PS_SOLID, 8);
	oldmx = mx = startx + int(cos(angle) * velocity + 0.5);
	oldmy = my = starty - int(sin(angle) * velocity + 0.5);
	line(startx, starty, mx, my);
	while (true)
	{
		if (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONUP)
				break;
		}

		mx = startx + int(cos(angle) * velocity + 0.5);
		my = starty - int(sin(angle) * velocity + 0.5);
		line(startx, starty, oldmx, oldmy);
		line(startx, starty, mx, my);
		oldmx = mx;
		oldmy = my;

		if (++velocity > 90)		velocity = 2;

		Sleep(20);
	}
	velocity *= 2;		// 力度扩大一倍
	line(startx, starty, oldmx, oldmy);
	// 恢复设置
	setlinestyle(PS_SOLID, 1);
	setwritemode(R2_COPYPEN);

	// 实施攻击
	g_bSunHit = false;
	int iPlayerHit = PlotShot(x, y, angle, velocity, idPlayer);

	// 攻击结果
	if (iPlayerHit == -1)
	{
		*win = -1;
		return false;
	}
	else
	{
		*win = (iPlayerHit == idPlayer) ? 1 - idPlayer : idPlayer;
		VictoryDance(*win);
		return true;
	}
}



// 进行香蕉攻击，使香蕉划过屏幕
// 参数：
//		startX, startY：游戏者(扔香蕉的)的坐标
//		angle：扔出的方向(弧度)
//		velocity：扔出的力度
//		idPlayer：游戏者(扔香蕉的)
int PlotShot(int startX, int startY, double angle, int velocity, int idPlayer)
{
	// 投掷力量在 x、y 方向上的分量
	double initXVel = cos(angle) * velocity;
	double initYVel = sin(angle) * velocity;

	double x, y;
	double oldx = startX;
	double oldy = startY;

	// 绘制游戏者(投掷动作)
	putimage(startX, startY, idPlayer == 0 ? &g_imgGorL : &g_imgGorR);
	Sleep(100);
	// 绘制游戏者(站立动作)
	putimage(startX, startY, &g_imgGorD);

	bool bImpact = false;		// 是否碰撞
	bool bShotInSun = false;	// 是否击中太阳
	bool bOnScreen = true;		// 香蕉是否在屏幕上
	int  iPlayerHit = -1;		// 是否击中对手(-1：未击中；0、1：被击中者的 ID)
	bool bNeedErase = false;	// 是否需要擦掉旧香蕉
	POINT look[4];				// 碰撞检测的位置(香蕉中心上下左右四个边的中点)
	look[2].x = 0;	look[3].x = g_imgBanana[0].getwidth() - 1;
	look[0].x = look[1].x = look[3].x / 2;
	look[0].y = 0;	look[1].y = g_imgBanana[0].getheight() - 1;
	look[2].y = look[3].y = look[1].y / 2;

	int startXPos = startX;
	int startYPos = startY - g_imgBanana[0].getheight();

	if (idPlayer == 1)
		startXPos = startXPos + g_imgGorD.getwidth() - g_imgBanana[0].getwidth();

	int pointColor;
	int rot;
	double t = 0;
	while (!bImpact && bOnScreen)
	{
		// 擦掉旧香蕉
		if (bNeedErase)
		{
			bNeedErase = false;
			DrawBanana(int(oldx + 0.5), int(oldy + 0.5), -1, false);
		}

		x = startXPos + (initXVel * t) + (g_iWind / 5.0 * t * t);
		y = startYPos + (-1 * (initYVel * t) + (GRAVITY * t * t));

		if ((x >= SCRWIDTH - 10) || (x <= 3) || (y >= SCRHEIGHT - 3))
			bOnScreen = false;

		if (bOnScreen && y > 0)
		{
			// 检测是否击中(对香蕉中心上下左右四个边的中点做检测)
			for (int i = 0; i < 4; i++)
			{
				pointColor = getpixel(int(x + look[i].x + 0.5), int(y + look[i].y + 0.5));
				if (pointColor == BACKATTR || pointColor == WHITE)		// 目标是背景色或白色字幕，未击中
				{
					bImpact = false;
					if (bShotInSun == true && (abs(SCRWIDTH / 2 - int(x)) > 20 || y > SUNHEIGHT))
						bShotInSun = false;
				}
				else if (pointColor == SUNATTR && y < SUNHEIGHT)	// 击中太阳
				{
					if (!g_bSunHit)
						DoSun(SUNSHOCK);

					g_bSunHit = true;
					bShotInSun = true;
				}
				else
					bImpact = true;

				if (bImpact)
					break;
			}

			if (!bShotInSun && !bImpact)
			{
				// 绘制香蕉
				rot = int(t * 10) % 4;
				DrawBanana(int(x + 0.5), int(y + 0.5), rot, true);
				bNeedErase = true;
			}

			oldx = x;
			oldy = y;
		}
		t += 0.1;

		Sleep(50);
	}

	if (pointColor != OBJECTCOLOR && bImpact)
		DoExplosion(int(x + g_imgBanana[0].getwidth() / 2 + 0.5), int(y + g_imgBanana[0].getheight() / 2 + 0.5));
	else if (pointColor == OBJECTCOLOR)
		iPlayerHit = ExplodeGorilla(int(x + 0.5), int(y + 0.5));

	return iPlayerHit;
}



// 绘制香蕉
// 参数：
//		x, y：香蕉的位置
//		r：香蕉的旋转位置
//		d：绘制还是擦除(true：绘制；false：擦除)
void DrawBanana(int x, int y, int r, bool d)
{
	static IMAGE oldimg;

	if (d)
	{
		getimage(&oldimg, x, y, g_imgBanana[0].getwidth(), g_imgBanana[0].getheight());
		putimage(x, y, &g_imgBanana[r]);
	}
	else
		putimage(x, y, &oldimg);
}



// 香蕉攻击后的爆炸效果
// 参数：
//		x, y：爆炸的位置
void DoExplosion(int x, int y)
{
	int r = 10;
	int i;

	setcolor(EXPLOSIONCOLOR);
	for (i = 0; i <= r; i++)
	{
		circle(x, y, i);
		Sleep(16);
	}

	setcolor(BACKATTR);
	for (i = r; i >= 0; i--)
	{
		circle(x, y, i);
		Sleep(16);
	}

	setfillcolor(BACKATTR);
	fillcircle(x, y, r);
}



// 游戏者死亡后爆炸
// 参数：
//		x, y：攻击的位置
int ExplodeGorilla(int x, int y)
{
	int iPlayerHit = (x < SCRWIDTH / 2) ? 0 : 1;
	int iPlayerX = g_ptGorilla[iPlayerHit].x + g_imgGorD.getwidth() / 2;
	int iPlayerY = g_ptGorilla[iPlayerHit].y + g_imgGorD.getheight() / 2;
	int i;

	setcolor(EXPLOSIONCOLOR);

	for (i = 1; i <= 10; i++)
	{
		circle(x, y, i);
		Sleep(10);
	}

	for (i = 1; i <= 16; i++)
	{
		circle(iPlayerX, iPlayerY + 11, i);
		Sleep(10);
	}

	for (i = 1; i <= 32; i++)
	{
		setcolor((i % 2 == 0) ? 0x54A8FC : 0x5400FC);
		circle(iPlayerX, iPlayerY, i);
		Sleep(10);
	}

	for (i = 48; i >= 1; i--)
	{
		setcolor(BACKATTR);
		circle(iPlayerX, iPlayerY, i);
		Sleep(10);
	}

	fillcircle(iPlayerX, iPlayerY, 48);

	return iPlayerHit;
}



// 绘制跳舞的大猩猩(胜利后执行)
// 参数：
//		idPlayer：游戏者编号
void VictoryDance(int idPlayer)
{
	for (int i = 0; i < 4; i++)
	{
		putimage(g_ptGorilla[idPlayer].x, g_ptGorilla[idPlayer].y, &g_imgGorL);
		Sleep(200);
		putimage(g_ptGorilla[idPlayer].x, g_ptGorilla[idPlayer].y, &g_imgGorR);
		Sleep(200);
	}
}
