//项目名称		方块阵
//作者			星羽1704
//email			1479245050@qq.com
//联系方式		qq 1479245050
//描述			一个方块阵，旋转移动的动态效果
//编译环境		win10 64位 + vs2017 + easyx—ver: 20180727(beta)
////////////////////////

#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>

////////////宏定义
//	常量
#define PI					3.1415926535898							//	圆周率
#define	square_root_two		1.4142135623731
#define	square_root_three	1.7320508075689
#define square_root_six		2.4494897427832
//	画布大小
#define	drawingboard_x		600										//	长
#define	drawingboard_y		600										//	高
#define	center_x			drawingboard_x/2
#define	center_y			drawingboard_y/2
#define	bkColor				BLACK									//	背景颜色
#define lColor				WHITE									//	画线颜色

//	数量
#define	layernum			10										//	层数
#define	cube_slength		42										//	正方体边长

//	时间长度
#define	FPS					60										//	帧数
#define	beginDelay			1000									//	动画开始时的静止时长
#define	endDelay			1000									//	动画结束时的静止时长
#define turn_times			90										//	旋转九十度过程中要旋转的次数
#define	everyturndeg		PI/2/turn_times							//	每次转动的角度	
#define	yi_stop				15										//	stoptime的num级延时
#define	er_stop				4										//	stoptime的num1级延时

////////////定义全局变量及结构体
class POINT3D												// 定义三维点
{
public:
	long x, y, z;
	POINT3D() {}
	POINT3D(long x, long y, long z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	POINT3D(POINT3D& P) {
		x = P.x;
		y = P.y;
		z = P.z;
	}
private:
};
struct  Cube												//	定义正方体
{
	POINT3D tP3[8];											//	初始坐标
	POINT3D P3[8];											//	当前坐标
	POINT3D P1ST[8];										//	第一象限
	POINT3D P2ED[8];										//	第二象限
	POINT3D P3RD[8];										//	第三象限

	POINT	cen_p2;											//	底面中心坐标
	POINT	p2[8];											//	投影坐标
	POINT	p1st[8];										//	第一象限
	POINT	p2ed[8];										//	第二象限
	POINT	p3rd[8];										//	第三象限

	int		Taking;											//	是否在可以运动 0否1是 初始为1 运动结束为0
	int		Stoptime;										//	旋转等待
	int		Haddone;										//	已完成帧数，到PI/2值 停止 taking变为0
}cube[(layernum * layernum)];
long	temp;												//	临时计算使用
int		end;												//	0为初始值，所有正方体停止后end变为1


////////////函数声明
//	功能函数
POINT projection(POINT3D& p3);								//	投影得到起始二维坐标

//	过程函数
void Init();												//	初始化
void rotation();											//	旋转
void Drawcube();											//	描点连线
void reflection();											//	在其他象限影射出来得到坐标 并且转化

////////////函数构建
//	程序入口
int main()
{
	//	初始化画板内容
	initgraph(drawingboard_x, drawingboard_y);
	setbkcolor(bkColor);
	setbkmode(TRANSPARENT);
	setlinecolor(lColor);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 1, NULL, 0);
	setfillcolor(bkColor);

	//	动画
	BeginBatchDraw();
	while (1)
	{
		cleardevice();
		Init();
		//	静态输出
		Drawcube();
		FlushBatchDraw();
		Sleep(beginDelay);
		//	开始动画
		while (end == 0)
		{
			cleardevice();
			rotation();
			reflection();
			Drawcube();

			FlushBatchDraw();
			Sleep(1000 / FPS);
		}
		Sleep(endDelay);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}

//	在其他象限影射出来得到坐标 并且转化
void reflection()
{
	for (int num = 0; num < layernum; num++) {
		for (int num1 = 0; num1 < (2 * num + 1); num1++) {
			for (int num2 = 0; num2 < 8; num2++) {
				// 第一象限
				cube[(num * num + num1)].P1ST[num2] = { cube[(num * num + num1)].P3[num2].x,
														cube[(num * num + num1)].P3[num2].y - 2 * cube[(num * num + num1)].cen_p2.y,
														cube[(num * num + num1)].P3[num2].z };
				cube[(num * num + num1)].p1st[num2] = projection(cube[(num * num + num1)].P1ST[num2]);
				// 第二象限
				cube[(num * num + num1)].P2ED[num2] = { cube[(num * num + num1)].P3[num2].x - 2 * cube[(num * num + num1)].cen_p2.x,
														cube[(num * num + num1)].P3[num2].y - 2 * cube[(num * num + num1)].cen_p2.y,
														cube[(num * num + num1)].P3[num2].z };
				cube[(num * num + num1)].p2ed[num2] = projection(cube[(num * num + num1)].P2ED[num2]);
				// 第三象限
				cube[(num * num + num1)].P3RD[num2] = { cube[(num * num + num1)].P3[num2].x - 2 * cube[(num * num + num1)].cen_p2.x,
														cube[(num * num + num1)].P3[num2].y,
														cube[(num * num + num1)].P3[num2].z };
				cube[(num * num + num1)].p3rd[num2] = projection(cube[(num * num + num1)].P3RD[num2]);
			}
		}
	}
}

//	旋转
void rotation()
{
	for (int num = 0; num < layernum; num++) {
		for (int num1 = 0; num1 < (2 * num + 1); num1++) {
			if (cube[(num * num + num1)].Taking == 1)
			{
				if (cube[(num * num + num1)].Stoptime != 0)
				{
					cube[(num * num + num1)].Stoptime--;
				}
				else {
					cube[(num * num + num1)].Haddone++;

					for (int num2 = 0; num2 < 8; num2++)
					{
						cube[(num * num + num1)].P3[num2].x = (long)((cube[(num * num + num1)].tP3[num2].x - cube[(num * num + num1)].cen_p2.x) * cos(cube[(num * num + num1)].Haddone * everyturndeg)
							- (cube[(num * num + num1)].tP3[num2].y - cube[(num * num + num1)].cen_p2.y) * sin(cube[(num * num + num1)].Haddone * everyturndeg) + cube[(num * num + num1)].cen_p2.x);
						cube[(num * num + num1)].P3[num2].y = (long)((cube[(num * num + num1)].tP3[num2].x - cube[(num * num + num1)].cen_p2.x) * sin(cube[(num * num + num1)].Haddone * everyturndeg)
							+ (cube[(num * num + num1)].tP3[num2].y - cube[(num * num + num1)].cen_p2.y) * cos(cube[(num * num + num1)].Haddone * everyturndeg) + cube[(num * num + num1)].cen_p2.y);
						cube[(num * num + num1)].P3[num2].y -= (long)(cube[(num * num + num1)].Haddone * 4 * cube_slength / turn_times / square_root_two);

						cube[(num * num + num1)].p2[num2] = projection(cube[(num * num + num1)].P3[num2]);
					}
				}
			}
			if (cube[(num * num + num1)].Haddone == turn_times)
				cube[(num * num + num1)].Taking = 0;
		}
	}
	if (cube[(layernum * layernum - 1)].Taking == 0)
		end = 1;
}

//	描点连线
void Drawcube()
{
	//	第一象限
	for (int num = (layernum - 1); num >= 0; num--) {
		for (int num1 = 0; num1 < (2 * num); num1++) {
			POINT pts[] = { cube[(num * num + num1)].p1st[4], cube[(num * num + num1)].p1st[5], cube[(num * num + num1)].p1st[6] ,cube[(num * num + num1)].p1st[7] };
			fillpolygon(pts, 4);
			if (cube[(num * num + num1)].Haddone <= (int)(turn_times / 2))
			{
				POINT pts1[] = { cube[(num * num + num1)].p1st[1],cube[(num * num + num1)].p1st[2],cube[(num * num + num1)].p1st[6],cube[(num * num + num1)].p1st[5] };
				POINT pts2[] = { cube[(num * num + num1)].p1st[2],cube[(num * num + num1)].p1st[3],cube[(num * num + num1)].p1st[7],cube[(num * num + num1)].p1st[6] };
				fillpolygon(pts1, 4);
				fillpolygon(pts2, 4);
			}
			else
			{
				POINT pts1[] = { cube[(num * num + num1)].p1st[2],cube[(num * num + num1)].p1st[3],cube[(num * num + num1)].p1st[7],cube[(num * num + num1)].p1st[6] };
				POINT pts2[] = { cube[(num * num + num1)].p1st[3],cube[(num * num + num1)].p1st[0],cube[(num * num + num1)].p1st[4],cube[(num * num + num1)].p1st[7] };
				fillpolygon(pts1, 4);
				fillpolygon(pts2, 4);
			}
		}
	}
	//	第二象限
	for (int num = (layernum - 1); num >= 0; num--) {
		for (int num1 = 1; num1 < (2 * num + 1); num1++) {
			POINT pts[] = { cube[(num * num + num1)].p2ed[4], cube[(num * num + num1)].p2ed[5], cube[(num * num + num1)].p2ed[6] ,cube[(num * num + num1)].p2ed[7] };
			fillpolygon(pts, 4);
			if (cube[(num * num + num1)].Haddone <= (int)(turn_times / 2))
			{
				POINT pts1[] = { cube[(num * num + num1)].p2ed[1],cube[(num * num + num1)].p2ed[2],cube[(num * num + num1)].p2ed[6],cube[(num * num + num1)].p2ed[5] };
				POINT pts2[] = { cube[(num * num + num1)].p2ed[2],cube[(num * num + num1)].p2ed[3],cube[(num * num + num1)].p2ed[7],cube[(num * num + num1)].p2ed[6] };
				fillpolygon(pts1, 4);
				fillpolygon(pts2, 4);
			}
			else
			{
				POINT pts1[] = { cube[(num * num + num1)].p2ed[2],cube[(num * num + num1)].p2ed[3],cube[(num * num + num1)].p2ed[7],cube[(num * num + num1)].p2ed[6] };
				POINT pts2[] = { cube[(num * num + num1)].p2ed[3],cube[(num * num + num1)].p2ed[0],cube[(num * num + num1)].p2ed[4],cube[(num * num + num1)].p2ed[7] };
				fillpolygon(pts1, 4);
				fillpolygon(pts2, 4);
			}
		}
	}
	//	第三象限
	for (int num = (layernum - 1); num >= 0; num--) {
		for (int num1 = 1; num1 < (2 * num); num1++) {
			POINT pts[] = { cube[(num * num + num1)].p3rd[4], cube[(num * num + num1)].p3rd[5], cube[(num * num + num1)].p3rd[6] ,cube[(num * num + num1)].p3rd[7] };
			fillpolygon(pts, 4);
			if (cube[(num * num + num1)].Haddone <= (int)(turn_times / 2))
			{
				POINT pts1[] = { cube[(num * num + num1)].p3rd[1],cube[(num * num + num1)].p3rd[2],cube[(num * num + num1)].p3rd[6],cube[(num * num + num1)].p3rd[5] };
				POINT pts2[] = { cube[(num * num + num1)].p3rd[2],cube[(num * num + num1)].p3rd[3],cube[(num * num + num1)].p3rd[7],cube[(num * num + num1)].p3rd[6] };
				fillpolygon(pts1, 4);
				fillpolygon(pts2, 4);
			}
			else
			{
				POINT pts1[] = { cube[(num * num + num1)].p3rd[2],cube[(num * num + num1)].p3rd[3],cube[(num * num + num1)].p3rd[7],cube[(num * num + num1)].p3rd[6] };
				POINT pts2[] = { cube[(num * num + num1)].p3rd[3],cube[(num * num + num1)].p3rd[0],cube[(num * num + num1)].p3rd[4],cube[(num * num + num1)].p3rd[7] };
				fillpolygon(pts1, 4);
				fillpolygon(pts2, 4);
			}
		}
	}
	//	第四象限
	for (int num = (layernum - 1); num >= 0; num--) {
		for (int num1 = 0; num1 < (2 * num + 1); num1++) {
			POINT pts[] = { cube[(num * num + num1)].p2[4], cube[(num * num + num1)].p2[5], cube[(num * num + num1)].p2[6] ,cube[(num * num + num1)].p2[7] };
			fillpolygon(pts, 4);
			if (cube[(num * num + num1)].Haddone <= (int)(turn_times / 2))
			{
				POINT pts1[] = { cube[(num * num + num1)].p2[1],cube[(num * num + num1)].p2[2],cube[(num * num + num1)].p2[6],cube[(num * num + num1)].p2[5] };
				POINT pts2[] = { cube[(num * num + num1)].p2[2],cube[(num * num + num1)].p2[3],cube[(num * num + num1)].p2[7],cube[(num * num + num1)].p2[6] };
				fillpolygon(pts1, 4);
				fillpolygon(pts2, 4);
			}
			else
			{
				POINT pts1[] = { cube[(num * num + num1)].p2[2],cube[(num * num + num1)].p2[3],cube[(num * num + num1)].p2[7],cube[(num * num + num1)].p2[6] };
				POINT pts2[] = { cube[(num * num + num1)].p2[3],cube[(num * num + num1)].p2[0],cube[(num * num + num1)].p2[4],cube[(num * num + num1)].p2[7] };
				fillpolygon(pts1, 4);
				fillpolygon(pts2, 4);
			}
		}
	}
}

//	初始化
void Init()
{
	end = 0;

	for (int num = 0; num < layernum; num++)
	{
		for (int num1 = 0; num1 < (2 * num + 1); num1++)
		{
			cube[(num * num + num1)].Haddone = 0;
			cube[(num * num + num1)].Taking = 1;

			temp = 2 * num - num1;
			if (temp == 0)
				cube[(num * num + num1)].Stoptime = num * yi_stop + num1 * er_stop;
			else
				cube[(num * num + num1)].Stoptime = num * yi_stop + (temp - 1) * er_stop;

			//	计算坐标 为了提高坐标精度，先放大再缩小倍数
			//	中心坐标
			cube[(num * num + num1)].cen_p2.x = (long)(num1 * cube_slength * square_root_two);
			cube[(num * num + num1)].cen_p2.y = (long)(-cube[(num * num + num1)].cen_p2.x + 2 * num * cube_slength * square_root_two);

			temp = (long)(cube_slength / 2 * square_root_two);
			//	八个顶点
			cube[(num * num + num1)].tP3[0] = { cube[(num * num + num1)].cen_p2.x ,cube[(num * num + num1)].cen_p2.y - temp,0 };
			cube[(num * num + num1)].tP3[1] = { cube[(num * num + num1)].cen_p2.x - temp, cube[(num * num + num1)].cen_p2.y ,0 };
			cube[(num * num + num1)].tP3[2] = { cube[(num * num + num1)].cen_p2.x ,cube[(num * num + num1)].cen_p2.y + temp,0 };
			cube[(num * num + num1)].tP3[3] = { cube[(num * num + num1)].cen_p2.x + temp, cube[(num * num + num1)].cen_p2.y ,0 };
			for (int num2 = 0; num2 < 4; num2++)
				cube[(num * num + num1)].tP3[num2 + 4] = { cube[(num * num + num1)].tP3[num2].x,cube[(num * num + num1)].tP3[num2].y ,cube_slength };

			//	转换坐标
			for (int num3 = 0; num3 < 8; num3++)
			{
				cube[(num * num + num1)].P3[num3] = cube[(num * num + num1)].tP3[num3];
				cube[(num * num + num1)].p2[num3] = projection(cube[(num * num + num1)].P3[num3]);
			}
		}
	}
	reflection();
}

//	投影得到起始二维坐标
POINT projection(POINT3D& p3)
{
	POINT p2;
	//	x直接取值
	p2.x = (long)p3.x + center_x;
	//	y用点到直线公式
	p2.y = (long)((p3.y * square_root_two - p3.z * 2) / square_root_six + center_y);
	return p2;
}