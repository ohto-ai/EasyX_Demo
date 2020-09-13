// 程序名称：谢宾斯基(Sierpinski)三角形，也叫垫片
// 编译环境：Visual C++ 6.0，EasyX 2011惊蛰版
// 最后更新：2010-11-16
//
#include <easyx.h>
#include <conio.h>
#include <time.h>

void main()
{
	srand((unsigned)time(NULL));						// 设置随机种子
	POINT P[3] = { {320, 50}, {120, 400}, {520, 400} };	// 设定三角形的三个顶点
	POINT p = { rand() % 640, rand() % 480 };				// 随机产生当前点

	// 初始化图形模式
	initgraph(640, 480);

	// 绘制三万个点
	int n;
	for (int i = 0; i <= 30000; i++)
	{
		n = rand() % 3;
		p.x = (p.x + P[n].x) / 2;
		p.y = (p.y + P[n].y) / 2;
		putpixel(p.x, p.y, GREEN);
	}

	// 按任意键退出
	_getch();
	closegraph();
}