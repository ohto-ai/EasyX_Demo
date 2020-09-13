/*
	程序名称：扫雷

	简　　介：一款经典的小游戏，程序运行中可以通过 ESC 直接退出
			　左键点击，右键标记和取消标记，同时按下左右或按下中间键触发提示
			　其他操作基本和官方一致，界面略有调整，详细操作自行测试

	编译环境：VS2019 + EasyX_20190529(beta)

	编写时间：2019-6-29
	最后修改：2019-7-30

	作　　者：Teternity(qq1926594835)

	版　　权：作者原创，无抄袭，不涉及版权问题，仅用作学习
*/

// 头文件
#include <easyx.h>
#include <conio.h>
#include <cstdio>
#include <ctime>


// 难度
#define primary 1
#define intermediate 2
#define exclusive 3


// 全局变量
long			ret = 0;
const double	PI = 3.1415926;
int				graphSize = primary;
const COLORREF	cellLT = WHITE;
const COLORREF	cellRB = DARKGRAY;
const COLORREF	cellBK = LIGHTGRAY;
const COLORREF	butTop = RGB(200, 200, 200);
const int		gap = 16;
const int		ltRim_w = 3;
const int		Rim_w = 6;
const int		ltrbRim_w = 3;
const int		g_top = 22;
const int		win_h = 43;
const int		face_w = 25;
bool			isInitgraph = true;
COLORREF COLOR[8] = { RGB(0,0,200), RGB(0,150,0), RGB(255,0,0), RGB(0,0,100), RGB(130,0,0), RGB(0,150,150), RGB(80,50,20) };


// Gragh类
class Graph
{
public:
	// 构造函数
	Graph(int g_size = primary)
	{
		InitData(g_size);

		if (isInitgraph)
		{
			closegraph();
			initgraph(g_width, g_height, EW_NOMINIMIZE);
		}
		setbkcolor(cellRB);
		cleardevice();
		isInitgraph = false;

		setlinecolor(butTop);
		line(0, 0, g_width - 1, 0);
		line(0, g_top - 1, g_width - 1, g_top - 1);
		setfillcolor(cellLT);
		solidrectangle(0, 1, g_width - 1, g_top - 2);
		setfillcolor(cellBK);
		solidrectangle(ltrbRim_w + Rim_w, top - ltrbRim_w - Rim_w, right + ltrbRim_w, top - ltrbRim_w);
		solidrectangle(ltRim_w + Rim_w + 2, g_top + ltRim_w + Rim_w + 2, g_width - 1 - Rim_w - 2, g_top + ltRim_w + Rim_w + 2 + 32);
		setlinecolor(cellLT);
		line(g_width - 1 - Rim_w, g_top + ltRim_w + Rim_w + 2 + 34, g_width - 1 - Rim_w, g_top + ltRim_w + Rim_w);
		line(g_width - 1 - Rim_w - 1, g_top + ltRim_w + Rim_w + 2 + 33, g_width - 1 - Rim_w - 1, g_top + ltRim_w + Rim_w + 1);
		line(g_width - 1 - Rim_w, g_top + ltRim_w + Rim_w + 2 + 34, ltRim_w + Rim_w, g_top + ltRim_w + Rim_w + 2 + 34);
		line(g_width - 1 - Rim_w, g_top + ltRim_w + Rim_w + 2 + 33, ltRim_w + Rim_w + 1, g_top + ltRim_w + Rim_w + 2 + 33);

		setfillcolor(BLACK);
		n_mineX = ltRim_w + Rim_w * 2 + 1, n_mineY = g_top + ltRim_w + Rim_w * 2;
		n_timeX = g_width - 2 - Rim_w * 2 - face_w * 2, n_timeY = n_mineY;
		faceX = (g_width - 1) / 2 - face_w / 2 + 2, faceY = n_mineY;
		putimage(n_mineX, n_mineY, img_Nmine);
		putimage(n_timeX, n_timeY, img_Ntime);
		putimage(faceX, faceY, img_faceSM);

		setlinecolor(cellLT);
		for (int i = 0; i < ltRim_w; i++)
		{
			line(0, g_top + i, g_width - 1, g_top + i);
			line(i, g_top, i, g_height - 1);
		}
		setlinecolor(cellBK);
		for (int i = 0; i < Rim_w; i++)
			rectangle(ltRim_w + i, g_top + ltRim_w + i, g_width - 1 - i, g_height - 1 - i);
		for (int i = 0; i < ltrbRim_w; i++)
		{
			setlinecolor(cellRB);
			line(ltRim_w + Rim_w + i, top - ltrbRim_w + i, g_width - 1 - Rim_w - i, top - ltrbRim_w + i);
			line(ltRim_w + Rim_w + i, top - ltrbRim_w + i, ltRim_w + Rim_w + i, g_height - 1 - Rim_w - i);
			setlinecolor(cellLT);
			line(g_width - 1 - Rim_w - i, g_height - 1 - Rim_w - i, g_width - 1 - Rim_w - i, top - ltrbRim_w + i);
			line(g_width - 1 - Rim_w - i, g_height - 1 - Rim_w - i, ltRim_w + Rim_w + i, g_height - 1 - Rim_w - i);
		}
		for (int j = 0; j < n_size_y; j++)
			for (int i = 0; i < n_size_x; i++)
				putimage(left + i * gap, top + j * gap, img_unClicked);

		SetMenu(g_size);
	}

	// 析构函数
	virtual ~Graph()
	{
		delete img_unClicked;
		delete img_Nmine;
		delete img_Ntime;
		delete img_faceSM;
		delete img_faceAM;
		delete img_faceDN;
		delete img_faceCR;
		delete img_blank;
		delete img_mine;
		delete img_mine_dead;
		delete img_mine_wrong;
		delete img_flag;
		delete img_num;
		delete img_p;
		delete img_i;
		delete img_e;
		if (isInitgraph)
			closegraph();
	}

	// 初始化数据
	void InitData(int g_size = primary)
	{
		if (g_size == primary)
		{
			n_size_x = 9;
			n_size_y = 9;
			n_mine = 10;
		}
		else if (g_size == intermediate)
		{
			n_size_x = 16;
			n_size_y = 16;
			n_mine = 40;
		}
		else
		{
			n_size_x = 30;
			n_size_y = 16;
			n_mine = 99;
		}
		g_width = n_size_x * gap + ltRim_w + Rim_w * 2 + ltrbRim_w * 2;
		g_height = n_size_y * gap + ltRim_w + Rim_w * 2 + ltrbRim_w * 2 + g_top + win_h;

		left = ltRim_w + Rim_w + ltrbRim_w;
		top = ltRim_w + Rim_w + ltrbRim_w + g_top + win_h;
		right = g_width - 1 - Rim_w - ltrbRim_w;
		button = g_width - 1 - Rim_w - ltrbRim_w;

		img_unClicked = new IMAGE(gap - 1, gap - 1);
		SetWorkingImage(img_unClicked);
		setbkcolor(cellBK);
		cleardevice();
		setlinecolor(cellLT);
		line(0, 0, gap - 2, 0);
		line(0, 0, 0, gap - 2);
		line(0, 1, gap - 4, 1);
		line(1, 0, 1, gap - 4);
		setlinecolor(cellRB);
		line(gap - 2, gap - 2, gap - 2, 1);
		line(gap - 2, gap - 2, 1, gap - 2);

		img_Nmine = new IMAGE(face_w * 2, face_w);
		img_Ntime = new IMAGE(face_w * 2, face_w);
		img_faceSM = new IMAGE(face_w, face_w);
		img_faceAM = new IMAGE(face_w, face_w);
		img_faceDN = new IMAGE(face_w, face_w);
		img_faceCR = new IMAGE(face_w, face_w);

		TCHAR str[5] = { 0 };
		SetWorkingImage(img_Nmine);
		settextcolor(RGB(255, 0, 0));
		settextstyle(face_w - 1, 0, _T("Consolas"));
		_stprintf_s(str, _T("0%d"), n_mine);
		outtextxy((2 * face_w - textwidth(str)) / 2, (face_w - textheight(str)) / 2, str);

		SetWorkingImage(img_Ntime);
		settextcolor(RGB(255, 0, 0));
		settextstyle(face_w - 1, 0, _T("Consolas"));
		_stprintf_s(str, _T("00%d"), 0);
		outtextxy((2 * face_w - textwidth(str)) / 2, (face_w - textheight(str)) / 2, str);

		SetWorkingImage(img_faceSM);
		setbkcolor(cellBK);
		cleardevice();
		setlinecolor(cellLT);
		for (int i = 0; i < 2; i++)
		{
			setlinecolor(cellLT);
			line(i, i, face_w - 1 - i, i);
			line(i, i, i, face_w - 1 - i);
			setlinecolor(cellRB);
			line(face_w - 1 - i, face_w - 1 - i, face_w - 1 - i, i);
			line(face_w - 1 - i, face_w - 1 - i, i, face_w - 1 - i);
		}
		setfillcolor(YELLOW);
		solidcircle(face_w / 2, face_w / 2, face_w / 2 - 4);
		setlinecolor(BLACK);
		circle(face_w / 2, face_w / 2, face_w / 2 - 4);
		setfillcolor(BLACK);
		solidcircle(face_w / 3 + 1, face_w / 3 + 1, 1);
		solidcircle(face_w / 3 * 2 - 1, face_w / 3 + 1, 1);
		arc(face_w / 3, face_w / 3, face_w / 3 * 2, face_w / 3 * 2, -1 * PI / 9 * 8, -1 * PI / 9);

		SetWorkingImage(img_faceAM);
		setbkcolor(cellBK);
		cleardevice();
		setlinecolor(cellLT);
		for (int i = 0; i < 2; i++)
		{
			setlinecolor(cellLT);
			line(i, i, face_w - 1 - i, i);
			line(i, i, i, face_w - 1 - i);
			setlinecolor(cellRB);
			line(face_w - 1 - i, face_w - 1 - i, face_w - 1 - i, i);
			line(face_w - 1 - i, face_w - 1 - i, i, face_w - 1 - i);
		}
		setfillcolor(YELLOW);
		solidcircle(face_w / 2, face_w / 2, face_w / 2 - 4);
		setlinecolor(BLACK);
		circle(face_w / 2, face_w / 2, face_w / 2 - 4);
		setfillcolor(BLACK);
		solidcircle(face_w / 3 + 1, face_w / 3 + 1, 1);
		solidcircle(face_w / 3 * 2 - 1, face_w / 3 + 1, 1);
		circle(face_w / 2, face_w / 3 * 2, 2);

		SetWorkingImage(img_faceDN);
		setbkcolor(cellBK);
		cleardevice();
		setlinecolor(cellRB);
		line(0, 0, face_w - 1, 0);
		line(0, 0, 0, face_w - 1);
		line(1, 1, face_w - 1 - 1, 1);
		line(1, 1, 1, face_w - 1 - 1);
		line(face_w - 1, face_w - 1, face_w - 1, 0);
		line(face_w - 1, face_w - 1, 0, face_w - 1);
		setfillcolor(YELLOW);
		solidcircle(face_w / 2, face_w / 2, face_w / 2 - 4);
		setlinecolor(BLACK);
		circle(face_w / 2, face_w / 2, face_w / 2 - 4);
		setfillcolor(BLACK);
		solidcircle(face_w / 3 + 1, face_w / 3 + 1, 1);
		solidcircle(face_w / 3 * 2 - 1, face_w / 3 + 1, 1);
		arc(face_w / 3, face_w / 3, face_w / 3 * 2, face_w / 3 * 2, -1 * PI / 9 * 8, -1 * PI / 9);

		SetWorkingImage(img_faceCR);
		setbkcolor(cellBK);
		cleardevice();
		setlinecolor(cellLT);
		for (int i = 0; i < 2; i++)
		{
			setlinecolor(cellLT);
			line(i, i, face_w - 1 - i, i);
			line(i, i, i, face_w - 1 - i);
			setlinecolor(cellRB);
			line(face_w - 1 - i, face_w - 1 - i, face_w - 1 - i, i);
			line(face_w - 1 - i, face_w - 1 - i, i, face_w - 1 - i);
		}
		setfillcolor(YELLOW);
		solidcircle(face_w / 2, face_w / 2, face_w / 2 - 4);
		setlinecolor(BLACK);
		circle(face_w / 2, face_w / 2, face_w / 2 - 4);
		setfillcolor(BLACK);
		line(face_w / 3, face_w / 3 - 1, face_w / 3 + 2, face_w / 3 + 1);
		line(face_w / 3, face_w / 3 + 1, face_w / 3 + 2, face_w / 3 - 1);
		line(face_w / 3 * 2 - 2, face_w / 3 - 1, face_w / 3 * 2, face_w / 3 + 1);
		line(face_w / 3 * 2 - 2, face_w / 3 + 1, face_w / 3 * 2, face_w / 3 - 1);
		arc(face_w / 3, face_w / 3 * 2 - 2, face_w / 3 * 2, face_w - 2, PI / 8, PI / 8 * 7);

		img_blank = new IMAGE(gap - 1, gap - 1);
		img_mine = new IMAGE(gap - 1, gap - 1);
		img_mine_dead = new IMAGE(gap - 1, gap - 1);
		img_mine_wrong = new IMAGE(gap - 1, gap - 1);
		img_flag = new IMAGE(gap - 1, gap - 1);
		img_num = new IMAGE(gap - 1, gap - 1);

		SetWorkingImage(img_blank);
		setbkcolor(cellBK);
		cleardevice();

		SetWorkingImage(img_mine);
		setbkcolor(cellBK);
		cleardevice();
		setfillcolor(BLACK);
		solidcircle((gap - 1) / 2, (gap - 1) / 2, (gap - 1) / 2 - 2);
		setfillcolor(WHITE);
		solidcircle((gap - 1) / 3, (gap - 1) / 3, 1);

		SetWorkingImage(img_mine_dead);
		setbkcolor(RGB(255, 0, 0));
		cleardevice();
		setfillcolor(BLACK);
		solidcircle((gap - 1) / 2, (gap - 1) / 2, (gap - 1) / 2 - 2);
		setfillcolor(WHITE);
		solidcircle((gap - 1) / 3, (gap - 1) / 3, 1);

		*img_mine_wrong = *img_mine;
		SetWorkingImage(img_mine_wrong);
		setlinecolor(RGB(255, 0, 0));
		line(2, 2, gap - 3, gap - 3);
		line(2, gap - 3, gap - 3, 2);

		*img_flag = *img_unClicked;
		POINT pot[3] = { {5,(gap - 1) / 4 + 3} ,{(gap - 1) / 2 + 2,2},{(gap - 1) / 2 + 2,(gap - 1) / 2 + 3} };
		SetWorkingImage(img_flag);
		setfillcolor(RGB(255, 0, 0));
		solidpolygon(pot, 3);
		setlinecolor(BLACK);
		line((gap - 1) / 2 + 1, (gap - 1) / 2 + 3, (gap - 1) / 2 + 1, (gap - 1) / 2 + 5);

		m_width = g_width / 3;
		m_height = g_top - 2;
		img_p = new IMAGE(m_width, m_height);
		img_i = new IMAGE(m_width, m_height);
		img_e = new IMAGE(m_width, m_height);

		SetWorkingImage(img_p);
		settextcolor(BLACK);
		settextstyle(m_height - 2, 0, _T("Microsoft YaHei UI"));
		SetWorkingImage(img_i);
		settextcolor(BLACK);
		settextstyle(m_height - 2, 0, _T("Microsoft YaHei UI"));
		SetWorkingImage(img_e);
		settextcolor(BLACK);
		settextstyle(m_height - 2, 0, _T("Microsoft YaHei UI"));

		SetWorkingImage();
	}

protected:
	// 保护成员
	int n_size_x;
	int n_size_y;
	int n_mine;
	int g_width;
	int g_height;
	int left, top, right, button;

	IMAGE* img_unClicked;

	int n_mineX, n_mineY;
	int n_timeX, n_timeY;
	int faceX, faceY;
	IMAGE* img_Nmine, * img_Ntime;
	IMAGE* img_faceSM, * img_faceAM, * img_faceDN, * img_faceCR;

	IMAGE* img_blank;
	IMAGE* img_mine;
	IMAGE* img_mine_dead;
	IMAGE* img_mine_wrong;
	IMAGE* img_flag;
	IMAGE* img_num;

	int m_width;
	int m_height;
	IMAGE* img_p, * img_i, * img_e;

	void SetMenu(int x)
	{
		SetWorkingImage(img_p);
		if (x == primary)
		{
			setbkcolor(RGB(200, 255, 255));
			setlinecolor(RGB(80, 200, 255));
			cleardevice();
			rectangle(0, 0, m_width - 1, m_height - 1);
		}
		else
		{
			setbkcolor(WHITE);
			cleardevice();
		}
		outtextxy((m_width - 1 - textwidth(_T("初级"))) / 2, (m_height - 1 - textheight(_T("初级"))) / 2, _T("初级"));
		SetWorkingImage(img_i);
		if (x == intermediate)
		{
			setbkcolor(RGB(200, 255, 255));
			setlinecolor(RGB(80, 200, 255));
			cleardevice();
			rectangle(0, 0, m_width - 1, m_height - 1);
		}
		else
		{
			setbkcolor(WHITE);
			cleardevice();
		}
		outtextxy((m_width - 1 - textwidth(_T("中级"))) / 2, (m_height - 1 - textheight(_T("中级"))) / 2, _T("中级"));
		SetWorkingImage(img_e);
		if (x != primary && x != intermediate)
		{
			setbkcolor(RGB(200, 255, 255));
			setlinecolor(RGB(80, 200, 255));
			cleardevice();
			rectangle(0, 0, m_width - 1, m_height - 1);
		}
		else
		{
			setbkcolor(WHITE);
			cleardevice();
		}
		outtextxy((m_width - 1 - textwidth(_T("高级"))) / 2, (m_height - 1 - textheight(_T("高级"))) / 2, _T("高级"));
		SetWorkingImage();
		putimage(0, 1, img_p);
		putimage(m_width, 1, img_i);
		putimage(m_width * 2, 1, img_e);
	}
};


// MineSweeper 类
class MineSweeper :protected Graph
{
public:
	// 构造函数
	MineSweeper(int g_size = primary) :Graph(g_size)
	{
		curSize = g_size;
		b_win = false;
		b_dead = false;
		isStart = false;
		isTick = false;
		residue = n_size_x * n_size_y - n_mine;
		val_blank = 0;
		val_mine = -1;
		val_clicked_blank = -2;
		val_clicked_num = -11;
		val_Dflag = -22;
		n_tick = 0;

		cell = new int* [n_size_x];
		for (int i = 0; i < n_size_x; i++)
		{
			cell[i] = new int[n_size_y];
			memset(cell[i], 0, n_size_y * sizeof(int));
		}

		for (int i = 0; i < n_mine; i++)
		{
			int x, y;
			x = rand() % n_size_x;
			y = rand() % n_size_y;
			if (cell[x][y] != val_mine)
				cell[x][y] = val_mine;
			else
			{
				int dd = (x * y > n_size_x * n_size_y / 2) ? -1 : 1;
				while (cell[x][y] == val_mine)
				{
					if (x + dd > 0 && x + dd < n_size_x - 1)
						x += dd;
					else
						y += dd;
				}
				cell[x][y] = val_mine;
			}
		}

		for (int y = 0; y < n_size_y; y++)
			for (int x = 0; x < n_size_x; x++)
				if (cell[x][y] != val_mine)
				{
					int n = 0;
					if (x - 1 >= 0 && cell[x - 1][y] == val_mine)
						n++;
					if (x + 1 < n_size_x && cell[x + 1][y] == val_mine)
						n++;
					if (y - 1 >= 0)
					{
						for (int i = 0; i < 3; i++)
							if (x - 1 + i >= 0 && x - 1 + i < n_size_x && cell[x - 1 + i][y - 1] == val_mine)
								n++;
					}
					if (y + 1 < n_size_y)
					{
						for (int i = 0; i < 3; i++)
							if (x - 1 + i >= 0 && x - 1 + i < n_size_x && cell[x - 1 + i][y + 1] == val_mine)
								n++;
					}
					if (n == 0)
						cell[x][y] = val_blank;
					else
						cell[x][y] = n;
				}
	}

	// 运行
	int Running()
	{
		MOUSEMSG msg;
		while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000) && !b_win && !b_dead)
		{
			if (MouseHit())
			{
				msg = GetMouseMsg();
				int lx = (msg.x - left) / gap;
				int ly = (msg.y - top) / gap;
				int downX = -1, downY = -1;
				if ((msg.mkLButton || msg.mkMButton) && msg.y >= g_top)
					putimage(faceX, faceY, img_faceAM);
				if (msg.uMsg == WM_LBUTTONDOWN || msg.uMsg == WM_LBUTTONDBLCLK || msg.uMsg == WM_RBUTTONDOWN || msg.uMsg == WM_RBUTTONDBLCLK
					|| msg.uMsg == WM_MBUTTONDOWN || msg.uMsg == WM_MBUTTONDBLCLK)
				{
					if (msg.y < g_top)
					{
						int lSize = curSize;
						if (msg.x < m_width)
							curSize = primary;
						else if (msg.x < m_width * 2)
							curSize = intermediate;
						else
							curSize = exclusive;
						isInitgraph = (lSize == curSize) ? false : true;
						return curSize;
					}
					else if (InFace(msg.x, msg.y) && msg.uMsg == WM_LBUTTONDOWN)
					{
						putimage(faceX, faceY, img_faceDN);
						while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
						{
							if (MouseHit())
							{
								msg = GetMouseMsg();
								if (msg.uMsg == WM_LBUTTONUP)
								{
									putimage(faceX, faceY, img_faceSM);
									if (InFace(msg.x, msg.y) && isStart)
										return curSize;
									else
										break;
								}
								else if (InFace(msg.x, msg.y))
									putimage(faceX, faceY, img_faceDN);
								else
									putimage(faceX, faceY, img_faceSM);
							}
							Sleep(1);
						}
					}
					else if (InMap(msg.x, msg.y) && !msg.mkLButton && !msg.mkMButton && (msg.uMsg == WM_RBUTTONDOWN || msg.uMsg == WM_RBUTTONDBLCLK))
					{
						if (cell[lx][ly] >= -1)
						{
							putimage(left + lx * gap, top + ly * gap, img_flag);
							cell[lx][ly] += val_Dflag;
							n_mine--;
						}
						else if (cell[lx][ly] <= -11)
						{
							putimage(left + lx * gap, top + ly * gap, img_unClicked);
							cell[lx][ly] -= val_Dflag;
							n_mine++;
						}
						isStart = true;
						ShowMine();
						continue;
					}
					else if (InMap(msg.x, msg.y) && (msg.mkMButton || msg.mkLButton && msg.mkRButton) && cell[lx][ly] != val_clicked_blank)
					{
						ShowHint(lx, ly);
						isStart = true;
						isTick = true;
						continue;
					}
					else if (InMap(msg.x, msg.y) && msg.mkLButton && !msg.mkRButton && !msg.mkMButton && cell[lx][ly] >= val_mine)
					{
						downX = lx, downY = ly;
						putimage(left + downX * gap, top + downY * gap, img_blank);
					}
					while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000) && !b_dead && !b_win)
					{
						if (MouseHit())
						{
							msg = GetMouseMsg();
							int x = (msg.x - left) / gap;
							int y = (msg.y - top) / gap;
							if (msg.uMsg == WM_LBUTTONUP || msg.uMsg == WM_RBUTTONUP || msg.uMsg == WM_MBUTTONUP)
							{
								putimage(faceX, faceY, img_faceSM);
								if (downX != -1 && downY != -1)
								{
									putimage(left + downX * gap, top + downY * gap, img_unClicked);
									downX = -1, downY = -1;
								}
								if (InMap(msg.x, msg.y) && msg.uMsg == WM_LBUTTONUP)
								{
									Click(x, y);
									isStart = true;
									isTick = true;
								}
								break;
							}
							else if (msg.uMsg == WM_LBUTTONDOWN || msg.uMsg == WM_RBUTTONDOWN || msg.uMsg == WM_MBUTTONDOWN)
							{
								if (InMap(msg.x, msg.y) && (msg.mkMButton || msg.mkLButton && msg.mkRButton) && cell[x][y] != val_clicked_blank)
								{
									if (downX != -1 && downY != -1)
									{
										putimage(left + downX * gap, top + downY * gap, img_unClicked);
										downX = -1, downY = -1;
									}
									ShowHint(x, y);
									isStart = true;
									isTick = true;
									break;
								}
							}
							else if (msg.uMsg == WM_MOUSEMOVE)
							{
								if (InMap(msg.x, msg.y))
								{
									if ((msg.mkMButton || msg.mkLButton && msg.mkRButton) && cell[x][y] != val_clicked_blank)
									{
										ShowHint(x, y);
										isStart = true;
										isTick = true;
										break;
									}
									else if (msg.mkLButton)
									{
										if (downX != -1 && downY != -1 && !(x == downX && y == downY))
											putimage(left + downX * gap, top + downY * gap, img_unClicked);
										if (cell[x][y] >= val_mine)
										{
											putimage(left + x * gap, top + y * gap, img_blank);
											downX = x, downY = y;
										}
									}
								}
								else if (downX != -1 && downY != -1)
								{
									putimage(left + downX * gap, top + downY * gap, img_unClicked);
									downX = -1, downY = -1;
								}
							}
						}
						FlushTime();
						Sleep(1);
					}
				}
			}
			FlushTime();
			Sleep(1);
		}
		return Finish();
	}

protected:
	// 保护成员
	// 设置 img_num
	void SetNum(int val)
	{
		TCHAR valstr[10];
		_stprintf_s(valstr, _T("%d"), val);
		*img_num = *img_blank;
		SetWorkingImage(img_num);
		settextstyle(gap - 1, 0, _T("Consolas"), 0, 0, FW_BLACK, false, false, false);
		settextcolor(COLOR[val - 1]);
		setbkmode(TRANSPARENT);
		outtextxy((gap - 1 - textwidth(valstr)) / 2, (gap - 1 - textheight(valstr)) / 2, valstr);
		SetWorkingImage();
	}

	// 鼠标在 Face 里
	bool InFace(int x, int y)
	{
		return (x >= faceX && x < faceX + face_w && y >= faceY && y < faceY + face_w) ? true : false;
	}

	// 鼠标在 Map 里
	bool InMap(int x, int y)
	{
		return (x >= left && x < left + n_size_x * gap && y >= top && y < top + n_size_y * gap) ? true : false;
	}

	// 刷新时间
	void FlushTime()
	{
		if (isTick)
		{
			TCHAR tstr[5] = { 0 };
			if (n_tick != 0)
			{
				int t = ((clock() - n_tick) / 1000) > 999 ? 999 : int((clock() - n_tick) / 1000) + 1;
				if (t < 10)
					_stprintf_s(tstr, _T("00%d"), t);
				else if (t < 100)
					_stprintf_s(tstr, _T("0%d"), t);
				else if (t < 1000)
					_stprintf_s(tstr, _T("%d"), t);
				else
					_stprintf_s(tstr, _T("%d"), 999);
			}
			else
			{
				n_tick = clock();
				_stprintf_s(tstr, _T("00%d"), 1);
			}
			SetWorkingImage(img_Ntime);
			outtextxy((2 * face_w - textwidth(tstr)) / 2, (face_w - textheight(tstr)) / 2, tstr);
			SetWorkingImage();
			putimage(n_timeX, n_timeY, img_Ntime);
		}
	}

	// 显示雷数目
	void ShowMine()
	{
		TCHAR tstr[5] = { 0 };
		if (n_mine < -9)
			_stprintf_s(tstr, _T("%d"), n_mine);
		else if (n_mine < 0)
			_stprintf_s(tstr, _T("0%d"), n_mine);
		else if (n_mine < 10)
			_stprintf_s(tstr, _T("00%d"), n_mine);
		else if (n_mine < 100)
			_stprintf_s(tstr, _T("0%d"), n_mine);
		else
			_stprintf_s(tstr, _T("%d"), n_mine);
		SetWorkingImage(img_Nmine);
		outtextxy((2 * face_w - textwidth(tstr)) / 2, (face_w - textheight(tstr)) / 2, tstr);
		SetWorkingImage();
		putimage(n_mineX, n_mineY, img_Nmine);
	}

	// 显示边缘数字
	void ShowSide()
	{
		for (int y = 0; y < n_size_y; y++)
			for (int x = 0; x < n_size_x; x++)
				if (cell[x][y] == val_clicked_blank)
				{
					POINT arr[8] = { {x - 1,y},{x - 1,y - 1},{x,y - 1},{x + 1,y - 1},{x + 1,y},{x + 1,y + 1},{x,y + 1},{x - 1,y + 1} };
					for (int i = 0; i < 8; i++)
						if (arr[i].x >= 0 && arr[i].x < n_size_x && arr[i].y >= 0 && arr[i].y < n_size_y && cell[arr[i].x][arr[i].y] > val_blank)
						{
							SetNum(cell[arr[i].x][arr[i].y]);
							putimage(left + (arr[i].x) * gap, top + arr[i].y * gap, img_num);
							cell[arr[i].x][arr[i].y] += val_clicked_num;
							residue--;
						}
				}
	}

	// 显示空格
	void ShowBlank(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < n_size_x && y < n_size_y && cell[x][y] == val_blank)
		{
			putimage(left + x * gap, top + y * gap, img_blank);
			cell[x][y] = val_clicked_blank;
			residue--;
		}
		else
			return;
		ShowBlank(x - 1, y);
		ShowBlank(x, y - 1);
		ShowBlank(x + 1, y);
		ShowBlank(x, y + 1);
		ShowBlank(x - 1, y - 1);
		ShowBlank(x + 1, y - 1);
		ShowBlank(x + 1, y + 1);
		ShowBlank(x - 1, y + 1);
	}

	// 点击动作
	void Click(int x, int y)
	{
		if (cell[x][y] == val_mine)
		{
			b_dead = true;
			putimage(left + x * gap, top + y * gap, img_mine_dead);
			cell[x][y] = val_clicked_blank;
			return;
		}
		else if (cell[x][y] == val_blank)
		{
			ShowBlank(x, y);
			ShowSide();
		}
		else if (cell[x][y] > val_blank)
		{
			SetNum(cell[x][y]);
			putimage(left + x * gap, top + y * gap, img_num);
			cell[x][y] += val_clicked_num;
			residue--;
		}
		if (residue <= 0)
			b_win = true;
	}

	// 提示
	void ShowHint(int x, int y)
	{
		bool down = true;
		if (cell[x][y] != val_clicked_blank)
		{
			POINT arr[8] = { {x - 1,y},{x - 1,y - 1},{x,y - 1},{x + 1,y - 1},{x + 1,y},{x + 1,y + 1},{x,y + 1},{x - 1,y + 1} };
			if (cell[x][y] >= val_mine)
				putimage(left + x * gap, top + y * gap, img_blank);
			for (int i = 0; i < 8; i++)
			{
				if (arr[i].x >= 0 && arr[i].x < n_size_x && arr[i].y >= 0 && arr[i].y < n_size_y && cell[arr[i].x][arr[i].y] >= val_mine)
					putimage(left + arr[i].x * gap, top + arr[i].y * gap, img_blank);
			}

			MOUSEMSG msg;
			while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
			{
				if (MouseHit())
				{
					msg = GetMouseMsg();
					if (msg.uMsg == WM_LBUTTONUP || msg.uMsg == WM_RBUTTONUP || msg.uMsg == WM_MBUTTONUP)
					{
						putimage(faceX, faceY, img_faceSM);
						if (InMap(msg.x, msg.y) && down)
						{
							if (cell[x][y] >= val_mine)
								putimage(left + x * gap, top + y * gap, img_unClicked);
							for (int i = 0; i < 8; i++)
								if (arr[i].x >= 0 && arr[i].x < n_size_x && arr[i].y >= 0 && arr[i].y < n_size_y && cell[arr[i].x][arr[i].y] >= val_mine)
									putimage(left + arr[i].x * gap, top + arr[i].y * gap, img_unClicked);
							if (cell[x][y] < val_clicked_blank && cell[x][y] - val_clicked_num > val_blank)
							{
								int todead = false;
								int nMine = 0, nFlag = 0;
								for (int i = 0; i < 8; i++)
									if (arr[i].x >= 0 && arr[i].x < n_size_x && arr[i].y >= 0 && arr[i].y < n_size_y)
									{
										if (cell[arr[i].x][arr[i].y] == val_mine || cell[arr[i].x][arr[i].y] - val_Dflag == val_mine)
											nMine++;
										if (cell[arr[i].x][arr[i].y] < val_clicked_blank + val_clicked_num && cell[arr[i].x][arr[i].y] - val_Dflag >= val_mine)
										{
											nFlag++;
											if (cell[arr[i].x][arr[i].y] - val_Dflag != val_mine)
												todead = true;
										}
									}
								if (nMine == nFlag && !todead)
								{
									for (int i = 0; i < 8; i++)
										if (arr[i].x >= 0 && arr[i].x < n_size_x && arr[i].y >= 0 && arr[i].y < n_size_y)
											Click(arr[i].x, arr[i].y);
								}
								if (nMine == nFlag && todead)
								{
									b_dead = true;
									for (int i = 0; i < 8; i++)
										if (arr[i].x >= 0 && arr[i].x < n_size_x && arr[i].y >= 0 && arr[i].y < n_size_y)
										{
											if (cell[arr[i].x][arr[i].y] == val_mine)
											{
												if (!(cell[arr[i].x][arr[i].y] < val_clicked_blank + val_clicked_num
													&& cell[arr[i].x][arr[i].y] - val_Dflag >= val_mine))
												{
													putimage(left + arr[i].x * gap, top + arr[i].y * gap, img_mine_dead);
													cell[arr[i].x][arr[i].y] = val_clicked_num;
												}
											}
											else
											{
												if (cell[arr[i].x][arr[i].y] < val_clicked_blank + val_clicked_num
													&& cell[arr[i].x][arr[i].y] - val_Dflag >= val_mine
													&& cell[arr[i].x][arr[i].y] - val_Dflag != val_mine)
													putimage(left + arr[i].x * gap, top + arr[i].y * gap, img_mine_wrong);
												else
													Click(arr[i].x, arr[i].y);
											}
										}
									for (int j = 0; j < n_size_y; j++)
										for (int i = 0; i < n_size_x; i++)
											if (cell[i][j] < val_clicked_blank + val_clicked_num
												&& cell[i][j] - val_Dflag >= val_mine
												&& cell[i][j] - val_Dflag != val_mine)
												putimage(left + i * gap, top + j * gap, img_mine_wrong);
								}
							}
						}
						return;
					}
					else if (msg.uMsg == WM_MOUSEMOVE)
					{
						if (down)
						{
							if (cell[x][y] >= val_mine)
								putimage(left + x * gap, top + y * gap, img_unClicked);
							for (int i = 0; i < 8; i++)
								if (arr[i].x >= 0 && arr[i].x < n_size_x && arr[i].y >= 0 && arr[i].y < n_size_y && cell[arr[i].x][arr[i].y] >= val_mine)
									putimage(left + arr[i].x * gap, top + arr[i].y * gap, img_unClicked);
						}

						x = (msg.x - left) / gap;
						y = (msg.y - top) / gap;
						arr[0] = { x - 1, y };
						arr[1] = { x - 1,y - 1 };
						arr[2] = { x,y - 1 };
						arr[3] = { x + 1,y - 1 };
						arr[4] = { x + 1,y };
						arr[5] = { x + 1,y + 1 };
						arr[6] = { x,y + 1 };
						arr[7] = { x - 1,y + 1 };

						if (InMap(msg.x, msg.y))
						{
							if (cell[x][y] != val_clicked_blank)
							{
								if (cell[x][y] >= val_mine)
									putimage(left + x * gap, top + y * gap, img_blank);
								for (int i = 0; i < 8; i++)
									if (arr[i].x >= 0 && arr[i].x < n_size_x && arr[i].y >= 0 && arr[i].y < n_size_y && cell[arr[i].x][arr[i].y] >= val_mine)
										putimage(left + arr[i].x * gap, top + arr[i].y * gap, img_blank);
							}
							down = true;
						}
						else
							down = false;
					}
				}
				FlushTime();
				Sleep(1);
			}
		}
	}

	// 结束
	int Finish()
	{
		if (b_dead)
		{
			for (int y = 0; y < n_size_y; y++)
				for (int x = 0; x < n_size_x; x++)
				{
					if (cell[x][y] == val_mine)
						putimage(left + x * gap, top + y * gap, img_mine);
					else if (cell[x][y] < val_clicked_blank + val_clicked_num && cell[x][y] - val_Dflag >= val_mine && cell[x][y] - val_Dflag != val_mine)
						putimage(left + x * gap, top + y * gap, img_mine_wrong);
				}
			putimage(faceX, faceY, img_faceCR);
		}
		else if (b_win)
		{
			for (int y = 0; y < n_size_y; y++)
				for (int x = 0; x < n_size_x; x++)
					if (cell[x][y] == val_mine)
						putimage(left + x * gap, top + y * gap, img_flag);
			putimage(faceX, faceY, img_faceSM);
			n_mine = 0;
			ShowMine();
		}

		MOUSEMSG msg;
		while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
		{
			if (MouseHit())
			{
				msg = GetMouseMsg();
				if ((msg.uMsg == WM_LBUTTONDOWN || msg.uMsg == WM_RBUTTONDOWN || msg.uMsg == WM_MBUTTONDOWN) && msg.y < g_top)
				{
					int lSize = curSize;
					if (msg.x < m_width)
						curSize = primary;
					else if (msg.x < m_width * 2)
						curSize = intermediate;
					else
						curSize = exclusive;
					isInitgraph = (lSize == curSize) ? false : true;
					return curSize;
				}
				else if (msg.uMsg == WM_LBUTTONDOWN && InFace(msg.x, msg.y))
				{
					putimage(faceX, faceY, img_faceDN);
					while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
					{
						if (MouseHit())
						{
							msg = GetMouseMsg();
							if (msg.uMsg == WM_LBUTTONUP && InFace(msg.x, msg.y))
								return curSize;
							else if (msg.uMsg == WM_LBUTTONUP && !InFace(msg.x, msg.y))
								break;
							else if (InFace(msg.x, msg.y))
								putimage(faceX, faceY, img_faceDN);
							else if (b_dead)
								putimage(faceX, faceY, img_faceCR);
							else
								putimage(faceX, faceY, img_faceSM);
						}
						Sleep(1);
					}
				}
			}
			Sleep(1);
		}
		return 0;
	}

private:
	// 私有成员
	int curSize;
	bool b_win;
	bool b_dead;
	bool isStart;
	bool isTick;
	int residue;
	int val_blank;
	int val_mine;
	int val_clicked_blank;
	int val_clicked_num;
	int val_Dflag;
	long n_tick;
	int** cell;
};


// 主函数
int main()
{
	srand((unsigned)time(nullptr));
	MineSweeper* ms = new MineSweeper(primary);

	while (true)
	{
		int size = ms->Running();
		if (size > 0)
		{
			delete ms;
			ms = new MineSweeper(size);
		}
		else
			break;
	}

	delete ms;
	ret = _getwch();
	return 0;
}