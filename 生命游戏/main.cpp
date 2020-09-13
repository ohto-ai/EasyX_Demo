///////////////////////////////////////////////////
// 程序名称：生命游戏
// 编译环境：Visual C++ 6.0，EasyX
//

#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<io.h>
// 定义全局变量
__int8 world[102][102] = { 0 }; // 定义二维世界
IMAGE imgLive, imgEmpty; // 定义活细胞和无细胞区域的图案
// 函数声明
void Init();   // 初始化
void SquareWorld();  // 创建一个细胞以方形分布的世界
void RandWorld();  // 创建一个细胞随机分布的世界
void PaintWorld();  // 绘制世界
void Evolution();  // 进化
// 主函数
int main()
{
	Init();
	BeginBatchDraw();
	int Speed = 500;  // 游戏速度（毫秒）
	while (true)
	{
		if (_kbhit() || Speed == 900)
		{
			char c = _getch();
			if (c == ' ' && Speed != 900)
				c = _getch();
			if (c >= '0' && c <= '9')
				Speed = ('9' - c) * 100;
			switch (c)
			{
			case 's':
			case 'S':
				SquareWorld(); // 产生默认的细胞以方形分布的世界
				break;
			case 'r':
			case 'R':
				RandWorld(); // 产生默认的细胞以方形分布的世界
				break;
			case VK_ESCAPE:
				goto END;
			}
		}
		Evolution();  // 进化
		PaintWorld();  // 绘制世界
		FlushBatchDraw();
		if (Speed != 900) // 速度为 900 时，为按任意键单步执行
			Sleep(Speed);
	}
END:
	closegraph();
	return 0;
}
///////////////////////////////////////////////////
// 函数定义
// 初始化
void Init()
{
	// 创建绘图窗口
	initgraph(640, 480);
	// 设置随机种子
	srand((unsigned)time(NULL));
	// 调整世界图案的大小
	Resize(&imgLive, 4, 4);
	Resize(&imgEmpty, 4, 4);
	// 绘制有生命世界的图案
	SetWorkingImage(&imgLive);
	setcolor(GREEN);
	setfillstyle(GREEN);
	fillellipse(0, 0, 3, 3);
	// 绘制无生命世界的图案
	SetWorkingImage(&imgEmpty);
	setcolor(DARKGRAY);
	rectangle(1, 1, 2, 2);
	// 恢复对默认窗口的绘图
	SetWorkingImage(NULL);
	// 输出简单说明
	settextstyle(24, 0, _T("黑体"));
	outtextxy(254, 18, _T("生 命 游 戏"));
	RECT r = { 440, 60, 620, 460 };
	settextstyle(12, 0, _T("宋体"));
	drawtext(_T(R"(生命游戏简介：
　　生命游戏包括一个二维矩形世界，这个世界中的每个方格居住
着一个活着的或死了的细胞。一个细胞在下一个时刻生死取决于相邻八个方格中活着的细胞
的数量。如果一个细胞周围的活细胞数量多于 3 个，这个细胞会因为资源匮乏而在下一个时
刻死去；如果一个位置周围有 3 个活细胞，则该位置在下一个时刻将诞生一个新的细胞；如
果一个位置周围有 2 个活细胞，则该位置的细胞生死状态保持不变；如果一个细胞周围的活
细胞少于 2 个，那么这个细胞会因太孤单而死去。这样整个生命世界才不至于太过荒凉或拥
挤，而是一种动态的平衡。

游戏控制：
 0-9: 调节速度(慢--快)
 ESC: 退出
空格: 
暂停|继续
  S: 创建细胞以方形分布的世界
  R: 创建细胞随机分布的世界))"), &r, DT_WORDBREAK);
	// 产生默认的细胞以方形分布的世界
	SquareWorld();
}
// 创建一个细胞以方形分布的世界
void SquareWorld()
{
	memset(world, 0, 102 * 102 * sizeof(__int8));
	for (int x = 1; x <= 100; x++)
		world[x][1] = world[x][100] = 1;
	for (int y = 1; y <= 100; y++)
		world[1][y] = world[100][y] = 1;
}
// 创建一个细胞随机分布的世界
void RandWorld()
{
	for (int x = 1; x <= 100; x++)
		for (int y = 1; y <= 100; y++)
			world[x][y] = rand() % 2;
}
// 绘制世界
void PaintWorld()
{
	for (int x = 1; x <= 100; x++)
		for (int y = 1; y <= 100; y++)
			putimage(16 + x * 4, 56 + y * 4, world[x][y] ? &imgLive : &imgEmpty);
}
// 进化
void Evolution()
{
	__int8 tmp[102][102] = { 0 }; // 临时数组
	int sum;
	for (int x = 1; x <= 100; x++)
	{
		for (int y = 1; y <= 100; y++)
		{
			// 计算周围活着的生命数量
			sum = world[x + 1][y] + world[x + 1][y - 1] + world[x][y - 1] + world[x - 1][y - 1]
				+ world[x - 1][y] + world[x - 1][y + 1] + world[x][y + 1] + world[x + 1][y + 1];
			// 计算当前位置的生命状态
			switch (sum)
			{
			case 3: tmp[x][y] = 1;  break;
			case 2: tmp[x][y] = world[x][y]; break;
			default: tmp[x][y] = 0;  break;
			}
		}
	}
	// 将临时数组恢复为世界
	memcpy(world, tmp, 102 * 102 * sizeof(__int8));
}