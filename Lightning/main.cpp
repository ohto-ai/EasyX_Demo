/************************************************************
* 程序名称： 随机街道闪电效果
* version ： Release x86-1.0
* 程序目标： 制作简单的随机街道上空出现闪电，并实现街道在闪电时的闪光变化
* 编写系统： win10 1809专业版 x64
* 编译环境： VS2017 + EasyX_20200727
* 作者 ： 星羽
* 邮箱 ： 1479245050@qq.com
* QQ ： 1479245050
* 最后修改： 2019年2月13日
**************************************************************/
#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>

#define PI		3.1415926															// 定义圆周率
#define BRIGHT	5																	// 闪电颜色亮度(可以随意调节)

// 街道建筑物的结构体
struct Window
{
	int length;																		// 窗户的长
	int width;																		// 窗户的宽
};
// 房子类型1
struct House1
{
	int length;																		// 房子的长
	int width;																		// 房子的宽
	Window win;																		// 房子上的窗户
};
// 房子类型2
struct House2
{
	int length;																		// 房子的长
	int width;																		// 房子墙壁的高度
	int top;																		// 房子顶部尖锥的高
};

IMAGE temp;																			// 用于保存闪电前的街道图像

// 函数声明
int CirJudgeZero(int index1, int index2);											// 得到一个非零的随机数，index1、index2表示随机范围的指数
void Light(int x);																	// 绘制闪电
void Building();																	// 绘制街道(部分随机)
int DrawHou1(int Sumbor);															// 画房子类型1
int DrawHou2(int Sumbor);															// 画房子类型2
void LightChange();																	// 绘制高亮街道

// 入口函数
int main()
{
	srand((unsigned)time(NULL));													// 设置随机种子
	initgraph(640, 480);

	setorigin(0, 480);																// 设置坐标原点在左下角
	setaspectratio(1, -1);															// 设置坐标y轴反向
	setbkcolor(RGB(20, 20, 20));													// 重置背景色
	cleardevice();

	Building();																		// 先绘制街道并保存image

	int seedtime, time, Xpoint;

	while (true)																	// 鼠标右键退出程序
	{
		do {
			seedtime = rand() % 500;
		} while (seedtime <= 150);

		time = rand() % 3000 + seedtime;
		Xpoint = rand() % 590 + 50;													// 随机闪电出现的横坐标
		Sleep(time);																// 等待一段随机时间，出现闪电
		Light(Xpoint);																// 画闪电
		LightChange();																// 高光图像算法以及恢复
	}

	closegraph();
}

// 得到一个非零的随机数，index1、index2表示随机范围的指数
int CirJudgeZero(int index1, int index2)												// 循环随机数，得到一个非零的随机数，index1、index2表示随机范围的指数
{
	int rec = 0;																	// 用于传递的非零参数，初始化为0
	while (rec == 0)
	{
		rec = rand() % index1 + index2;
	}
	return rec;
}

// 绘制闪电
void Light(int x)
{
	int RanTimes = CirJudgeZero(3, 0);												// 随机是否出现双重闪电
	int EndPoi;																		// 用于记录闪电的终点
	for (int i = 0; i < RanTimes; i++)
	{
		int Xpoint = x, Ypoint = 480;
		EndPoi = Ypoint - rand() % 120 - 120;										// 随机闪电的终点
		while (Ypoint >= EndPoi)													// 判断是否到达终点 && 绘制每个小线段
		{
			int Dis = rand() % 20;													// 随机闪电每段的长度
			int Ang1 = rand() % 120 - 60;											// 随机闪电的角度(x轴)
			int Ang2 = rand() % 120 - 60;											// 随机闪电的角度(y轴)
			int NextX = Xpoint + (int)(Dis * sin(Ang1 * PI / 180));					// 计算闪电短点的下一点(x轴)
			int NextY = Ypoint - (int)(Dis * cos(Ang2 * PI / 180));					// 计算闪电短点的下一点(y轴)
			setlinecolor(RGB(125, 250, 255));										// 设置闪电颜色
			setlinestyle(PS_ENDCAP_ROUND | PS_SOLID, 2);							// 设置闪电样式(线段断点为平、实线、宽度)
			line(Xpoint, Ypoint, NextX, NextY);										// 绘制闪电
			Xpoint = NextX;															// 将端点赋值给初始点(x轴)
			Ypoint = NextY;															// 将端点赋值给初始点(y轴)
			Sleep((rand() % 100) > 66 ? 1 : 0);										// 设置停滞时间
		}
	}
}

// 画房子类型1
int DrawHou1(int Sumbor)
{
	House1 hou1;
	hou1.length = CirJudgeZero(40, 60);												// 随机房子的长
	hou1.width = CirJudgeZero(160, 80);												// 随机房子的宽
	hou1.win.length = 10;															// 窗户的长
	hou1.win.width = 8;																// 窗户的宽
	int point1[8] = { Sumbor,0,
						Sumbor + hou1.length,0,
						Sumbor + hou1.length,hou1.width,
						Sumbor,hou1.width };										// 定义房子的轮廓坐标
	setfillcolor(RGB(30, 30, 60));													// 设置房子的填充色
	solidpolygon((POINT*)point1, 4);												// 绘制填充房子(四边形)

	// 绘制房子上的窗户
	int WinLefDis = CirJudgeZero(10, 18);											// 随机窗户离左边的距离
	int WinTopDis = CirJudgeZero(10, 10);											// 随机窗户离顶部的距离
	int WinInvDis = CirJudgeZero(10, 10);											// 随机窗户与窗户之间的间隔(上下与左右都是这个值)
	for (int Ypoint = hou1.width - WinTopDis; Ypoint > hou1.width / 3;)				// 控制窗户的行数
	{
		for (int Xpoint = Sumbor + WinLefDis; Xpoint < Sumbor + hou1.length - 19;)	//  控制每行画几个窗户
		{
			int WinPoint[8] = { Xpoint ,Ypoint ,
				Xpoint + hou1.win.length ,Ypoint ,
				Xpoint + hou1.win.length ,Ypoint - hou1.win.width,
				Xpoint ,Ypoint - hou1.win.width };									// 定义窗户坐标数组
			int ran = CirJudgeZero(3, 0);											// 随机值用于判断颜色(亮着的和暗着的)
			if (ran == 1)
				setfillcolor(RGB(240, 240, 150));									// 设置窗户亮着
			else if (ran == 2)
				setfillcolor(RGB(30, 44, 40));										// 设置窗户暗着

			solidpolygon((POINT*)WinPoint, 4);										// 画无边框的填充窗(四边形)
			Xpoint += (hou1.win.length + WinInvDis);								// 窗左上角的x值每次画完都增加，方便画下一个窗户
		}
		Ypoint -= (hou1.win.width + WinInvDis);										// 窗左上角的y值每次画完都增加，方便画下一个窗户
	}
	Sumbor += hou1.length;															// x坐标加上整个房子的长，用于判断是否画出屏幕范围
	return Sumbor;																	// 返回x坐标
}

// 画房子类型2
int DrawHou2(int Sumbor)
{
	House2 hou2;
	hou2.length = CirJudgeZero(20, 80);												// 随机房子的长
	hou2.width = CirJudgeZero(40, 60);												// 随机房子的宽
	hou2.top = CirJudgeZero(10, 100);												// 随机房子的顶部的y坐标
	int point2[10] = { Sumbor,0,
						Sumbor ,hou2.width ,
						Sumbor + (hou2.length / 2),hou2.top ,
						hou2.length + Sumbor,hou2.width,
						hou2.length + Sumbor,0 };									// 定义房子的坐标
	setfillcolor(RGB(30, 44, 83));													// 设置房子的填充色
	solidpolygon((POINT*)point2, 5);												// 画填充的房子(5边形)

	// 画房子2的装饰
	int ran = CirJudgeZero(3, 0);													// 定义一个随机数，用于判断画什么样的窗户
	if (ran == 1)																	// 画圆形窗户
	{
		int ranlight = CirJudgeZero(3, 0);											// 定义一个随机数，用于判断窗户是否亮着
		if (ranlight == 1)															// 暗着的窗户
			setfillcolor(RGB(30, 44, 50));
		else if (ranlight == 2)														// 亮着的窗户
			setfillcolor(RGB(150, 200, 130));
		int radius = rand() % 10 + (hou2.width / 6);								// 定义半径
		solidcircle(Sumbor + (hou2.length / 2), hou2.width * 2 / 3, radius);		// 画填充无边框圆形窗户
	}
	else if (ran == 2)																// 画拱形窗户
	{
		int ranlight = CirJudgeZero(3, 0);											// 定义一个随机数，用于判断窗户是否亮着
		if (ranlight == 1)															// 暗着的窗户
			setfillcolor(RGB(30, 44, 50));
		else if (ranlight == 2)														// 亮着的窗户
			setfillcolor(RGB(150, 200, 130));
		int radius = rand() % 10 + (hou2.width / 6);								// 定义半径
		solidcircle(Sumbor + (hou2.length / 2), hou2.width * 3 / 5, radius);		// 拱形是由圆形和四边形嵌合组成
		solidrectangle(Sumbor + (hou2.length / 2) - radius, hou2.width * 3 / 5, Sumbor + (hou2.length / 2) + radius, hou2.width * 3 / 5 - radius);
	}
	Sumbor += hou2.length;															// x坐标加上整个房子的长，用于判断是否画出屏幕范围
	return Sumbor;
}

// 画建筑物
void Building()
{
	int index = 0;																	// 随机值，来判断绘制什么房子
	// 随机房子类型
	for (int Sumbor = 0; Sumbor < 640; )											// 记录建筑物的右边界，别让程序停不下来了
	{
		index = CirJudgeZero(3, 0);
		switch (index)
		{
		case 1:																		// 画房子类型1
			Sumbor = DrawHou1(Sumbor);
			break;
		case 2:																		// 画房子类型2
			Sumbor = DrawHou2(Sumbor);
			break;
		}
	}
	getimage(&temp, 0, 0, 640, 480);												// 记录此时的图像，为了闪电后恢复图像
}

// 闪电出现时的光
void LightChange()
{
	// 随机闪电(屏幕)的亮度
	float Lightness = (float)CirJudgeZero(10, (CirJudgeZero(15, 10) / 10));

	IMAGE image;																	// 定义一个图像对象，用于绘制高亮的图像
	getimage(&image, 0, 0, 640, 480);												// 获取图像范围
	DWORD* pMem = GetImageBuffer(&image);											// 获取指向显示缓冲区的指针
	int r, g, b;																	// 定义分别获取点的RGB颜色值
	for (int i = 0; i < (640 * 480); i++)											// 循环获取每个点的RGB值并判断是否大于255
	{
		r = min((int)(GetRValue(pMem[i]) * Lightness), 255);							// 这里所有像素点都乘以一个大于1的数值，点的颜色就变亮了
		b = min((int)(GetBValue(pMem[i]) * Lightness), 255);
		g = min((int)(GetGValue(pMem[i]) * Lightness), 255);
		pMem[i] = RGB(r, g, b);
	}
	putimage(0, 0, &image);															// 输出高光图像
	Sleep(100);																		// 停滞时间
	cleardevice();																	// 清除屏幕图像(好像putimage输出图像在顶部是有一段空隙的,因此直接清屏,就看不到空隙了)
	putimage(0, 0, &temp);															// 重新输出原来街道的图像
}