///////////////////////////////////////////////////
// 程序名称：自由运动的点
// 编译环境：Visual C++ 6.0 / 2012，EasyX 2013霜降版
// 作　　者：yangw80 <yw80@qq.com>
// 最后修改：2011-5-3
//
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#define AMOUNT 64

// 全屏模糊处理
// （为了简化范例，该函数略去了屏幕第一行和最后一行的处理）
void Blur(DWORD* pMem)
{
	for (int i = 640; i < 640 * 479; i++)
	{
		pMem[i] = RGB(
			(GetRValue(pMem[i]) + GetRValue(pMem[i - 640]) + GetRValue(pMem[i - 1]) + GetRValue(pMem[i + 1]) + GetRValue(pMem[i + 640])) / 5,
			(GetGValue(pMem[i]) + GetGValue(pMem[i - 640]) + GetGValue(pMem[i - 1]) + GetGValue(pMem[i + 1]) + GetGValue(pMem[i + 640])) / 5,
			(GetBValue(pMem[i]) + GetBValue(pMem[i - 640]) + GetBValue(pMem[i - 1]) + GetBValue(pMem[i + 1]) + GetBValue(pMem[i + 640])) / 5);
	}
}

// 点的结构
struct SPOT
{
	int x, y;
	int targetx, targety;
	int dx, dy;
	COLORREF color;
};

// 精确延时函数(可以精确到 1ms，精度 ±1ms)
// （原理在 www.easyx.cn 有文章详细解释）
void HpSleep(int ms)
{
	static clock_t oldclock = clock();		// 静态变量，记录上一次 tick

	oldclock += ms * CLOCKS_PER_SEC / 1000;	// 更新 tick

	if (clock() > oldclock)					// 如果已经超时，无需延时
		oldclock = clock();
	else
		while (clock() < oldclock)			// 延时
			Sleep(1);						// 释放 CPU 控制权，降低 CPU 占用率
}

// 主函数
int main()
{
	// 初始化
	initgraph(640, 480);			// 创建绘图窗口
	BeginBatchDraw();				// 设置批绘图模式
	srand((unsigned)time(NULL));	// 设置随机种子
	DWORD* pMem = GetImageBuffer();	// 获取显存地址

	// 定义所有点
	SPOT spots[AMOUNT];

	// 初始化每个点
	for (int i = 0; i < AMOUNT; i++)
	{
		spots[i].x = spots[i].targetx = rand() % 600 + 20;
		spots[i].y = spots[i].targety = rand() % 440 + 20;
		spots[i].dx = rand() % 40 - 20;
		spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx) * ((rand() % 2) * 2 - 1);
		spots[i].color = HSLtoRGB((float)(rand() % 360), 1.0, 0.5);
	}

	while (!_kbhit())
	{
		for (int i = 0; i < AMOUNT; i++)
		{
			setcolor(spots[i].color);
			moveto(spots[i].x, spots[i].y);

			spots[i].targetx += spots[i].dx;
			spots[i].targety += spots[i].dy;

			// 判断是否越界，以及越界处理
			if (spots[i].targetx <= 0)
			{
				spots[i].dx = rand() % 20;
				spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx) * ((rand() % 2) * 2 - 1);
			}
			else if (spots[i].targetx >= 639)
			{
				spots[i].dx = -rand() % 20;
				spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx) * ((rand() % 2) * 2 - 1);
			}

			if (spots[i].targety <= 0)
			{
				spots[i].dx = rand() % 40 - 20;
				spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx);
			}
			else if (spots[i].targety >= 479)
			{
				spots[i].dx = rand() % 40 - 20;
				spots[i].dy = -(int)sqrt(400 - spots[i].dx * spots[i].dx);
			}

			// 未越界时，有 10% 的概率改变运行方向
			if (rand() % 10 < 1)
			{
				spots[i].dx = rand() % 40 - 20;
				spots[i].dy = (int)sqrt(400 - spots[i].dx * spots[i].dx) * ((rand() % 2) * 2 - 1);
			}

			// 计算新点坐标，画线
			spots[i].x += (int)((spots[i].targetx - spots[i].x) * 0.1);
			spots[i].y += (int)((spots[i].targety - spots[i].y) * 0.1);
			lineto(spots[i].x, spots[i].y);
		}

		// 全屏模糊处理
		Blur(pMem);
		FlushBatchDraw();

		// 延时
		HpSleep(3);
	}

	// 按任意键退出
	closegraph();
	return 0;
}