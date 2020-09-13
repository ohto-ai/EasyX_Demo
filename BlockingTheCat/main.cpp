/////////////////////////////////////////////////////////
// 程序名称：围住神经猫
// 编译环境  Visual Studio2019(C++语言标准选择C++17),EasyX
// 作者：雷步云<508222866@qq.com> <QQ:508222866>
// 最后修改：2020-5-26

#include <graphics.h>
#include <queue>
#include <cstring>
#include <ctime>
#include <vector>

#pragma comment( lib, "MSIMG32.LIB")
#define pix		 50			// 像素比例
#define hight	(14 * pix)
#define width	(10 * pix)
using namespace std;

int stepS;					// 记录已经走的步数
int startBarrier;			// 开始的障碍物数目
wchar_t Score_[30];
MOUSEMSG m;					// 鼠标操作
IMAGE pig, bkimg;;			// 加载图片


enum class picture
{
	none, barrier
};

struct XY
{
	int x, y;
	int lastX, lastY;	//记录上一个点的坐标
}cat;

struct node
{
	int x, y;			//坐标,圆心位置
	picture pic;		//当前位置的图片内容
};

node canvas[10][10];	//  地图
bool visit[10][10];		//	记录是否访问过地图中的元素
int path[10][10][2];	//  记录上一个位置


//注意：因为地图是交错排列的，奇数列与偶数列猫的移动不同
int dirOdd[6][2]{ 1,0,-1,0,0,1,0,-1,-1,-1,-1,1 };	//控制方向奇数列
int dirEven[6][2]{ 1,0,-1,0,0,1,0,-1,1,-1,1,1 };	//控制方向偶数列

int main();

//贴图函数
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, int direction)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = 50;
	int h = 100;

	// 使用 Windows GDI 函数实现透明位图
	if (direction == 0)
		TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, 0);
	else
		TransparentBlt(dstDC, x, y, w, h, srcDC, 10, 187, w, h, 0);
}

//游戏初始化
void initial()
{
	srand(time(0));
	stepS = 0;
	startBarrier = rand() % 6 + 8;	//障碍物数量
	loadimage(&pig, L"pig.png");
	loadimage(&bkimg, L"bkground.jpg", width, hight, true);
	initgraph(width, hight);
	HWND wnd = GetHWnd();
	SetWindowText(wnd, L"围住神经猫");//设置文章标题
	for (int i = 1; i <= 9; ++i)
		for (int j = 1; j <= 9; ++j)
		{
			if (i & 1)	//如果是奇数行
				canvas[i][j] = node{ j * pix - pix / 4, pix * 4 + i * pix ,picture::none };
			else
				canvas[i][j] = node{ j * pix + pix / 4, pix * 4 + i * pix ,picture::none };
		}

	cat.x = 5; cat.y = 5;	//猫最开始的地方
	while (startBarrier--)
	{
		int bx, by;			//设置初始障碍
		do
		{
			bx = rand() % 10;
			by = rand() % 10;
		} while (canvas[by][bx].pic == picture::barrier || (by == cat.y && bx == cat.x));
		canvas[by][bx].pic = picture::barrier;
	}
	setbkmode(TRANSPARENT);
	BeginBatchDraw();
}


//绘制游戏画面, 白色：空  黄色：障碍物 
void show()
{
	putimage(0, 0, &bkimg);
	setbkcolor(WHITE);
	settextstyle(20, 0, L"微软雅黑");
	outtextxy(200, 170, L"重玩");
	outtextxy(250, 180, L"步数: ");
	swprintf(Score_, 29, L"%d", stepS);
	outtextxy(290, 180, Score_);
	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			if (canvas[i][j].pic == picture::barrier)
				setfillcolor(YELLOW);
			else
				setfillcolor(LIGHTGRAY);
			solidcircle(canvas[i][j].x, canvas[i][j].y, (pix - 4) / 2);
		}
	}
	if (cat.y & 1)	//奇数列
		transparentimage(NULL, cat.x * pix - pix / 4 - 25, pix * 3 + cat.y * pix - 21, &pig, 0);
	else			//偶数列
		transparentimage(NULL, cat.x * pix - 25 + pix / 4, pix * 3 + cat.y * pix - 21, &pig, 1);
	FlushBatchDraw();
}


//寻找下一个点的位置
struct LastOrder
{
	int x, y;
};

vector<LastOrder> vec;

void findNextXY(int x, int y)
{
	if (x == cat.x && y == cat.y)
	{
		vec.push_back({ x,y });
		return;
	}
	else
	{
		findNextXY(path[y][x][0], path[y][x][1]);
		vec.push_back({ x,y });
	}
}


//利用广度优先搜索求最短路径，xy为数组的i,j下标，注意传参
void bfs(XY xy)
{
	//每次搜索时初始化数组
	memset(visit, false, sizeof(visit));
	memset(path, 0, sizeof(path));
	bool tag = true;
	queue<XY> que;
	que.push(xy);
	visit[xy.y][xy.x] = true;
	while (!que.empty())
	{
		XY temp = que.front();
		que.pop();
		//如果找到出口
		if (temp.x == 1 || temp.x == 9 || temp.y == 1 || temp.y == 9)
		{
			findNextXY(temp.x, temp.y);
			cat.x = vec[1].x;
			cat.y = vec[1].y;
			vec.clear();
			tag = false;
			break;
		}
		int dx, dy;
		//寻找可走的路
		for (int i = 0; i < 6; ++i)
		{
			if (temp.y & 1)
			{
				dx = temp.x + dirOdd[i][0];
				dy = temp.y + dirOdd[i][1];
			}
			else
			{
				dx = temp.x + dirEven[i][0];
				dy = temp.y + dirEven[i][1];
			}
			if (dx >= 1 && dx <= 9 && dy >= 1 && dy <= 9 && !visit[dy][dx] && canvas[dy][dx].pic == picture::none)
			{
				visit[dy][dx] = true;
				path[dy][dx][0] = temp.x;
				path[dy][dx][1] = temp.y;
				que.push({ dx,dy,temp.x,temp.y });
			}
		}
	}
	if (tag)					//如果没找到出口
	{
		show();
		HWND wnd = GetHWnd();
		swprintf(Score_, 29, L"你共用了%d步，重玩一局吗", stepS);
		FlushBatchDraw();
		if (MessageBox(wnd, Score_, L"成功", MB_YESNO | MB_ICONQUESTION) == IDYES)
			main();
		else
			exit(-1);
	}
}


//鼠标操作
void dataChangeWithMouseHit()
{
	while (true)
	{
		m = GetMouseMsg();
		if (m.x >= 200 && m.x <= 230 && m.y >= 170 && m.y <= 200)
			settextcolor(BLACK);
		else
			settextcolor(WHITE);
		outtextxy(200, 170, L"重玩");
		FlushBatchDraw();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 200 && m.x <= 230 && m.y >= 170 && m.y <= 200)
				main();
			for (int i = 1; i <= 9; ++i)
				for (int j = 1; j <= 9; ++j)
					//如果在当前方格内,则改变信息
					if (canvas[i][j].pic != picture::barrier && (m.x - canvas[i][j].x) * (m.x - canvas[i][j].x) +
						(m.y - canvas[i][j].y) * (m.y - canvas[i][j].y) <= (pix - 4) * (pix - 4) / 4)
					{
						canvas[i][j].pic = picture::barrier;
						stepS++;
						bfs({ cat.x,cat.y,0,0 });
						return;
					}
		}
	}
}


//不需要鼠标的操作，判断猫是否跑掉
void dataChangeWithoutMouseHit()
{
	if (cat.x == 1 || cat.y == 1 || cat.x == 9 || cat.y == 9)
	{
		show();
		HWND wnd = GetHWnd();
		if (MessageBox(wnd, L"游戏结束。\n神经猫跑掉了！,重玩一局吗", L"询问", MB_YESNO | MB_ICONQUESTION) == IDYES)
			main();
		else
			exit(-1);
	}
}


int main()
{
	initial();
	while (true)
	{
		show();
		dataChangeWithMouseHit();
		dataChangeWithoutMouseHit();
		Sleep(20);
	}
	return 0;
}