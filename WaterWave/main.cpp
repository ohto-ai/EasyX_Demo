/****************************************************
 * 程序名称：实现水波纹显示							*
 * 编译环境：Visual C++ 6.0，EasyX 20130114(beta)	*
 * 作　　者：豪 (QQ: 415051674) 					*
 * 核心算法：参考网上算法							*
 * 最后修改：2013/3/20								*
 ****************************************************/


#include <easyx.h>    
#include <conio.h>
#include <stdio.h>

#define PIC_HEIGHT 615
#define PIC_WIDTH  800

void FrameFun();									// 帧逻辑函数，处理每一帧的逻辑
void RenderFun();									// 帧渲染函数，输出每一帧到显示设备

IMAGE src_img;										// 原位图		
IMAGE dest_img(PIC_WIDTH, PIC_HEIGHT);				// 处理后显示的位图
DWORD* img_ptr1;									// 原图片片内存指针
DWORD* img_ptr2;									// 处理后显示的位图内存指针


// 以下两个 buf 为每一个点的波幅，前者为当前波幅，后者为下一个时刻的波幅。
short* buf = new short[PIC_HEIGHT * PIC_WIDTH + PIC_WIDTH];
short* buf2 = new short[PIC_HEIGHT * PIC_WIDTH + PIC_WIDTH];


int main()
{
	// 初始化设备，加载图片
	loadimage(&src_img, _T("water.jpg"));
	HWND Hwnd = initgraph(PIC_WIDTH, PIC_HEIGHT);
	SetWindowText(Hwnd, _T("Wave-水波纹效果（点击产生一个水波纹。移动鼠标连续产生水波纹）"));
	//loadimage(&src_img, "water.jpg");		// 加载图片，大小：800*600
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(25, 0,_T( "Arial"));

	// 获得内存指针
	img_ptr1 = GetImageBuffer(&src_img);
	img_ptr2 = GetImageBuffer(NULL);

	// 初始化波幅数组
	memset(buf, 0, (PIC_HEIGHT * PIC_WIDTH + PIC_WIDTH) * sizeof(short));
	memset(buf2, 0, (PIC_HEIGHT * PIC_WIDTH + PIC_WIDTH) * sizeof(short));

	// Let's Go!
	BeginBatchDraw();		// 双缓冲，闪屏时需要
	while (true)
	{
		FrameFun();
		RenderFun();
		FlushBatchDraw();
		Sleep(1);
	}
	EndBatchDraw();

	return 0;
}

// 计算出下一个时刻所有点的波幅
void nextFrame()
{
	for (int i = PIC_WIDTH; i < PIC_HEIGHT * (PIC_WIDTH - 1); i++) {
		// 公式：X0'= (X1+X2+X3+X4) / 2 - X0
		buf2[i] = ((buf[i - PIC_WIDTH] + buf[i + PIC_WIDTH] + buf[i - 1] + buf[i + 1]) >> 1) - buf2[i];
		buf2[i] -= buf2[i] >> 5;// 波能衰减
	}
	short* ptmp = buf;
	buf = buf2;
	buf2 = ptmp;
}
// 处理当前时刻波幅影响之后的位图，保存在 dest_img 中
void RenderRipple()
{
	int i = 0;
	for (int y = 0; y < PIC_HEIGHT; y++)
		for (int x = 0; x < PIC_WIDTH; x++)
		{
			short data = 0X400 - buf[i];
			// 偏移
			int a = (((x - PIC_WIDTH / 2) * data) >> 10) + PIC_WIDTH / 2;
			int b = (((y - PIC_HEIGHT / 2) * data) >> 10) + PIC_HEIGHT / 2;
			// 边界处理
			if (a >= PIC_WIDTH)		a = PIC_WIDTH - 1;
			if (a < 0)				a = 0;
			if (b >= PIC_HEIGHT)	b = PIC_HEIGHT - 1;
			if (b < 0)				b = 0;
			// 处理偏移 
			img_ptr2[x + y * PIC_WIDTH] = img_ptr1[a + (b * PIC_WIDTH)];
			i++;
		}
}
void disturb(int x, int y, int stonesize, int stoneweight)
{
	// 突破边界不处理
	if ((x >= PIC_WIDTH - stonesize) ||
		(x < stonesize) ||
		(y >= PIC_HEIGHT - stonesize) ||
		(y < stonesize))
		return;
	for (int posx = x - stonesize; posx < x + stonesize; posx++)
		for (int posy = y - stonesize; posy < y + stonesize; posy++)
			if ((posx - x) * (posx - x) + (posy - y) * (posy - y) < stonesize * stonesize)
				buf[PIC_WIDTH * posy + posx] += stoneweight;
}

// 计算fps
float getFps()
{
#define FPS_COUNT 8
	static int i = 0;
	static auto oldTime = GetTickCount();
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

// 渲染
void RenderFun()
{
	RenderRipple();
	//putimage(0, 0, &dest_img);

	char s[5];
	sprintf(s, "%.1f", getFps());
	outtextxy(0, 0, s);
}

// 逻辑
void FrameFun()
{
	// 鼠标
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_MOUSEMOVE)
			disturb(msg.x, msg.y, 3, 256);
		else if (msg.uMsg == WM_LBUTTONDOWN)
			disturb(msg.x, msg.y, 3, 2560);
		FlushMouseMsgBuffer();
	}
	// 计算下一帧的波幅
	nextFrame();
}