///////////////////////////////////////////////////
// 程序名称：力学：鼠标用弹簧挂着一串小方块
// 编译环境：Visual C++ 6.0 / 2010，EasyX 20120404(beta)
// 原 作 品：很早以前收藏的一个 javascript 脚本，源地址忘了
// 移植作者：yangw80 <yw80@qq.com>
// 最后修改：2012-4-27
//
#include <graphics.h>
#include <math.h>
#include <time.h>

const	int		WIDTH = 640;		// 窗口宽
const	int		HEIGHT = 480;		// 窗口高
const	int		BOXCOUNT = 10;		// 方块的数量
const	int		BOXSIZE = 10;		// 方块的边长
const	double	DELTAT = 0.01;		// 时间粒度
const	double	SEGLEN = 10;		// 一根弹簧的静止长度
const	double	SPRINGK = 10;		// 弹簧的弹力系数
const	double	MASS = 1;		// 质量
const	double	GRAVITY = 50;		// 重力加速度
const	double	RESISTANCE = 10;		// 空气的阻力系数（阻力=速度*阻力系数）
const	double	STOPVEL = 0.1;		// 速度的临界值（低于此值将忽略）
const	double	STOPACC = 0.1;		// 重力加速度的临界值（低于此值将忽略）
const	double	BOUNCE = 0.75;		// 边框的弹力（反弹速度 = 原速度 * BOUNCE）


//////////////////////////////
// 浮点坐标
//
class FPOINT
{
public:
	double X, Y;

	FPOINT(double x, double y)
	{
		X = x;
		Y = y;
	}
};



//////////////////////////////
// 方块对象
//
class BOX
{
private:
	double oldx, oldy;

public:
	double X, Y;
	double dx, dy;

	// 构造函数
	BOX()
	{
		oldx = X = dx = 0;
		oldy = Y = dy = 0;
	}

	// 设置方块是否可以见
	void setvisible(bool visible)
	{
		if (visible)
			rectangle(round(X), round(Y), round(X) + BOXSIZE - 1, round(Y) + BOXSIZE - 1);
	}

	// 绘制方块
	void draw()
	{
		//rectangle(round(oldx), round(oldy), round(oldx) + BOXSIZE - 1, round(oldy) + BOXSIZE - 1);
		rectangle(round(X), round(Y), round(X) + BOXSIZE - 1, round(Y) + BOXSIZE - 1);
		oldx = X;
		oldy = Y;
	}
};



//////////////////////////////
// 方块数组
//
BOX g_boxes[BOXCOUNT];



//////////////////////////////
// 计算 g_boxes[i] 和 g_boxes[j] 之间的拉力
//
void springForce(int i, int j, FPOINT* spring)
{
	double dx = (g_boxes[i].X - g_boxes[j].X);
	double dy = (g_boxes[i].Y - g_boxes[j].Y);
	double len = sqrt(dx * dx + dy * dy);

	if (len > SEGLEN)
	{
		double springF = SPRINGK * (len - SEGLEN);
		spring->X += (dx / len) * springF;
		spring->Y += (dy / len) * springF;
	}
}



//////////////////////////////
// 动画过程
//
void Animate()
{
	// g_boxes[0] 表示鼠标位置，不绘制方块

	cleardevice();
	for (int i = 1; i < BOXCOUNT; i++)
	{
		FPOINT spring(0, 0);

		// 计算每个方块受前后方块的拉力
		springForce(i - 1, i, &spring);
		if (i < (BOXCOUNT - 1))
			springForce(i + 1, i, &spring);

		// 空气阻力
		FPOINT resist(-g_boxes[i].dx * RESISTANCE, -g_boxes[i].dy * RESISTANCE);

		// 计算新的加速度
		FPOINT accel((spring.X + resist.X) / MASS, (spring.Y + resist.Y) / MASS + GRAVITY);

		// 计算新的速度
		g_boxes[i].dx += (DELTAT * accel.X);
		g_boxes[i].dy += (DELTAT * accel.Y);

		// 接近静止时使其不再运动
		if (fabs(g_boxes[i].dx) < STOPVEL && fabs(g_boxes[i].dy) < STOPVEL &&
			fabs(accel.X) < STOPACC && fabs(accel.Y) < STOPACC)
		{
			g_boxes[i].dx = 0;
			g_boxes[i].dy = 0;
		}

		// 计算移动到的新位置
		g_boxes[i].X += g_boxes[i].dx;
		g_boxes[i].Y += g_boxes[i].dy;

		// 墙壁的反弹（天花板不反弹）
		if (g_boxes[i].X > WIDTH - BOXSIZE)
		{
			g_boxes[i].X = WIDTH - BOXSIZE;	if (g_boxes[i].dx > 0)	g_boxes[i].dx = BOUNCE * -g_boxes[i].dx;
		}
		if (g_boxes[i].Y > HEIGHT - BOXSIZE)
		{
			g_boxes[i].Y = HEIGHT - BOXSIZE;	if (g_boxes[i].dy > 0)	g_boxes[i].dy = BOUNCE * -g_boxes[i].dy;
		}
		if (g_boxes[i].X < 0)
		{
			g_boxes[i].X = 0;					if (g_boxes[i].dx < 0)	g_boxes[i].dx = BOUNCE * -g_boxes[i].dx;
		}

		// 在新位置画出方块
		line(g_boxes[i - 1].X + BOXSIZE / 2, g_boxes[i - 1].Y + BOXSIZE
			, g_boxes[i].X + BOXSIZE / 2, g_boxes[i].Y);
		g_boxes[i].draw();

	}
}



//////////////////////////////
// 延时器
//
void MySleep(int t)
{
	static clock_t oldclock = clock();	// 静态变量，记录上一次 tick

	t *= CLOCKS_PER_SEC / 1000;			// 将毫秒转换为 tick
	oldclock += t;						// 更新 tick

	while (clock() < oldclock)			// 延时
		Sleep(1);						// 释放 CPU 控制权，降低 CPU 占用率
}



//////////////////////////////
// 主函数
//
void main()
{
	// 初始化绘图环境
	initgraph(WIDTH, HEIGHT);			// 设置绘图窗口宽高
	setcolor(GREEN);					// 设置绘图颜色
	setwritemode(R2_XORPEN);			// 设置异或绘图模式
	BeginBatchDraw();					// 开始批量绘图模式

	// 初始化所有方块
	for (int i = 0; i < BOXCOUNT; i++)
	{
		g_boxes[i].setvisible(i != 0);	// 除了第 0 个方块，都设置为可见
		g_boxes[i].draw();
	}


	// 进入主循环

	MOUSEMSG m;							// 保存鼠标消息的变量

	while (true)
	{
		// 处理鼠标消息
		while (MouseHit())
		{
			m = GetMouseMsg();			// 获取一条鼠标消息

			switch (m.uMsg)
			{
			case WM_MOUSEMOVE:
				g_boxes[0].X = m.x;	// g_boxes[0] 表示鼠标位置（不绘制）
				g_boxes[0].Y = m.y;
				break;

			case WM_RBUTTONUP:		// 按鼠标右键退出程序
				return;
			}
		}

		// 绘制一帧动画并显示
		Animate();
		FlushBatchDraw();

		// 延时 20 毫秒
		MySleep(20);
	}
}