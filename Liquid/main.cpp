///////////////////////////////////////////////////
// 程序名称：流体(Liquid)
// 编译环境：Visual C++ 6.0 / 2010，EasyX 20120404(beta)
// 原 作 品：http://spielzeugz.de/html5/liquid-particles.html (HTML5)
// 移植作者：krissi <zh@easyx.cn>
// 最后修改：2012-4-5
//
#include <graphics.h>
#include <math.h>
#include <time.h>

typedef union {
	DWORD Data;
	struct {
		DWORD Color : 0X18;
		DWORD : 0;
	};
	struct {
		BYTE B;	//蓝色通道
		BYTE G;	//绿色通道
		BYTE R;	//红色通道
		BYTE A;	//透明通道
	};
}COLOR;//ARGB分量颜色（与RGB兼容）


short  WIDTH;		// 屏幕宽
short HEIGHT;			// 屏幕高
#define NUM_MOVERS	640			// 小球数量
#define	FRICTION	0.96f		// 摩擦力


// 定义小球结构
struct Mover
{
	COLORREF	color;			// 颜色
	float		x, y;			// 坐标
	float		vX, vY;			// 速度
};


// 定义全局变量
Mover	movers[NUM_MOVERS];			// 小球数组
int		mouseX, mouseY;			// 当前鼠标坐标
int		mouseVX, mouseVY;		// 鼠标速度
int		prevMouseX, prevMouseY;		// 上次鼠标坐标
bool	isMouseDown;				// 鼠标左键是否按下
COLOR* pBuffer;					// 显存指针


// 初始化
void init()
{
	// 初始化小球数组
	for (int i = 0; i < NUM_MOVERS; i++)
	{
		movers[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
		movers[i].x = WIDTH * 0.5;
		movers[i].y = HEIGHT * 0.5;
		movers[i].vX = float(cos(float(i))) * (rand() % 34);
		movers[i].vY = float(sin(float(i))) * (rand() % 34);
	}

	// 初始化鼠标变量
	mouseX = prevMouseX = WIDTH / 2;
	mouseY = prevMouseY = HEIGHT / 2;

	// 设置随机种子
	srand((unsigned int)time(NULL));

	// 获取显存指针
	pBuffer = (COLOR*)GetImageBuffer(NULL);
}


// 全屏变暗 50%
void darken()
{
#define ChangeLight 30
	for (int i = WIDTH * HEIGHT - 1; i >= 0; i--)
		//if (pBuffer[i].Data!= 0)
	{
		if (pBuffer[i].R > ChangeLight)
			pBuffer[i].R -= ChangeLight;
		else
			pBuffer[i].R = 0;
		if (pBuffer[i].G > ChangeLight)
			pBuffer[i].G -= ChangeLight;
		else
			pBuffer[i].G = 0;
		if (pBuffer[i].B > ChangeLight)
			pBuffer[i].B -= ChangeLight;
		else
			pBuffer[i].B = 0;
		//	pBuffer[i].G>>=1;
		//	pBuffer[i].B>>=1;
	}
}


// 绝对延时
void delay(DWORD ms)
{
	static DWORD oldtime = GetTickCount();

	while (GetTickCount() - oldtime < ms)
		Sleep(1);

	oldtime = GetTickCount();
}


// 绘制动画(一帧)
void run()
{
	darken();	// 全屏变暗

	mouseVX = mouseX - prevMouseX;
	mouseVY = mouseY - prevMouseY;
	prevMouseX = mouseX;
	prevMouseY = mouseY;

	float toDist = WIDTH * 0.86f;
	float stirDist = WIDTH * 0.125f;
	float blowDist = WIDTH * 0.5f;

	for (int i = 0; i < NUM_MOVERS; i++)
	{
		float x = movers[i].x;
		float y = movers[i].y;
		float vX = movers[i].vX;
		float vY = movers[i].vY;

		float dX = x - mouseX;
		float dY = y - mouseY;
		float d = (float)sqrt(dX * dX + dY * dY);
		dX = d ? dX / d : 0;
		dY = d ? dY / d : 0;

		if (isMouseDown && d < blowDist)
		{
			float blowAcc = (1 - (d / blowDist)) * 14;
			vX += dX * blowAcc + 0.5f - float(rand()) / RAND_MAX;
			vY += dY * blowAcc + 0.5f - float(rand()) / RAND_MAX;
		}

		if (d < toDist)
		{
			float toAcc = (1 - (d / toDist)) * WIDTH * 0.0014f;
			vX -= dX * toAcc;
			vY -= dY * toAcc;
		}

		if (d < stirDist)
		{
			float mAcc = (1 - (d / stirDist)) * WIDTH * 0.00026f;
			vX += mouseVX * mAcc;
			vY += mouseVY * mAcc;
		}

		vX *= FRICTION;
		vY *= FRICTION;

		float avgVX = (float)fabs(vX);
		float avgVY = (float)fabs(vY);
		float avgV = (avgVX + avgVY) * 0.5f;

		if (avgVX < 0.1) vX *= float(rand()) / RAND_MAX * 3;
		if (avgVY < 0.1) vY *= float(rand()) / RAND_MAX * 3;

		float sc = avgV * 0.45f;
		sc = max(min(sc, 3.5f), 0.4f);

		float nextX = x + vX;
		float nextY = y + vY;

		if (nextX > WIDTH) { nextX = WIDTH;	vX *= -1; }
		else if (nextX < 0) { nextX = 0;		vX *= -1; }
		if (nextY > HEIGHT) { nextY = HEIGHT;	vY *= -1; }
		else if (nextY < 0) { nextY = 0;		vY *= -1; }

		movers[i].vX = vX;
		movers[i].vY = vY;
		movers[i].x = nextX;
		movers[i].y = nextY;

		// 画小球
		setcolor(movers[i].color);
		setfillstyle(movers[i].color);
		fillcircle(int(nextX + 0.5), int(nextY + 0.5), int(sc + 0.5));
	}
}


// 主函数
void main()
{
	// 创建绘图窗口
	WIDTH = GetSystemMetrics(SM_CXSCREEN);
	HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	initgraph(WIDTH, HEIGHT);
	HWND hwnd = GetHWnd();
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, WIDTH, HEIGHT, SWP_SHOWWINDOW);

	// 启用批绘图模式
	BeginBatchDraw();

	// 初始化
	init();

	// 鼠标消息变量
	MOUSEMSG m;

	while (true)
	{
		// 处理鼠标消息
		while (MouseHit())
		{
			m = GetMouseMsg();

			switch (m.uMsg)
			{
			case WM_MOUSEMOVE:		mouseX = m.x;	mouseY = m.y;	break;
			case WM_LBUTTONDOWN:	isMouseDown = true;				break;
			case WM_LBUTTONUP:		isMouseDown = false;			break;
			}
		}

		// 绘制动画
		run();

		// 显示缓存的绘制内容
		FlushBatchDraw();

		Sleep(5);

		// 延时 20 毫秒
		///delay(5);
	}

	// 关闭
	EndBatchDraw();
	closegraph();
}