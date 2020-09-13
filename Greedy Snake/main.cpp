///////////////////////////////////////////////////
// 程序名称：Greedy Snake (贪吃蛇)
// 编译环境：Visual C++ 6.0 / 2010，EasyX 2011惊蛰版
// 作　　者：wysaid (博客：<a href="http://qzone.qq.com/wysaid" target="_black">http://qzone.qq.com/wysaid</a>)
// 最后修改：2011-1-23
//
#include "Snake.h"

int main()
{
	// 初始化
	initgraph(640, 480);				// 创建绘图窗口
	setbkcolor(BLUE);					// 设置背景为蓝色
	cleardevice();						// 清屏
	setlinestyle(PS_SOLID, 5);			// 设置画线宽度
	BeginBatchDraw();					// 设置批绘图模式
	setlinecolor(YELLOW);				// 设置绘图颜色
	setfillcolor(YELLOW);				// 设置填充颜色

	Snake snake;
	int speed = 1;
	Fruit fruit(rand() % 600 + 100, rand() % 400 + 100);
	char c = 'd';
	char prior = 0;
	line(100, 240, 0, 240);
	fillcircle(100, 240, 4);
	outtextxy(100, 50, _T("Choose a level:(1, 2, 3)"));
	outtextxy(100, 100, _T("1. easy"));
	outtextxy(100, 150, _T("2. normal"));
	outtextxy(100, 200, _T("3. hard"));
	FlushBatchDraw();
	prior = _getch();
	switch (prior)
	{
	case '2': speed = 2; break;
	case '3': speed = 4; break;
	default: prior = 49;
	}
	while (c != 27)
	{
		cleardevice();
		if (_kbhit())
		{
			c = _getch();
			if (abs(prior - c) == 4 || abs(prior - c) == 3) c = prior;
			prior = c;
		}
		switch (c)
		{
		case 'w': snake.move_up(c, speed); break;
		case 's': snake.move_down(c, speed); break;
		case 'a': snake.move_left(c, speed); break;
		case 'd': snake.move_right(c, speed); break;
		case 27: break;
		default: 
			outtextxy(200, 240, _T("Error key!Press 'w''a''s''d' to move or 'ESC' to exit"));
			FlushBatchDraw(); Sleep(1000);
			c = snake.getdir();
		}

		fruit.drawfruit(snake, speed);
		snake.draw(speed);
		snake.ifhit();
		FlushBatchDraw();
		Sleep(10);
	}

	// 结束游戏，按任意键退出
	outtextxy(200, 240, _T("Bye~"));
	EndBatchDraw();
	_getch();
	closegraph();
	return 0;
}