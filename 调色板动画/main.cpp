///////////////////////////////////////////////////
// 程序名称：基于 EasyX 的调色板动画程序
// 编译环境：VC6.0 / VC2010，EasyX 2011惊蛰版
// 作　　者：krissi <zh@easyx.cn>
// 最后修改：2012-1-4
//
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define	WIDTH	640
#define	HEIGHT	480
#define	PI		3.1415926535

HPALETTE	g_hPal, g_hOldPal;	// 调色板句柄
LOGPALETTE* g_pLOGPAL;			// 调色板信息结构体
HDC			g_hDC;				// EasyX 窗口的 DC 句柄


// 初始化
void Init()
{
	initgraph(WIDTH, HEIGHT);	// 创建绘图窗口
	g_hDC = GetImageHDC();		// 获取绘图窗口的句柄

	// 设定动画色系
	int r = 216;
	int g = 0;
	int b = 0;

	///////////////////
	// 初始化调色板

	// 为调色板结构体分配内存(该调色板含有 32 种颜色)
	// 比实际多四个字节是增加了 palVersion 和 palNumEntries 两个成员，
	// 详见 MSDN 中 LOGPALETTE 的定义
	g_pLOGPAL = (LOGPALETTE*)new PALETTEENTRY[33];
	g_pLOGPAL->palVersion = 0x300;
	g_pLOGPAL->palNumEntries = 32;

	for (int i = 0; i < 32; i++)
	{
		g_pLOGPAL->palPalEntry[i].peRed = BYTE(r * sin(PI * i / 32));
		g_pLOGPAL->palPalEntry[i].peGreen = BYTE(g * sin(PI * i / 32));
		g_pLOGPAL->palPalEntry[i].peBlue = BYTE(b * sin(PI * i / 32));
		g_pLOGPAL->palPalEntry[i].peFlags = PC_RESERVED;
	}

	g_hPal = CreatePalette(g_pLOGPAL);					// 创建调色板
	g_hOldPal = SelectPalette(g_hDC, g_hPal, false);	// 选入调色板
}


// 改变调色板
void ChangePal()
{
	// 使调色板的颜色循环更换
	PALETTEENTRY t = g_pLOGPAL->palPalEntry[0];
	for (int i = 0; i < 31; i++)
		g_pLOGPAL->palPalEntry[i] = g_pLOGPAL->palPalEntry[i + 1];
	g_pLOGPAL->palPalEntry[31] = t;

	// 使调色板的修改生效
	AnimatePalette(g_hPal, 0, 32, g_pLOGPAL->palPalEntry);
	RealizePalette(g_hDC);
}


// 画图案
void Draw()
{
	HBRUSH hBrush;
	int x1, y1;
	COLORREF c;

	for (int y = 0; y < 64; y++)
		for (int x = 0; x < 64; x++)
		{
			RECT rt = { x * WIDTH / 64 + 1,
						y * HEIGHT / 64 + 1,
						(x + 1) * WIDTH / 64,
						(y + 1) * HEIGHT / 64 };

			x1 = x > 31 ? 63 - x : x;
			y1 = y > 31 ? 63 - y : y;
			c = PALETTEINDEX(min(x1, y1));

			hBrush = CreateSolidBrush(c);
			FillRect(g_hDC, &rt, hBrush);
			DeleteObject(hBrush);
		}
}


// 退出
void Quit()
{
	SelectPalette(g_hDC, g_hOldPal, false);
	DeleteObject(g_hPal);

	delete[] g_pLOGPAL;

	closegraph();
}


// 主函数
void main()
{
	Init();					// 初始化

	while (!_kbhit())		// 按任意键退出
	{
		ChangePal();		// 改变调色板
		Draw();				// 画图案(每次画的都一样，动态图像是因为调色板的改变)
		FlushBatchDraw();	// 使 GDI 绘图函数生效
		Sleep(33);			// 延时
	}

	Quit();					// 退出
}