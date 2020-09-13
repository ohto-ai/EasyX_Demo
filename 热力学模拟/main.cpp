// 项目：热力学模拟实验
// 作者：常数变易法(QQ：1375989292)
// 时间：2019.12.19
// 环境：VC6.0

// 学热力学与统计物理的时候就想设计一个程序模拟气体分子的运动，这样就能直观地检测那些热力学公式的正确性。
// 这个程序设计成所有粒子之间的力都是斥力，粒子和边界之间也有斥力。粒子在二维平面运动。

// 版权：作者原创，无抄袭，不涉及版权问题，仅用作学习

#include <graphics.h>						// 引用图形库头文件
#include <stdio.h>
#include <conio.h>
#include <math.h>
#define Round(d)	int(floor(d + 0.5))		// 四舍五入宏定义

#define Xsize	960
#define Ysize	960


class Vector2D		// 定义二维向量类
{
public:
	Vector2D() {};

	Vector2D(double a, double b)
	{
		x = a;
		y = b;
		length = sqrt(x * x + y * y);
	};

	void vectorAB(double ax, double ay, double bx, double by)	// 生成 A B 向量
	{
		x = bx - ax;
		y = by - ay;
		length = sqrt(x * x + y * y);
	};

	void minusVector(Vector2D v)		// 变成负向量
	{
		x = -v.x;
		y = -v.y;
		length = v.length;
	};

	void zeroVector()	// 零向量
	{
		x = 0;
		y = 0;
		length = 0;
	};

public:
	double x, y;
	double length;		// 向量长度
};


class Particle
{
public:
	Particle() {};

	void initParticle(double paraa, double parab, double parac, double parad)
	{
		x = paraa;
		y = parab;
		vx = parac;
		vy = parad;
		dt = 0.2;	// 时间间隔 1 毫秒
	};

	void nextT()	// 只能设计边界的作用力让粒子不越界了
	{
		x = x + (vx + ax * dt / 2) * dt;
		vx = vx + ax * dt;
		y = y + (vy + ay * dt / 2) * dt;
		vy = vy + ay * dt;
	};

public:
	double x, y;		// 记录当前位移
	double vx, vy;		// 记录当前速度
	double ax, ay;		// 记录当前合加速度
	double dt;
};


class Boundary
{
public:
	// 构造函数。设置粒子初始状态，分配相应内存空间
	Boundary(Particle* p, int n, double g, int a, int b, int c, int d)
	{
		left = a;			// 设置粒子运动边界
		top = b;
		right = c;
		bottom = d;
		G = g;				// 设置斥力系数
		num = n;
		particle = p;		// 设置边界内的粒子
		space = new Vector2D * [num];		// 分配二维数组
		for (i = 0; i < num; i++)
			space[i] = new Vector2D[num];		// 设置列
		for (i = 0; i < num; i++)
			space[i][i].zeroVector();		// 数组对角线是粒子到自己的距离，为零向量
	};

	// 析构函数。释放指针的内存，防止内存泄漏
	~Boundary()
	{
		if (particle != NULL)
		{
			delete[] particle;
			particle = NULL;
		}
		for (i = 0; i < num; i++)		// 注意撤销次序,先列后行,与设置相反
		{
			delete[] space[i];
			space[i] = NULL;
		}
		delete[] space;
		space = NULL;
	};

	// 计算分子间作用力
	void calculate()
	{
		for (i = 0; i < num; i++)		// 计算各个分子间的向量
		{
			for (j = i + 1; j < num; j++)
			{
				space[i][j].vectorAB(particle[i].x, particle[i].y, particle[j].x, particle[j].y);
				space[j][i].minusVector(space[i][j]);
				//	printf(" %d , %d , %f , %f , %f\n", i, j, particle[i].x, particle[i].y, space[j][i].length);
			}
		}

		double F;
		for (i = 0; i < num; i++)	// 计算分子间的作用力，修改力的公式可以更改力学模型
		{
			particle[i].ax = 0;
			particle[i].ay = 0;
			for (j = 0; j < i; j++)
			{
				F = -G / (space[i][j].length * space[i][j].length);			// 力是斥力，且力的大小与距离平方成反比
			//	printf(" %d , %d , %f\n", i, j, F);
				particle[i].ax += F * space[i][j].x / space[i][j].length;	// 计算粒子加速度
				particle[i].ay += F * space[i][j].y / space[i][j].length;
			}
			for (j = i + 1; j < num; j++)
			{
				F = -G / (space[i][j].length * space[i][j].length);			//力是斥力，且力的大小与距离平方成反比
				particle[i].ax += F * space[i][j].x / space[i][j].length;
				particle[i].ay += F * space[i][j].y / space[i][j].length;
			}

			F = G / (particle[i].x - left) + G / (particle[i].x - right);	// 计算边界作用力，边界作用力与距离成反比
			particle[i].ax += F;
			F = G / (particle[i].y - top) + G / (particle[i].y - bottom);	// 计算边界作用力
			particle[i].ay += F;

			particle[i].nextT();	// 计算粒子下一个位置，下面还要防止粒子跑出边界，若跃出边界，则认为在边界上发生弹性碰撞反射
			if (particle[i].x < left)
			{
				particle[i].vx = -particle[i].vx;
				particle[i].x = left - particle[i].x;
			}
			if (particle[i].x > right)
			{
				particle[i].vx = -particle[i].vx;
				particle[i].x = right - (particle[i].x - right);
			}
			if (particle[i].y < top)
			{
				particle[i].vy = -particle[i].vy;
				particle[i].y = top - particle[i].y;
			}
			if (particle[i].y > bottom)
			{
				particle[i].vy = -particle[i].vy;
				particle[i].y = bottom - (particle[i].y - bottom);
			}
		}
	};

public:
	double left, top, right, bottom;
	double dt;
	Particle* particle;		// 箱子里的粒子
	int num;				// 箱子里的粒子数

private:
	Vector2D** space;		// 记录粒子间距 ，为 num * num 的矩阵
	int i, j, k;			// 过程中间参数
	double G;				// 设分子间的力是 G / (r * r).

};


void main()
{
	initgraph(Xsize, Ysize, SHOWCONSOLE);	// 创建绘图窗口，大小为 640x480 像素

	Particle partical[100];
	for (int i = 0; i < 100; i++)
	{
		partical[i].initParticle(i / 10 * 50.0 + 400, i % 10 * 50.0 + 400, 0, 0);
	}
	Boundary bound(partical, 100, 9.80, 0, 0, Xsize, Ysize);	// 定义粒子和边界

	IMAGE image(Xsize, Ysize);		// 缓冲图片，防止出现闪屏
	while (1)
	{
		SetWorkingImage(&image);
		clearrectangle(0, 0, Xsize, Ysize);
		bound.calculate();
		for (int i = 0; i < 100; i++)
		{
			circle(Round(partical[i].x), Round(partical[i].y), 5);		// 绘制粒子
		}
		SetWorkingImage();
		putimage(0, 0, &image);
		Sleep(1);
	}
	closegraph();				// 关闭绘图窗口
}