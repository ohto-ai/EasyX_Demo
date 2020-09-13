///////////////////////////////////////////////////
// 程序名称：火焰模拟程序
// 编译环境：VC6.0 / VC2010，EasyX 2011惊蛰版
// 作　　者：yangw80 <yw80@qq.com>
// 最后修改：2012-1-21
// PS: 核心算法参考的微软范例，在此感谢。
//
#include <graphics.h>
#include <conio.h>
#include <time.h>

// 宏常量
#define	WIDTH		640
#define HEIGHT		480
#define FIREWIDTH	320
#define	FIREHEIGHT	180

// 全局变量
COLORREF	g_Colors[193];						// 火焰色系中使用的全部颜色
BYTE		g_Fire[FIREWIDTH];					// 火焰数据
BYTE		g_Bits[FIREHEIGHT * FIREWIDTH];		// 火焰数据

// 火焰属性
int m_nDecay = 5;			// 衰减度，范围 [1, 100]，默认 5
int m_nFlammability = 385;	// 易燃性，范围 [1, 399]，默认 385
int m_nMaxHeat = 192;		// 最高热度，范围 [0, 192]，默认 192
int m_nSpreadRate = 20;		// 传播速率，范围 [1, 100]，默认 20
int m_nSize = 160;			// 火源宽度，范围 [40, FIREWIDTH]，默认 160
int m_nSmoothness = 1;		// 平滑度，范围 [0, 5]，默认 1
int m_nDistribution = 1;	// 分布，范围 [0, 10]，默认 1
int m_nChaos = 50;			// 混沌，范围 [1, 100]，默认 50



// 初始化火焰
void InitFire()
{
	// 初始化颜色
	int r, g, b;

	// 默认红色火焰。根据注释选择不同的火焰效果
	b = 256 + 256 + 255;	g = 256 + 255;	r = 255;	// 红色火焰
	//r = 256+256+255;	g = 256+255;	b = 255;	// 蓝色火焰
	//g = 256+256+255;	b = 256+255;	r = 255;	// 绿色火焰

	// 生成火焰色系
	for (int i = 192; i >= 0; i--)
	{
		g_Colors[i] = RGB((r > 255) ? 255 : r, (g > 255) ? 255 : g, (b > 255) ? 255 : b);
		r = (r > 3) ? (r - 4) : 0;
		g = (g > 3) ? (g - 4) : 0;
		b = (b > 3) ? (b - 4) : 0;
	}

	// 置空火焰数组
	memset(g_Fire, 0, FIREWIDTH);
	memset(g_Bits, 0, FIREWIDTH * FIREHEIGHT);
}



// 画色系
void DrawColorScheme()
{
	POINT s[8] = { {0,	450}, {580, 450}, {580, 420}, {610, 420},
					{610, 0}, {639, 0},	  {639, 479}, {0, 479} };

	HRGN rgn1 = CreatePolygonRgn(s, 8, WINDING);
	HRGN rgn2 = CreateEllipticRgn(550, 390, 611, 451);
	CombineRgn(rgn1, rgn1, rgn2, RGN_DIFF);

	// 定义裁剪区域
	setcliprgn(rgn1);			// 设置区域 rgn 为裁剪区
	DeleteObject(rgn1);
	DeleteObject(rgn2);

	// 画色系
	int c, x, y;
	for (int i = 0; i < 1120; i++)
	{
		c = int(i / 5.8);
		x = (i <= 479 ? 639 : 639 - i + 479);
		y = (i <= 479 ? i : 479);
		setcolor(BGR(g_Colors[c]));
		line(0, 0, x, y);
	}

	// 取消裁剪区域
	setcliprgn(NULL);
}



// 计算火焰的每个点
inline void BurnPoint(BYTE* pRow, BYTE* pNextRow)
{
	BYTE* pTarget;

	int off = rand() % (m_nDistribution + 1);

	int val = m_nDecay + 1;
	val = rand() % val;
	val = *pNextRow - val;

	pTarget = (rand() & 1) ? pRow + off : pRow - off;
	*pTarget = (val > 0) ? (BYTE)val : 0;
}



// 计算火焰
void RenderFlame()
{
	int xStart, xEnd, x, y;
	BYTE* pRow;
	BYTE* pNextRow;

	xStart = (FIREWIDTH - m_nSize) / 2;
	xEnd = xStart + m_nSize + 1;

	pRow = g_Bits;
	for (x = 0; x < FIREWIDTH; x++)
	{
		if (x < (xStart + m_nDistribution) || x >= (xEnd - m_nDistribution))
			g_Fire[x] = 0;
		*pRow++ = g_Fire[x];
	}

	for (y = FIREHEIGHT - 1; y > 0; y--)	// y = 0 火焰最大；y++ 火焰变小
	{
		pRow = (g_Bits + FIREWIDTH * y);
		pNextRow = (g_Bits + FIREWIDTH * (y - 1));

		if (rand() & 1)
		{
			for (x = 0; x < FIREWIDTH; x++)
			{
				BurnPoint(pRow, pNextRow);
				pRow++;
				pNextRow++;
			}
		}
		else
		{
			pRow += FIREWIDTH - 1;
			pNextRow += FIREWIDTH - 1;
			for (x = 0; x < FIREWIDTH; x++)
			{
				BurnPoint(pRow, pNextRow);
				pRow--;
				pNextRow--;
			}
		}
	}

	if (rand() % (400 - m_nFlammability) == 0)
	{
		int off = m_nSize - 5;
		off = rand() % off;
		off += xStart;

		for (x = off; x < off + 5; x++)
			g_Fire[x] = 239;
	}

	for (x = xStart; x < xEnd; x++)
	{
		if (g_Fire[x] < m_nMaxHeat)
		{
			int val = rand() % m_nChaos + 1;
			val -= m_nChaos / 2;
			val += m_nSpreadRate;
			val += g_Fire[x];

			if (val > m_nMaxHeat)
				g_Fire[x] = m_nMaxHeat;
			else if (val < 0)
				g_Fire[x] = 0;
			else
				g_Fire[x] = val;
		}
		else
			g_Fire[x] = m_nMaxHeat;
	}

	if (m_nSmoothness > 0)
	{
		xStart += m_nSmoothness;
		xEnd -= m_nSmoothness;
		for (x = xStart; x < xEnd; x++)
		{
			int val = 0;
			for (y = x - m_nSmoothness; y < x + 1 + m_nSmoothness; y++)
				val += g_Fire[y];

			g_Fire[x] = val / (2 * m_nSmoothness + 1);
		}
	}
}



// 显示火焰
void PaintFlame(int offset_x, int offset_y)
{
	DWORD* pDst = GetImageBuffer();	// 获取显存指针

	for (int y = 0; y < FIREHEIGHT; y++)
		for (int x = 0; x < FIREWIDTH; x++)
		{
			COLORREF c = g_Colors[g_Bits[y * FIREWIDTH + x]];
			pDst[(offset_y - y) * WIDTH + offset_x + x] = c;
		}

	FlushBatchDraw();				// 使显存操作生效
}



// 主函数
int main()
{
	initgraph(WIDTH, HEIGHT);		// 创建绘图窗口
	srand((unsigned)time(NULL));	// 设置随机种子

	InitFire();						// 初始化火焰
	DrawColorScheme();				// 绘制色系图

	while (!_kbhit())					// 按任意键退出
	{
		RenderFlame();				// 渲染火焰
		PaintFlame(145, 320);		// 显示火焰
		Sleep(33);					// 延时
	}

	closegraph();
	return 0;
}