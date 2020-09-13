/////////////////////////////////////////////////////////
// 程序名称：带一盏油灯去巡视仓库
// 编译环境：Visual C++ 6.0 / 2010，EasyX 2013霜降版
// 作    者：yangw80 <http://hi.baidu.com/yangw80>
// 最后修改：2013-11-11
//
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define RANGE 80			// 灯光照射半径
#define WIDTH 320			// 场景宽度
#define HEIGHT 240			// 场景高度
#define ZOOM 2				// 显示时的缩放倍数
#define PI 3.1415926536		// 圆周率

DWORD* g_bufMask;			// 指向“建筑物”IMAGE 的指针
DWORD* g_bufRender;			// 指向渲染 IMAGE 的指针

// 创建“仓库”
void MakeMask()
{
	// 创建“仓库” IMAGE 对象
	static IMAGE g_imgMask(WIDTH, HEIGHT);
	g_bufMask = GetImageBuffer(&g_imgMask);

	// 设置绘图目标
	SetWorkingImage(&g_imgMask);

	// 绘制“仓库”
	settextstyle(160, 0, _T("楷体"));
	outtextxy(6, 20, _T("仓库"));

	// 恢复绘图目标为默认窗口
	SetWorkingImage(NULL);
}


// 在指定位置“照明”
void Lighting(int _x, int _y)
{
	int i;	// 定义循环变量

	// 清空 Render 对象
	memset(g_bufRender, 0, WIDTH * HEIGHT * 4);

	// 计算灯光照亮的区域
	for (double a = 0; a < 2 * PI; a += PI / 360)	// 圆周循环
	{
		for (int r = 0; r < RANGE; r++)				// 半径循环
		{
			// 计算照射到的位置
			int x = (int)(_x + cos(a) * r);
			int y = (int)(_y + sin(a) * r);

			// 光线超出屏幕范围，终止
			// （为了简化全凭模糊运算，不处理最上和最下一行）
			if (x < 0 || x >= WIDTH || y <= 0 || y >= HEIGHT - 1)
				break;

			// 光线碰到建筑物，终止
			if (g_bufMask[y * WIDTH + x])
				break;

			// 光线叠加
			g_bufRender[y * WIDTH + x] += 0x101000;	// 0x101000 是很淡的黄色
		}
	}

	// 修正曝光过度的点
	for (i = WIDTH * HEIGHT - 1; i >= 0; i--)
		if (g_bufRender[i] > 0xffff00)
			g_bufRender[i] = 0xffff00;

	// 将光线模糊处理（避开建筑物）
	for (i = WIDTH; i < WIDTH * (HEIGHT - 1); i++)
		if (!g_bufMask[i])
			for (int j = 0; j < 2; j++)
			{
				g_bufRender[i] = RGB(
					(GetRValue(g_bufRender[i - WIDTH]) + GetRValue(g_bufRender[i - 1]) + GetRValue(g_bufRender[i])
						+ GetRValue(g_bufRender[i + 1]) + GetRValue(g_bufRender[i + WIDTH])) / 5,
					(GetGValue(g_bufRender[i - WIDTH]) + GetGValue(g_bufRender[i - 1]) + GetGValue(g_bufRender[i])
						+ GetGValue(g_bufRender[i + 1]) + GetGValue(g_bufRender[i + WIDTH])) / 5,
					(GetBValue(g_bufRender[i - WIDTH]) + GetBValue(g_bufRender[i - 1]) + GetBValue(g_bufRender[i])
						+ GetBValue(g_bufRender[i + 1]) + GetBValue(g_bufRender[i + WIDTH])) / 5);
			}
}


// 主函数
void main()
{
	// 初始化绘图窗口
	initgraph(WIDTH * ZOOM, HEIGHT * ZOOM);
	BeginBatchDraw();
	DWORD* bufScreen = GetImageBuffer(NULL);

	// 制作建筑
	MakeMask();



	// 创建渲染对象
	IMAGE imgRender(WIDTH, HEIGHT);
	g_bufRender = GetImageBuffer(&imgRender);


	// 定义鼠标消息变量
	MOUSEMSG msg;

	while (true)
	{
		// 获取一条鼠标移动的消息
		do
		{
			msg = GetMouseMsg();
		} while ((msg.uMsg != WM_MOUSEMOVE) || MouseHit());

		// 在鼠标位置模拟灯光
		Lighting(msg.x / ZOOM, msg.y / ZOOM);

		// 将渲染的内容拉伸后显示在绘图窗口中
		int ps = 0, pr = 0;
		for (int y = 0; y < HEIGHT; y++)
			for (int x = 0; x < WIDTH; x++, pr++)
			{
				ps = y * ZOOM * WIDTH * ZOOM + x * ZOOM;
				for (int zy = 0; zy < ZOOM; zy++)
				{
					for (int zx = 0; zx < ZOOM; zx++)
						bufScreen[ps++] = g_bufRender[pr];
					ps += ZOOM * (WIDTH - 1);
				}
			}

		// 刷新显示，并延时
		FlushBatchDraw();
		FlushMouseMsgBuffer();
		Sleep(20);

		// 按任意键退出
		if (_kbhit())
		{
			EndBatchDraw();
			closegraph();
		}
	}
}