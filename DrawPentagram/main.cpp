//项目名称		多边形万花筒动态
//作者			星羽1704
//email			1479245050@qq.com
//联系方式		qq 1479245050
//描述			由五角星反复变形的多边形，可以在宏定义直接改变数量和画布大小等，直接感受不同的视觉效果。
//编译环境		win10 64位 + vs2017 + easyx—ver: 20180727(beta)

#include <graphics.h>
#include <conio.h>
#include <math.h>

// 宏定义
#define PI				3.1415926535897932384626433832795
#define Lthickness		2										//	画线粗细
#define	Pnum			30										//	五角星数量
#define	side			4										//	最近点位
#define FPS				50										//	帧数
#define	BeginDelay		1000
#define	EndDelay		1000
#define	AllTurnDeg		4*PI									//	总共旋转角度
#define	EveryTurnDeg	PI/FPS/2								//	每次旋转角度
#define	Width			640										//	画板宽度
#define	Highth			480										//	画板高度

// 全局变量定义
struct pentagram
{
	POINT	p[10];
}Ptm[Pnum];
int temp;
int ifre;														//	是否重来
int times;														//	旋转次数

																//	TODO	：	函数声明
void init();													//	设置点位
void rotating();												//	旋转
void connectp();												//	绘画

																//	TODO	：	构造函数
int main()
{
	initgraph(Width, Highth);
	setbkcolor(WHITE);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, Lthickness, NULL, 0);

	BeginBatchDraw();
	while (1)
	{
		init();
		connectp();
		FlushBatchDraw();
		Sleep(BeginDelay);
		while (times < (4 * FPS + 1))
		{
			cleardevice();
			rotating();
			connectp();
			FlushBatchDraw();
			Sleep(1000 / FPS);
		}
		Sleep(EndDelay);
	}
	EndBatchDraw();
	_getch();
	closegraph();
	return 0;
}

// 设置点位
void init()
{
	cleardevice();
	times = 0;
	for (int num = 0; num < Pnum; num++) {
		Ptm[num].p[0] = { 0,(side * 4 * (num + 1)) };
		Ptm[num].p[1].x = (LONG)(Ptm[num].p[0].x * cos(-PI / 5) - Ptm[num].p[0].y / 2 * sin(-PI / 5));
		Ptm[num].p[1].y = (LONG)(Ptm[num].p[0].x * sin(-PI / 5) + Ptm[num].p[0].y / 2 * cos(-PI / 5));
		for (int num1 = 0; num1 < 5; num1++)
		{
			Ptm[num].p[(num1 * 2)].x = (LONG)(Ptm[num].p[0].x * cos(-2 * num1 * PI / 5) - Ptm[num].p[0].y * sin(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2)].y = (LONG)(Ptm[num].p[0].x * sin(-2 * num1 * PI / 5) + Ptm[num].p[0].y * cos(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2 + 1)].x = (LONG)(Ptm[num].p[1].x * cos(-2 * num1 * PI / 5) - Ptm[num].p[1].y * sin(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2 + 1)].y = (LONG)(Ptm[num].p[1].x * sin(-2 * num1 * PI / 5) + Ptm[num].p[1].y * cos(-2 * num1 * PI / 5));
		}
	}
}

// 绘画
void connectp()
{
	for (int num = 0; num < Pnum; num++)
		for (int num1 = 0; num1 < 10; num1++) {
			temp = ((num1 + 1) < 10) ? (num1 + 1) : 0;
			line(Ptm[num].p[num1].x + Width / 2, -Ptm[num].p[num1].y + Highth / 2, Ptm[num].p[temp].x + Width / 2, -Ptm[num].p[temp].y + Highth / 2);
		}
}

// 旋转
void rotating()
{
	for (int num = 0; num < Pnum; num++) {
		//((num%2)==0?(-1):1) 由层数判断旋转方向
		Ptm[num].p[0].x = (LONG)(0 * cos(((num % 2) == 0 ? (-1) : 1) * times * EveryTurnDeg) - (side * 4 * (num + 1)) * sin(((num % 2) == 0 ? (-1) : 1) * times * EveryTurnDeg));
		Ptm[num].p[0].y = (LONG)(0 * sin(((num % 2) == 0 ? (-1) : 1) * times * EveryTurnDeg) + (side * 4 * (num + 1)) * cos(((num % 2) == 0 ? (-1) : 1) * times * EveryTurnDeg));
		Ptm[num].p[1].x = (LONG)(Ptm[num].p[0].x * cos(-PI / 5) - Ptm[num].p[0].y / 2 * sin(-PI / 5));
		Ptm[num].p[1].y = (LONG)(Ptm[num].p[0].x * sin(-PI / 5) + Ptm[num].p[0].y / 2 * cos(-PI / 5));
		for (int num1 = 0; num1 < 5; num1++) {
			Ptm[num].p[(num1 * 2)].x = (LONG)(Ptm[num].p[0].x * cos(-2 * num1 * PI / 5) - Ptm[num].p[0].y * sin(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2)].y = (LONG)(Ptm[num].p[0].x * sin(-2 * num1 * PI / 5) + Ptm[num].p[0].y * cos(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2 + 1)].x = (LONG)(Ptm[num].p[1].x * cos(-2 * num1 * PI / 5) - Ptm[num].p[1].y * sin(-2 * num1 * PI / 5));
			Ptm[num].p[(num1 * 2 + 1)].y = (LONG)(Ptm[num].p[1].x * sin(-2 * num1 * PI / 5) + Ptm[num].p[1].y * cos(-2 * num1 * PI / 5));
		}
	}
	times++;
}