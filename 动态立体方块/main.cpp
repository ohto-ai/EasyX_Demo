// 动态立体方块.cpp : Defines the entry point for the console application.
//
/***********************************************
*	程序名称：动态立体方块                     *
*	编译环境：Visual C++ 6.0 / EasyX冬至版     *
*	作者：Kenny <499508968@qq.com>             *
*	最后修改：2015.05.21                       *
***********************************************/

#include <graphics.h>
#include <conio.h>
#include <math.h>

void Block(int length);		// 画方块函数声明
void StrongBlock();			// 动态立体方块函数声明

// 主函数
int main()
{
	StrongBlock();
	return 0;
}

// 画方块函数定义
void Block(int length, COLORREF color)
{
	setorigin(getwidth() / 2, getheight() / 2);			// 设置坐标原点为窗口中心
	POINT a[4][6];						// 定义二维数组存放点，4条边，每条边分5段，也就是6个点
	int i, j;

	// 顺时针初始化点坐标，第一条边为坐标原点左上方的边
	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 6; i++)			// 初始化每条边上的6个点坐标
		{
			a[j][i].x = (int)pow(-1, j / 2 + j % 2 + 1) * length * i / 5;
			a[j][i].y = (int)pow(-1, j / 2 + 1) * length * (5 - i) / 5;
		}
	}

	setlinecolor(color);				// 设置线条颜色
	for (i = 0, j = 5; i < 6; i++, j--)
	{
		line(a[0][i].x, a[0][i].y, a[2][j].x, a[2][j].y);   // 连接第一条和第三条边上的6对点坐标
		line(a[1][i].x, a[1][i].y, a[3][j].x, a[3][j].y);	// 连接第二条和第四条边上的6对点坐标
	}
}

// 动态立体方块函数定义
void StrongBlock()
{
	initgraph(800, 800);
	int length;
	float H, S, L;
	while (!_kbhit())
	{
		length = 10;
		H = 0;
		S = 1;
		L = 0.5;
		while (length <= getwidth() / 2)
		{
			Block(length, HSLtoRGB(H, S, L));
			Sleep(150);
			length += 4;		// 长度递增
			H += (float)1.8;	// 改变颜色
		}
		length = 400;
		H = 360;
		S = 1;
		L = 0.5;
		while (length > 10)
		{
			Block(length, HSLtoRGB(H, S, L));
			Sleep(150);
			length -= 4;		// 长度递减
			H -= (float)1.8;	// 改变颜色
		}
	}
	_getch();
	closegraph();
}