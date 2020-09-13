////////////////////////////////////////////////////////
// 程序名称：二维天体模型
// 开发工具：Visual Studio 2019，EasyX 20190529(beta)
// 项目类型：Windows Consoal Application
// 　　作者：悠远的苍穹
// 　　　QQ：2237505658
//    Email：2237505658@qq.com
// 发布日期：2020-1-6
// 最后修改：2020-1-6
//
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <list>
#include <vector>
#include <graphics.h>

using std::wstring;
using std::list;
using std::vector;
using std::to_wstring;
using std::cout;
using std::wcout;
using std::endl;

constexpr double PI = 3.14159265358;
constexpr double G = 0.1;
constexpr double T = 0.5;
constexpr int WIDTH = 960;
constexpr int HEIGHT = 640;
constexpr int BODYNUMS = 200;			// 多天体默认数量
constexpr int MAXLENGTH = 100;			// 轨道线最大长度

// 天体类
class Body
{
private:
	double x;
	double y;
	double m;
	double r;
	double vx;
	double vy;
	COLORREF color;
	int name;

	list<double> listx;					// 轨道线坐标链表 x
	list<double> listy;					// 轨道线坐标链表 y
public:
	Body()noexcept;
	Body(double X, double Y, double M, double VX, double VY, COLORREF C, const int N);

	double CaiR()noexcept;
	void Move();
	void Show()const;
	void ShowPath()const;
	wstring Message()const;
	COLORREF PutColor()const noexcept;
	friend void Gravitation(Body& bthis, Body& other)noexcept;
};


int Choose();
void MultiBody(vector<Body>& bodys);
void ThreeBody(vector<Body>& bodys);
void Draw(vector<Body>& p, const int nums);
void FpsTime(const time_t* t0, time_t* t1, const time_t* t2);



int main()
{
	srand(time(nullptr) & 0xFFFFFFFF);		// 设置随机种子
	initgraph(WIDTH, HEIGHT);
	while (true)
	{
		vector<Body> bodys;					// 天体数组
		int BodyNums = 0;					// 天体数量
		int mode = -1;
		mode = Choose();
		if (mode == 0)
		{
			closegraph();					// 关闭窗口
			return 0;						// 退出程序
		}
		else if (mode == 1)
		{
			BodyNums = BODYNUMS;			// 天体数量为多体
			MultiBody(bodys);				// 初始化多天体
		}
		else if (mode == 2)
		{
			BodyNums = 3;					// 天体数量为三体
			ThreeBody(bodys);				// 初始化三体
		}

		BeginBatchDraw();
		const time_t start = time(nullptr);
		time_t justnow = time(nullptr);
		time_t now = time(nullptr);

		MOUSEMSG Mouse = GetMouseMsg();
		while (true)
		{
			if (MouseHit())
			{
				Mouse = GetMouseMsg();
				if (Mouse.mkRButton && Mouse.x > 0 && Mouse.x < WIDTH && Mouse.y>0 && Mouse.y < HEIGHT)
					break;					// 检测到鼠标消息为右键按下，且在绘图窗口内，则退出
			}

			Draw(bodys, BodyNums);
			now = time(nullptr);
			FpsTime(&start, &justnow, &now);
			FlushBatchDraw();
			Sleep(10);
		}
		EndBatchDraw();
	}
	return 0;
}



Body::Body()noexcept
{
	x = rand() % WIDTH;
	y = rand() % HEIGHT;
	m = rand() % 3000 + 1000.0;
	r = CaiR();
	vx = (rand() & 1 ? 1 : -1) * ((rand() % 401) / 200.0);	// 速度 -2.00 ~ +2.00
	vy = (rand() & 1 ? 1 : -1) * ((rand() % 401) / 200.0);
	color = RGB(rand() % 256, rand() % 256, rand() % 256);	// 随机设置 RGB 颜色
	name = rand() % 200 + 100;
}

Body::Body(double X, double Y, double M, double VX, double VY, COLORREF C, const int N)
	:m(M), vx(VX), vy(VY), x(X), y(Y), color(C), name(N)
{
	r = CaiR();												// 初始化半径
}


double Body::CaiR()noexcept									// 计算半径
{
	return pow((m / PI) * (3.0 / 4.0), 1.0 / 3.0);			// pow(sqrt(3m/4PI),1/3)
}

void Body::Show()const
{
	if (m > 0)												// 质量为 0 的不显示
	{
		setfillcolor(color);								// 设置填充色,设置天体颜色
															// 画天体
		solidcircle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(r));
		setbkmode(TRANSPARENT);								// 字体背景透明
		settextstyle(14, 0, L"宋体");						// 设置字体样式
		settextcolor(WHITE);								// 设置字体颜色
															// 显示天体名字
		outtextxy(static_cast<int>(x), static_cast<int>(y), to_wstring(name).c_str());
	}
}

void Body::ShowPath()const
{
	if (m > 0)
	{
		setlinecolor(color);								// 设置画线颜色
		auto beginx = listx.begin();						// 初始化迭代器
		auto beginy = listy.begin();
		double ax = *beginx;
		double ay = *beginy;
		double bx = 0;
		double by = 0;
		for (vector<double>::size_type i = 0; i < listx.size(); i++)
		{
			ax = *beginx;
			ay = *beginy;
			bx = 0;
			by = 0;
			if (i + 1 == listx.size())
			{
				bx = *beginx;
				by = *beginy;
			}
			else
			{
				bx = *(++beginx);
				by = *(++beginy);

				setfillstyle(BS_SOLID);
				// 画轨道线
				line(static_cast<int>(ax), static_cast<int>(ay), static_cast<int>(bx), static_cast<int>(by));
			}
		}
	}
}

void Body::Move()
{
	x += vx * T;						// 移动坐标
	y += vy * T;

	listx.push_back(x);					// 记录位置，在链表尾添加
	listy.push_back(y);
	if (listx.size() > MAXLENGTH)		// 如果大于最大长度，从链表头删除元素
	{
		listx.pop_front();
		listy.pop_front();
	}
}

wstring Body::Message()const
{
	wstring str(L"Body #");
	str += to_wstring(name);
	str += wstring(L" Mass: ");
	str += to_wstring((int)m);

	return str;
}

COLORREF Body::PutColor()const noexcept
{
	return color;
}

void Gravitation(Body& bthis, Body& other)noexcept			// 两天体相互作用
{
	if (bthis.m > 0 && other.m > 0)							// 不与质量为 0 的发生作用
	{														// 距离判断 s<sqrt((x1-x2)^2+(y1-y2)^2) 没有相撞
		if (bthis.r + other.r < sqrt(pow((other.x - bthis.x), 2) + pow((other.y - bthis.y), 2)))
		{
			// ax=(G*m2)/(r^2)*cos(atan((y1-y2)/(x1-x2)))
			const double ax = ((G * other.m) / (pow((other.x - bthis.x), 2) + pow((other.y - bthis.y), 2)))
				* cos(atan(fabs((other.y - bthis.y) / (other.x - bthis.x))));

			// ay=(G*m2)/(r^2)*sin(atan((y1-y2)/(x1-x2)))
			const double ay = ((G * other.m) / (pow((other.x - bthis.x), 2) + pow((other.y - bthis.y), 2)))
				* sin(atan(fabs((other.y - bthis.y) / (other.x - bthis.x))));

			if (other.x > bthis.x)
				bthis.vx = bthis.vx + ax * T;				// vx=v0+axt
			else if (other.x < bthis.x)
				bthis.vx = bthis.vx - ax * T;				// vx=v0-axt
			else
				bthis.vx = bthis.vx;						// vx=vx0

			if (other.y > bthis.y)
				bthis.vy = bthis.vy + ay * T;				// vy=v0+ayt
			else if (other.y < bthis.y)
				bthis.vy = bthis.vy - ay * T;				// vy=v0-ayt
			else
				bthis.vy = bthis.vy;						// vy=vy0
		}
		else												// 发生碰撞
		{
			if (bthis.m >= other.m)							// bthis 吞噬 other
			{
				// 动量关系 v=(m1*v1+m2*v2)/(m1+m2)
				bthis.vx = (bthis.m * bthis.vx + other.m * other.vx) / (bthis.m + other.m);
				bthis.vy = (bthis.m * bthis.vy + other.m * other.vy) / (bthis.m + other.m);
				bthis.m += other.m;							// 吞噬质量
				bthis.r = bthis.CaiR();						// 更新半径

				other.m = 0;								// 被吞噬 m,r,v 清零
				other.r = 0;
				other.vx = 0;
				other.vy = 0;
			}
			else											// other 吞噬 bthis
			{

				other.vx = (bthis.m * bthis.vx + other.m * other.vx) / (bthis.m + other.m);
				other.vy = (bthis.m * bthis.vy + other.m * other.vy) / (bthis.m + other.m);
				other.m += bthis.m;
				other.r = other.CaiR();

				bthis.m = 0;
				bthis.r = 0;
				bthis.vx = 0;
				bthis.vy = 0;
			}
		}
	}
}



int Choose()
{
	cleardevice();											// 清屏
	settextcolor(0x5555FF);
	settextstyle(14, 0, L"宋体");
	outtextxy(190, 620, L"提示：左键进行选择 在多天体模拟下，天体数量默认为200个 进入模拟后鼠标右键退出模拟");

	setlinecolor(WHITE);
	settextcolor(WHITE);
	settextstyle(20, 0, L"宋体");
	rectangle(120, 280, 320, 380);							// 画边框
	rectangle(380, 280, 580, 380);
	rectangle(640, 280, 840, 380);
	outtextxy(420, 140, L"二维天体模拟");
	outtextxy(180, 320, L"三体模拟");
	outtextxy(430, 320, L"多天体模拟");
	outtextxy(720, 320, L"退出");

	MOUSEMSG Mouse = GetMouseMsg();
	while (true)
	{
		Mouse = GetMouseMsg();
		if (Mouse.mkLButton)								// 左键是否按下
		{													// 三体模拟
			if (Mouse.x >= 100 && Mouse.x <= 300 && Mouse.y >= 300 && Mouse.y <= 420)
			{
				FlushMouseMsgBuffer();						// 清空鼠标消息队列
				return 2;
			}
			// 多天体模拟
			if (Mouse.x >= 360 && Mouse.x <= 560 && Mouse.y >= 300 && Mouse.y <= 420)
			{
				FlushMouseMsgBuffer();						// 清空鼠标消息队列
				return 1;
			}
			// 退出
			if (Mouse.x >= 620 && Mouse.x <= 820 && Mouse.y >= 300 && Mouse.y <= 420)
			{
				setfillcolor(WHITE);
				settextcolor(RED);
				setbkmode(TRANSPARENT);						// 背景透明
				settextstyle(20, 0, L"宋体");
				solidroundrect(320, 260, 640, 420, 80, 100);// 画空心圆角矩形
				outtextxy(440, 330, L"已退出!");
				Sleep(1000);
				closegraph();								// 关闭窗口
				return 0;
			}
		}
	}
}


void MultiBody(vector<Body>& bodys)							// 初始化多天体
{
	for (int i = 0; i < BODYNUMS; i++)
	{														// 初始化天体
		bodys.push_back(Body(rand() % WIDTH, rand() % HEIGHT, rand() % 500 + 10.0, 0, 0,
			COLORREF(RGB(rand() % 256, rand() % 256, rand() % 256)), i));
	}
	// 中心天体
	bodys.at(0) = Body(WIDTH / 2, HEIGHT / 2, 20000, 0, 0, RED, 0);
}


void ThreeBody(vector<Body>& bodys)							// 初始化三体
{
	// 随机速度-1.50~+1.50
	const double temp_vx = (rand() & 1 ? 1 : -1) * (rand() % 151 / 100.0);
	const double temp_vy = (rand() & 1 ? 1 : -1) * (rand() % 151 / 100.0);
	const int temp_m = rand() % 5001 + 5000;				// 随机质量5000~10000

	// 这里让其中两个天体的速度大小相等，速度方向相反，令系统总动量为0
	// 但是，因为种种误差，运行后往往系统总动量不为0
	// 同样的，在多天体模拟中，初始时，系统总动量为0
	// 在只受万有引力的作用下，也出现了运行后系统总动量不为0

															// 随机位置，质量，颜色
	bodys.push_back(Body(rand() % (WIDTH - 240) + 120.0, rand() % (HEIGHT - 240) + 120.0, rand() % 5001 + 5000.0, 0, 0,
		COLORREF(RGB(rand() % 256, rand() % 256, rand() % 256)), 0));

	bodys.push_back(Body(rand() % (WIDTH - 240) + 120.0, rand() % (HEIGHT - 240) + 120.0, temp_m, temp_vx, temp_vy,
		COLORREF(RGB(rand() % 256, rand() % 256, rand() % 256)), 1));

	bodys.push_back(Body(rand() % (WIDTH - 240) + 120.0, rand() % (HEIGHT - 240) + 120.0, temp_m, -temp_vx, -temp_vy,
		COLORREF(RGB(rand() % 256, rand() % 256, rand() % 256)), 2));
}


void Draw(vector<Body>& p, const int nums)
{
	cleardevice();
	settextcolor(WHITE);									// 设置字体颜色
	int num = 1;
	for (int i = 0; i < nums; i++)
	{
		p.at(i).Show();										// 画天体
		p.at(i).Move();										// 移动
		p.at(i).ShowPath();									// 画轨迹线
		if (p.at(i).CaiR() > 0)								// 如果天体有质量(半径)则显示信息
		{
			settextcolor(p.at(i).PutColor());
			// 显示天体信息
			outtextxy(0, num * 12, p.at(i).Message().c_str());
			num++;
		}

		for (int j = 0; j < nums; j++)
		{
			if (i != j)										// 不与自己发生作用
				Gravitation(p.at(i), p.at(j));				// 两天体之间相互作用
		}
	}
}


void FpsTime(const time_t* t0, time_t* t1, const time_t* t2)// 计算显示FPS
{
	if (!t0 || !t1 || !t2)									// 空指针检查
		return;
	static int FPS = 0;										// 计算显示的FPS
	static int fps = 0;										// 计算使用的fps
	static wstring fpsTime;

	if (*t2 - *t1 == 1)										// 初始化各值
	{
		FPS = ++fps;
	}

	fpsTime = L"FPS: ";
	fpsTime += to_wstring(FPS);
	fpsTime += L" 运行时间: ";
	fpsTime += to_wstring(*t2 - *t0);
	fpsTime += L"s";
	settextcolor(LIGHTMAGENTA);
	outtextxy(0, 0, fpsTime.c_str());

	if (*t2 - *t1 == 1)
	{
		fps = 0;
		*t1 = time(nullptr);
	}
	else
	{
		fps++;
	}
}