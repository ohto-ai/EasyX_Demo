/******************************************************************************************/
// EasyPlot 是基于 EasyX 绘图库（https://www.easyx.cn）而编写的，对实时信号进行绘制的一个工具
// 以下代码为了便于学习以源文件形式展示，如果应用于工程建议采用头文件形式封装
// 本文采用Unicode字符集，ANSI环境的读者注意相关地方格式的转换
// 作者：Ephemeroptera
// 地点：安徽大学
// 联系方式：605686962@qq.com
// 最后修改时间：2018-11-29
/*******************************************************************************************/
#include <iostream>
#include <easyx.h>
#include <conio.h>
#include <sstream>
#include <string>
#define PI 3.141592

/*************** 定义动态 X 坐标结构体 ************************/
struct AxisX {
	double index; // 坐标
	double label; // 标签
};
/*************** 定义数字转 LPWSTR 字符型函数 ****************/
void num2WSTR(double x, wchar_t*& buffer);

/*************** 定义类：动态窗口 ****************************************************************************/
// 说明：动态窗口实现绘制实时信号的原理，其实是将当前信号送入定长队列 DynaQue,
//      并在每一帧进行绘制，其中队列长度由分辨率决定。最大分辨率应该 <= COLS
//      例如：DynaQue[xResolution+1] = {DynaQue[0],DynaQue[1,..,DynaQue[xRelolution]}，其中 DynaQue[N] 为最近信号
//            那么相邻点距离为：COLS/xResolution
// 当然 X 坐标也需要动态更新，同理类中也定义了队列 xAxis 对坐标进行同步更新
//      例如：xAxis[xSegment+1] = {xAxis[0],xAxis[1],..,xAxis[xSegment]}，其中 xAxis[N] 保存的是 AxisX 数据
//             那么相邻刻度距离：COLS/xSegment
/***************************************************************************************************************/
class DynaWin {
public:
	DynaWin(double XLIM, double YLIM, unsigned short xSegment, unsigned short ySegment, int xResolution);	// 构造器
	void UpateWin(double y);	// 更新窗口
	void ShowWin();				// 绘制窗口
	void InitBackGround();		// 背景初始化（静态窗口）

private:
	const int CENTERX = 50;		// 定义原点
	const int CENTERY = 200;
	const int ROWS = 150;		// 定义边长
	const int COLS = 1000;

	double XLIM;				// 定义 X 轴区间
	double YLIM;				// 定义 Y 轴区间
	unsigned short xSegment;	// 定义坐标的刻度
	unsigned short ySegment;
	int xResolution;			// x轴分辨率(决定 Dynaque 的长度)
	int DynaNum = 0;			// 已读信号个数
	double* DynaQue;			// 实时信号队列
	AxisX* xAxis;				// 实时坐标队列
	IMAGE* canvas;				// 当前画布
	IMAGE local;				// 局部背景

	void UpdateAxisX();			// 更新坐标

};
/*************************** MAIN 函数 *********************************/
int main()
{
	// 定义动态窗口：
	//            X 坐标：长度 = 2 * PI，	10 个刻度，500 分辨率（采样点）
	//            Y 坐标：长度 = 1，		5 个刻度
	DynaWin win(2 * PI, 1, 10, 5, 500);
	win.InitBackGround(); //背景初始化
	int i = 0;

	// 批量绘制：绘制一个y=sin(wt)，其中 w = 2*π*f , t = n*T0 =n/Fs，这里我们绘制 f = 5 的正弦信号
	BeginBatchDraw();
	while (1)
	{
		win.UpateWin(sin(2 * PI * 5 * i / 500));	// 更新窗
		win.ShowWin();								// 显示窗
		FlushBatchDraw();
		Sleep(10);
		++i;
	}

	EndBatchDraw();
	// 按任意键退出
	_getch();
	closegraph();
	return 0;
}

/******************* 以下是相关方法的具体实现 **************************/
// 数字转 LPWSTR 字符
void num2WSTR(double x, wchar_t*& buffer)
{
	// NUM TO WSS TO WSTR
	std::wstringstream ss;
	ss << x;
	size_t size = ss.str().length();
	buffer = new wchar_t[size + 1];
	ss >> buffer;
	buffer[size] = 0;
}

// DynaWin 构造函数
DynaWin::DynaWin(double XLIM, double YLIM, unsigned short xSegment, unsigned short ySegment, int xResolution)
{
	this->XLIM = XLIM;
	this->YLIM = YLIM;
	this->xSegment = xSegment;
	this->ySegment = ySegment;
	this->xResolution = xResolution;
	this->DynaQue = new double[xResolution + 1];
	this->xAxis = new AxisX[xSegment + 1];
}

//背景初始化
void DynaWin::InitBackGround()
{
	// 定义画布
	initgraph(1100, 400, EW_SHOWCONSOLE);
	// 初始化画笔
	setlinecolor(RED);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 3);
	// 定义坐标原点以及方向
	setorigin(CENTERX, CENTERY);
	setaspectratio(1, -1);
	// 绘制静态窗口
	rectangle(0, 150, 1000, -150);
	line(0, 0, 1000, 0);
	// 标注说明
	setbkmode(TRANSPARENT);
	settextcolor(YELLOW);
	setaspectratio(1, 1);// 纠正字体倒置
	settextstyle(30, 0, L"宋体");
	outtextxy(450, -185, L"EasyPlot");
	settextstyle(10, 0, L"宋体");
	outtextxy(850, 160, L"Author:Ephemeroptera");
	outtextxy(870, 180, L"Version:1.2.0");
	// 绘制刻度
	unsigned short yVertex = 2 * ySegment + 1;
	double yUnit = YLIM / ySegment;
	setbkmode(TRANSPARENT);
	settextcolor(YELLOW);
	settextstyle(15, 0, L"10");
	setlinestyle(PS_DASH, 1);				// 虚线
	// 水平刻度线
	for (int i = 1; i <= yVertex; ++i)
	{
		// 表明 y 刻度值
		double label = YLIM - (i - 1) * yUnit;
		wchar_t* buffer;
		num2WSTR(label, buffer);
		outtextxy(-30, (int)((i - ySegment - 1) * (ROWS / ySegment)) - 10, buffer);
		delete[] buffer;
		buffer = (wchar_t*)NULL;

		// 画出虚线
		line(0, (int)((i - ySegment - 1) * (ROWS / ySegment)), 1000, (int)((i - ySegment - 1) * (ROWS / ySegment)));
	}
	setaspectratio(1, -1);					// 恢复坐标方向
	canvas = GetWorkingImage();				// 画板地址
	getimage(&local, -50, -160, 1100, 320);	// 保存局部背景
}


// 更新窗
void DynaWin::UpateWin(double y)	// 更新 win
{
	++DynaNum;										// 信息个数
	UpdateAxisX();									// 更新AxisX

	if (DynaNum <= xResolution + 1)					// 队列未满
		DynaQue[DynaNum - 1] = y;

	else	// 队列已满
	{
		for (int i = 0; i <= xResolution - 1; ++i)	// 出队列
		{
			DynaQue[i] = DynaQue[i + 1];			// 向左移位
		}
		DynaQue[xResolution] = y;					// 尾部添加新元素
	}
}

// 绘制窗
void DynaWin::ShowWin()
{
	// 选定画板
	SetWorkingImage(canvas);

	// 背景覆盖	
	putimage(-50, -160, &local);

	// (1) 显示 AxisX
	setlinecolor(RED);
	setlinestyle(PS_DASH, 2);
	for (int j = 0; j <= xSegment - 1; ++j)
	{
		// 绘制虚线
		line((int)xAxis[j].index, -150, (int)xAxis[j].index, 150);
		// 绘制标注
		setaspectratio(1, 1);
		setbkmode(TRANSPARENT);
		settextcolor(YELLOW);
		settextstyle(15, 0, L"10");
		double label = xAxis[j].label;
		wchar_t* wstr;
		num2WSTR(label, wstr);
		outtextxy((int)xAxis[j].index - 10, 0, wstr);
		delete[] wstr;
		wstr = NULL;
		setaspectratio(1, -1);						// 回复坐标系
	}

	// (2) 显示 DynaQue
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 2);
	if (DynaNum <= xResolution)						// 队列未满
	{
		for (int i = 0; i <= DynaNum - 2; ++i)		// 0 to DynaNum-1 (DynaNum 个)
		{
			line((int)(i * COLS / xResolution), (int)(DynaQue[i] / YLIM * ROWS), (int)((i + 1) * COLS / xResolution), (int)(DynaQue[i + 1] / YLIM * ROWS));
		}
	}
	else	// 队列已满
	{
		for (int i = 0; i <= xResolution - 1; ++i)	// 0 to xResolution (xResolution + 1 个) 即显示全部 DynaQue
		{
			line((int)(i * COLS / xResolution), (int)(DynaQue[i] / YLIM * ROWS), (int)((i + 1) * COLS / xResolution), (int)(DynaQue[i + 1] / YLIM * ROWS));
		}
	}
}

// 更新坐标
void DynaWin::UpdateAxisX()
{
	if (DynaNum <= xResolution)										// 队列未满
		for (int i = 0; i <= xSegment - 1; ++i)
		{
			xAxis[i].index = (i + 1) * COLS / xSegment;
			xAxis[i].label = (i + 1) * XLIM / xSegment;
		}
	else	// 队列已满
	{

		for (int i = 0; i <= xSegment - 1; ++i)						// 向左移动
		{
			xAxis[i].index -= COLS / xResolution;
		}
		if (xAxis[0].index <= 0)									// AxisX 动态更新
		{
			int j;
			for (j = 0; j <= xSegment - 2; ++j)						// 队列更替
			{
				xAxis[j].index = xAxis[j + 1].index;
				xAxis[j].label = xAxis[j + 1].label;
			}
			xAxis[j].index = xAxis[j - 1].index + COLS / xSegment;	// 尾部添加新元素
			xAxis[j].label = xAxis[j - 1].label + XLIM / xSegment;
		}
	}
}