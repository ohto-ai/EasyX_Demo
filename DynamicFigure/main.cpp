// 项目名称		方块旋转动态
// 作者			星羽1704
// email		1479245050@qq.com
// 联系方式		qq 1479245050
// 描述			将三维空间的点投影到二维平面，感受三维正方体的旋转的视觉盛宴
//				暂时做到这点，以后要修改的话，应该是2019年的6月了。
// 编译环境		win10 64位 + vs2017 + easyx—ver: 20180727(beta)
//
#include <graphics.h>
#include <math.h>

#define		Db_x				640
#define		Db_y				640 
#define		center_x			Db_x/2
#define		center_y			Db_y/2
#define		square_root_two		1.4142135623731
#define		square_root_three	1.7320508075689
#define     square_root_six		2.4494897427832
#define		PI					3.1415926535898
#define		cubenum				29						// 正方体数量
#define		LineCOLOR			BLACK
#define		bkCOLOR				WHITE
#define		turnrate			3						// 正方体之间旋转等待
#define		Stardelay			800
#define		Enddelay			1000
#define		FPS					50						// 帧数
#define		sidegap				8						// 正方体边差


///////////////////////
// 结构定义

// 定义三维点
class POINT3D
{
public:
	double x, y, z;
	POINT3D() {}
	POINT3D(double x, double y, double z) {
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

//	定义正方体
struct  Cube
{
	double		side_length;	// 边长
	POINT3D P[8];				// 当前坐标
	POINT3D tP[8];				// 初始坐标
	POINT	p[8];				// 投影坐标
	int		taking;				// 是否在可以运动 0否1是 初始为1 运动结束为0
	int		shoptime;			// 暂停时间
	int		haddone;			// 已完成帧数，到 FPS值 停止 taking变为0
}cube[cubenum];					// cubenum个	边长低为10 公差10
int end;						// 0为初始值，所有正方体停止后end变为1
int temp;						// 临时变量，可以随便用



///////////////////////
// 函数声明

// 功能函数
POINT projection(POINT3D& P3);								// 降维投影得到起始二维坐标
// 过程函数
void Initcube();											// 初始化所有正方体信息
void rotation();											// 旋转
void Drawcube();											// 描点连线
void changing();											// 计算二维点
void ifend();												// 判断是否结束



///////////////////////
// 函数定义
int main()
{
	// 设置画布相关信息
	initgraph(Db_x, Db_y);
	setbkcolor(bkCOLOR);
	setbkmode(TRANSPARENT);
	setlinecolor(BLACK);

	BeginBatchDraw();
	while (true)
	{
		cleardevice();
		Initcube();

		// 静态输出
		Drawcube();
		FlushBatchDraw();
		Sleep(Stardelay);

		// 开始动画
		while (end == 0)
		{
			cleardevice();
			rotation();
			changing();
			Drawcube();
			FlushBatchDraw();
			ifend();
			Sleep(1000 / FPS);		          // 1000/帧数 = 延迟时间
		}
		Sleep(Enddelay);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}

// 初始化所有正方体信息
void Initcube()
{
	end = 0;
	for (int num = 0; num < cubenum; num++)
	{
		// 设置边长
		cube[num].side_length = square_root_two * (num + 1) * sidegap;

		// 设置三维点
		cube[num].tP[0] = { 0,0,0 };
		cube[num].tP[1] = { cube[num].side_length / square_root_two ,
							cube[num].side_length / square_root_two ,
							0 };
		cube[num].tP[2] = { 0,
							cube[num].side_length * square_root_two ,
							0 };
		cube[num].tP[3] = { cube[num].side_length / square_root_two * (-1) ,
							cube[num].side_length / square_root_two ,
							0 };
		for (int num1 = 4; num1 < 9; num1++)
		{
			cube[num].P[(num1 - 4)] = cube[num].tP[(num1 - 4)];
			cube[num].tP[num1] = { cube[num].P[(num1 - 4)].x ,cube[num].P[(num1 - 4)].y ,
				cube[num].P[(num1 - 4)].z + cube[num].side_length };
			cube[num].P[num1] = cube[num].tP[num1];
		}

		cube[num].taking = 1;
		cube[num].shoptime = (int)(num * turnrate);
		cube[num].haddone = 0;
	}
	// 转换得到投影点
	changing();
}

// 旋转
void rotation()
{
	// 旋转轴点为（0，cube[num].side_length / square_root_two，z)
	for (int num = 0; num < cubenum; num++)
	{
		if (cube[num].taking == 1)
		{
			if (cube[num].shoptime == 0)
			{
				// 底面做旋转操作
				for (int num1 = 0; num1 < 4; num1++)
				{
					cube[num].P[num1].x = (cube[num].tP[num1].x - 0) * cos(-cube[num].haddone * PI / FPS) - (cube[num].tP[num1].y - cube[num].side_length / square_root_two) * sin(-cube[num].haddone * PI / FPS) + 0;
					cube[num].P[num1].y = (cube[num].tP[num1].x - 0) * sin(-cube[num].haddone * PI / FPS) + (cube[num].tP[num1].y - cube[num].side_length / square_root_two) * cos(-cube[num].haddone * PI / FPS) + cube[num].side_length / square_root_two;
				}

				// 顶面
				for (int num2 = 4; num2 < 8; num2++)
				{
					cube[num].P[num2].x = cube[num].P[(num2 - 4)].x;
					cube[num].P[num2].y = cube[num].P[(num2 - 4)].y;
					cube[num].P[num2].z = cube[num].P[(num2 - 4)].z + cube[num].side_length;
				}

				cube[num].haddone++;
				if (cube[num].haddone == (FPS + 1))
					cube[num].taking = 0;
			}
			else	cube[num].shoptime--;
		}
	}
}

// 计算二维点
void changing()
{
	for (int num = 0; num < cubenum; num++)
		for (int num1 = 0; num1 < 8; num1++)
			cube[num].p[num1] = projection(cube[num].P[num1]);
}

// 描点连线
void Drawcube()
{
	for (int num = 0; num < cubenum; num++)
	{
		for (int num1 = 0; num1 < 4; num1++)
		{
			// 底层
			temp = ((num1 + 1) == 4) ? 0 : (num1 + 1);
			line(cube[num].p[num1].x, cube[num].p[num1].y, cube[num].p[temp].x, cube[num].p[temp].y);

			// 侧楞
			temp = num1 + 4;
			line(cube[num].p[num1].x, cube[num].p[num1].y, cube[num].p[temp].x, cube[num].p[temp].y);

			// 顶层
			temp = ((num1 + 5) == 8) ? 4 : (num1 + 5);
			line(cube[num].p[(num1 + 4)].x, cube[num].p[(num1 + 4)].y, cube[num].p[temp].x, cube[num].p[temp].y);
		}
	}
}

// 判断是否结束
void ifend()
{
	if (cube[(cubenum - 1)].taking == 0)
		end = 1;
}

// 投影得到起始二维坐标
POINT projection(POINT3D& p3)
{
	POINT p2;
	// x直接取值
	p2.x = (long)p3.x + center_x;
	// y用点到直线公式
	p2.y = (long)((p3.y * square_root_two - p3.z * 2) / square_root_six + center_y);
	return p2;
}
