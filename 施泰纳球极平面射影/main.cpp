////////////////////////////////////////
// 程序：施泰纳球极平面射影
// 作者：Gary
// 编译环境：Visual C++ 6.0，EasyX_20211109
// 编写日期：2021-11-13

# include <math.h>
# include <graphics.h>
# include <string>
using namespace std;

// 定义一个结构体，内切圆
struct Node
{
	double x1, y1, z1, r1;
	double x2, y2, z2, r2;
	double posx, posy, r;
};

// 定义一个结构体，按钮
struct Node2
{
	int posx1, posy1, posx2, posy2;
	int mod;
	double r;
	const char* text;
};

// 定义一个类
class Gary
{
public:
	void carry();				// 主进程
	void initialization();		// 初始化
	void draw();				// 绘制，参数更新
	void draw1();				// 绘制
	void move();				// 窗口主视角

private:
	double a;					// 对称情况下，北级点对小圆投影至球后，投影点和球心连线与水平面的夹角
	double w;					// 旋转角度
	double pi;					// 圆周率π
	double R;					// 大圆半径
	double d;					// 变换前后的比例尺
	double r;					// 对称情况下，小圆半径
	double t;					// 三维坐标直线参数
	double r0;					// 旋转前，投影点与 y 轴的距离
	double b;					// 旋转前，投影点与 y 轴的夹角
	double d0;					// 变换前后的坐标偏移量
	double c;					// 对称情况下，内切圆圆心的初始角度
	int n;						// 内切圆的数量
	int exit1, exit2, exit3;	// 进程控制
	HWND hOut;					// 画布
	Node box[100];				// 内切圆
	Node2 boxm[10];				// 按钮
};

// 绘制函数，参数更新函数
void Gary::draw()
{
	int i;
	char s[10];
	cleardevice();
	// 参数初始化
	r = R * (1.0 - sin(pi / double(n))) / (1.0 + sin(pi / double(n)));
	a = 90.0 - 2 * atan(r / R) * 180.0 / pi;
	d = 2.0 / (tan((90.0 + w) / 2.0 / 180.0 * pi) + tan((90.0 - w) / 2.0 / 180.0 * pi));
	d0 = R * ((tan((90.0 + w) / 2.0 / 180.0 * pi) - tan((90.0 - w) / 2.0 / 180.0 * pi)) / 2.0);

	// 内切圆初始化
	for (i = 0; i < n; i++)
	{
		box[i].r1 = r;
		box[i].x1 = box[i].r1 * cos(2.0 * pi / double(n) * double(i) + c / 180.0 * pi);
		box[i].y1 = box[i].r1 * sin(2.0 * pi / double(n) * double(i) + c / 180.0 * pi);
		box[i].z1 = 0;
		box[i].r2 = R;
		box[i].x2 = box[i].r2 * cos(2.0 * pi / double(n) * double(i) + c / 180.0 * pi);
		box[i].y2 = box[i].r2 * sin(2.0 * pi / double(n) * double(i) + c / 180.0 * pi);
		box[i].z2 = 0;
	}

	// 小圆初始化
	i = n;
	box[i].r1 = r;
	box[i].x1 = box[i].r1 * cos(0);
	box[i].y1 = box[i].r1 * sin(0);
	box[i].z1 = 0;
	box[i].r2 = r;
	box[i].x2 = box[i].r2 * cos(pi);
	box[i].y2 = box[i].r2 * sin(pi);
	box[i].z2 = 0;

	// 大圆初始化
	i = n + 1;
	box[i].r1 = R;
	box[i].x1 = box[i].r1 * cos(0);
	box[i].y1 = box[i].r1 * sin(0);
	box[i].z1 = 0;
	box[i].r2 = R;
	box[i].x2 = box[i].r2 * cos(pi);
	box[i].y2 = box[i].r2 * sin(pi);
	box[i].z2 = 0;

	// 参数更新
	for (i = n + 1; i >= 0; i--)
	{
		// 映射至球
		t = 2.0 * R * R / (R * R + box[i].r1 * box[i].r1);
		box[i].x1 *= t;
		box[i].y1 *= t;
		box[i].z1 = (1.0 - t) * R;
		t = 2.0 * R * R / (R * R + box[i].r2 * box[i].r2);
		box[i].x2 *= t;
		box[i].y2 *= t;
		box[i].z2 = (1.0 - t) * R;

		// 旋转
		r0 = sqrt(box[i].x1 * box[i].x1 + box[i].z1 * box[i].z1);
		if (box[i].x1 == 0) { b = box[i].z1 > 0 ? 90 : -90; }
		else { b = box[i].x1 > 0 ? (atan(box[i].z1 / box[i].x1) * 180.0 / pi) : (180.0 + atan(box[i].z1 / box[i].x1) * 180.0 / pi); }
		box[i].x1 = r0 * cos((w + b) * pi / 180.0);
		box[i].y1 = box[i].y1;
		box[i].z1 = r0 * sin((w + b) * pi / 180.0);
		r0 = sqrt(box[i].x2 * box[i].x2 + box[i].z2 * box[i].z2);
		if (box[i].x2 == 0) { b = box[i].z2 > 0 ? 90 : -90; }
		else { b = box[i].x2 > 0 ? (atan(box[i].z2 / box[i].x2) * 180.0 / pi) : (180.0 + atan(box[i].z2 / box[i].x2) * 180.0 / pi); }
		box[i].x2 = r0 * cos((w + b) * pi / 180.0);
		box[i].y2 = box[i].y2;
		box[i].z2 = r0 * sin((w + b) * pi / 180.0);

		// 映射至水平面
		t = -R / (box[i].z1 - R);
		box[i].x1 *= t;
		box[i].y1 *= t;
		box[i].z1 = 0;
		t = -R / (box[i].z2 - R);
		box[i].x2 *= t;
		box[i].y2 *= t;
		box[i].z2 = 0;

		// 圆心矫正
		if (box[i].y1 != 0 && box[i].y2 != 0 && w > 0)
		{
			box[i].posy = ((box[n].x1 + box[n].x2) / 2.0 - (box[n + 1].x1 + box[n + 1].x2) / 2.0) / ((box[i].x2 - (box[n + 1].x1 + box[n + 1].x2) / 2.0) / box[i].y2 - (box[i].x1 - (box[n].x1 + box[n].x2) / 2.0) / box[i].y1);
			box[i].posx = (box[i].x2 - (box[n + 1].x1 + box[n + 1].x2) / 2.0) / box[i].y2 * box[i].posy + (box[n + 1].x1 + box[n + 1].x2) / 2.0;
		}
		else
		{
			box[i].posy = (box[i].y1 + box[i].y2) / 2.0;
			box[i].posx = (box[i].x1 + box[i].x2) / 2.0;
		}
		box[i].r = sqrt((box[i].posx - box[i].x2) * (box[i].posx - box[i].x2) + (box[i].posy - box[i].y2) * (box[i].posy - box[i].y2));

		// 坐标，半径矫正
		box[i].r *= d;
		box[i].posx -= d0;
		box[i].posx = 250 + box[i].posx * d;
		box[i].posy = 250 + box[i].posy * d;

		// 圆绘制
		fillcircle(box[i].posx, box[i].posy, box[i].r);
	}
	// 按钮更新
	boxm[5].posx1 = box[n].posx;
	boxm[5].posy1 = box[n].posy;
	boxm[6].posx1 = box[0].posx;
	boxm[6].posy1 = box[0].posy;

	// 按钮绘制
	for (i = 0; i < 5; i++)
	{
		fillrectangle(boxm[i].posx1, boxm[i].posy1, boxm[i].posx2, boxm[i].posy2);
		outtextxy(5 + boxm[i].posx1, 500 + 10 + 15, boxm[i].text);
	}

	// 参数绘制
	sprintf(s, "%0.1d", n); outtextxy(60, 500 + 10 + 15, s);
	sprintf(s, "%0.0f", w); outtextxy(170, 500 + 10 + 15, s);
	sprintf(s, "%0.0f", c); outtextxy(270, 500 + 10 + 15, s);
	FlushBatchDraw();
}

// 绘制函数
void Gary::draw1()
{
	setfillcolor(LIGHTRED);
	fillcircle(box[n].posx, box[n].posy, 5);
	fillcircle(box[0].posx, box[0].posy, 5);
	setfillcolor(WHITE);
	FlushBatchDraw();
}

// 初始化函数
void Gary::initialization()
{
	// 参数初始化
	pi = acos(-1);
	R = 200;
	w = 0;
	n = 4;
	c = 0;

	// 画布初始化
	setbkcolor(WHITE);
	setfillcolor(WHITE);
	settextcolor(BLACK);
	setlinecolor(BLACK);

	// 按钮初始化
	boxm[0].text = _T("圆数：");
	boxm[1].text = _T("旋转角：");
	boxm[2].text = _T("初始角：");
	boxm[3].text = _T("退出");
	boxm[4].text = _T("重置");
	for (int i = 0; i < 5; i++)
	{
		boxm[i].posx1 = 10 + 100 * i;
		boxm[i].posy1 = 500;
		boxm[i].posx2 = 100 + 100 * i;
		boxm[i].posy2 = 560;
		boxm[i].mod = 1;
	}
	boxm[5].mod = 2;
	boxm[5].r = 5;
	boxm[6].mod = 2;
	boxm[6].r = 5;

	draw();
	draw1();
	FlushBatchDraw();
}

// 窗口主视角函数，获取用户操作
void Gary::move()
{
	// 鼠标定义
	ExMessage m;
	char ss[10];
	int i;
	exit2 = 0;
	while (exit2 == 0)
	{
		if (MouseHit())
		{
			m = getmessage(EM_MOUSE | EM_KEY);
			// 左键单击判断
			if (m.message == WM_LBUTTONDOWN)
			{
				// 判断是否点击了按钮
				for (i = 0; i < 7; i++)
				{
					if (boxm[i].mod == 1 && m.x > boxm[i].posx1 && m.y > boxm[i].posy1 && m.x < boxm[i].posx2 && m.y < boxm[i].posy2)
					{
						break;
					}
					else if (boxm[i].mod == 2 && (m.x - boxm[i].posx1) * (m.x - boxm[i].posx1) + (m.y - boxm[i].posy1) * (m.y - boxm[i].posy1) < boxm[i].r * boxm[i].r)
					{
						break;
					}
				}

				switch (i)
				{
					// 设置圆数按钮
				case 0:
				{
					InputBox(ss, 10, "输入内切圆个数(3 ~ 99)");
					sscanf(ss, "%d", &i);
					if (i >= 3 && i <= 99)
					{
						n = int(i);
					}
					else
					{
						MessageBox(hOut, "输入错误，不在范围内", "来自小豆子的提醒", MB_OK);
					}
					draw();
					draw1();
					break;
				}

				// 设置旋转角按钮
				case 1:
				{
					InputBox(ss, 10, "输入旋转角度(0 ~ 89)");
					sscanf(ss, "%d", &i);
					if (i >= 0 && i <= 89)
					{
						w = int(i);
					}
					else
					{
						MessageBox(hOut, "输入错误，不在范围内", "来自小豆子的提醒", MB_OK);
					}
					draw();
					draw1();
					break;
				}

				// 设置初始角按钮
				case 2:
				{
					InputBox(ss, 10, "输入初始角度(0 ~ 360)");
					sscanf(ss, "%d", &i);
					if (i >= 0 && i <= 360)
					{
						c = int(i);
					}
					else
					{
						MessageBox(hOut, "输入错误，不在范围内", "来自小豆子的提醒", MB_OK);
					}
					draw();
					draw1();
					break;
				}

				// 退出按钮
				case 3:
				{
					exit2 = 1; exit1 = 1;
					break;
				}

				// 重置按钮
				case 4:
				{
					w = 0; n = 4; c = 0; draw(); draw1();
					break;
				}

				// 旋转按钮
				case 5:
				{
					exit3 = 0;
					while (exit3 == 0)
					{
						if (MouseHit())
						{
							m = getmessage(EM_MOUSE | EM_KEY);
							// 鼠标移动来旋转
							if (m.message == WM_MOUSEMOVE)
							{
								if (m.x < 250 && m.x > 50)
								{
									w = 89 * (250 - m.x) / 200;
									draw();
								}
							}

							// 左键单击退出旋转
							else if (m.message == WM_LBUTTONDOWN)
							{
								exit3 = 1;
							}
						}
					}
					draw1();
					break;
				}

				// 平移按钮
				case 6:
				{
					exit3 = 0;
					while (exit3 == 0)
					{
						if (MouseHit())
						{
							m = getmessage(EM_MOUSE | EM_KEY);
							// 鼠标移动来平移
							if (m.message == WM_MOUSEMOVE)
							{
								if (m.x < 500 && m.x > 0)
								{
									if (m.x == 250) { c = m.y > 250 ? 90 : -90; }
									else { c = m.x > 250 ? (atan(double(m.y - 250) / double(m.x - 250)) * 180.0 / pi) : (180 + atan(double(m.y - 250) / double(m.x - 250)) * 180.0 / pi); }
									draw();
								}
							}

							// 左键单击退出旋转
							else if (m.message == WM_LBUTTONDOWN)
							{
								exit3 = 1;
							}
						}
					}
					draw1();
					break;
				}
				default:break;
				}
			}
		}
	}
}

// 主进程
void Gary::carry()
{
	// 窗口定义
	hOut = initgraph(501, 600);
	SetWindowText(hOut, _T("施泰纳球极平面射影"));

	// 进程控制
	exit1 = 0;
	BeginBatchDraw();
	while (exit1 == 0)
	{
		initialization();
		move();
	}
	EndBatchDraw();
	closegraph();
}

// 主函数
int main(void)
{
	Gary G;
	G.carry();
	return 0;
}