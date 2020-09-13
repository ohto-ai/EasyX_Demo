
/////////////////////////////////////////////////////////
// 程序名称：致你的七夕礼物 —— 3D Heart
// 编译环境：Visual Studio 2017，EasyX 20151015(beta)
// 作    者：Werelone <werelone.cn>
// 最后修改：2017-8-19
// 项目类型：Win32 Console Application
//

#include <easyx.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
using namespace std;

// 宏定义屏幕宽度和高度
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// 点 结构体
struct Point
{
	int x;
	int y;
};

// 粒子 结构体
// width、height 分别定义了粒子的宽和高
// x、y 表示粒子的三维坐标（z 坐标永为 0）
// color 表示该粒子的颜色
struct Atom
{
	static const int width;
	static const int height;
	int x;
	int y;
	COLORREF color;
};
const int Atom::width{ 4 };
const int Atom::height{ 4 };

// 全局变量定义

vector<Atom> _atomGroup;	// 粒子群
vector<Atom> _wordGroup;	// 文字粒子群

double _angle_x = 0;		// 绕 x 轴旋转角度
double _angle_y = 0;		// 绕 y 轴旋转角度

MOUSEMSG _lastMsg;			// 记录上次绘图时的鼠标消息
MOUSEMSG _nowMsg;			// 记录当前鼠标消息


// 函数声明

void InitQuietHeart();		// 创建静止的心形粒子群（已弃用，但仍保留）
void BeatHeart();			// 创建跳动的心脏，每运行一次该函数，心跳一帧
void InitBlessingWord();	// 创建祝福文字粒子群
void Rotate();				// 计算旋转角度
void Draw();				// 绘制三维投影
Point GetConvertPoint(int x, int y);	// 将三维坐标转化为投影后的绘图物理坐标


int main()
{
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);						// 初始化窗口
	setorigin(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);				// 设置原点为屏幕中心
	SetWindowText(GetHWnd(), _T("致你的七夕礼物 —— 3D Heart"));	// 设置窗口标题

	BeatHeart();			// 创建跳动的心脏
	InitBlessingWord();		// 创建文字
	Draw();					// 进行一次绘图
	bool beating = true;	// 控制心跳的变量

	while (true)			// 进入主循环
	{
		while (MouseHit())	// 检测有无鼠标消息
		{
			MOUSEMSG msg = GetMouseMsg();
			// 按下鼠标左键时，记录鼠标位置
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				_lastMsg = msg;
			}
			// 鼠标左键按下，且鼠标正在移动时，进行旋转
			if (msg.mkLButton && msg.uMsg == WM_MOUSEMOVE)
			{
				beating = false;	// 旋转图形时，停止心跳
				_nowMsg = msg;		// 获取当前鼠标位置
				Rotate();			// 通过对比上一次绘图时的鼠标位置，进行旋转
				_lastMsg = _nowMsg;	// 记录下这一次的鼠标位置
				Draw();				// 进行一次绘图
				circle(0, 0, 2);	// 画出坐标原点
			}
			else if (msg.mkRButton)
			{
				beating = false;	// 鼠标右键按下时，停止心跳
			}
			else
			{
				beating = true;		// 其他情况继续心跳
			}
		}
		// 心脏跳动
		if (beating)
		{
			BeatHeart();
			Draw();
		}
		// 延时
		Sleep(16);
	}
	// 关闭绘图环境
	closegraph();
	return 0;
}


// 计算心形时的辅助函数
float f(float x, float y, float z)
{
	float a = x * x + 9.0f / 4.0f * y * y + z * z - 1;
	return a * a * a - x * x * z * z * z - 9.0f / 80.0f * y * y * z * z * z;
}

// 计算心形时的辅助函数
float h(float x, float z)
{
	for (float y = 1.0f; y >= 0.0f; y -= 0.001f)
	{
		if (f(x, y, z) <= 0.0f)
		{
			return y;
		}
	}
	return 0.0f;
}

void InitQuietHeart()
{
	// 添加心形粒子群
	int _x = 0;
	int _y = -30;
	for (float z = 1.5f; z > -1.5f; z -= 0.05f)
	{
		_x = -60;
		for (float x = -1.5f; x < 1.5f; x += 0.025f)
		{
			float v = f(x, 0.0f, z);
			if (v <= 0.0f)
			{
				// 创建粒子
				Atom a;
				// 设置粒子坐标
				a.x = _x;
				a.y = _y;
				// 计算该位置颜色
				float y0 = h(x, z);
				float ny = 0.01f;
				float nx = h(x + ny, z) - y0;
				float nz = h(x, z + ny) - y0;
				float nd = 1.0f / sqrtf(nx * nx + ny * ny + nz * nz);
				float d = (nx + ny - nz) * nd * 0.5f + 0.5f;
				switch ((int)(d * 5.0f))
				{
				case 0: a.color = 0x111133; break;
				case 1: a.color = 0x111144; break;
				case 2: a.color = 0x111155; break;
				case 3: a.color = 0x111177; break;
				case 4: a.color = 0x111199; break;
				case 5: a.color = 0x1111aa; break;
				case 6: a.color = 0x2222cc; break;
				case 7: a.color = 0x2222ee; break;
				case 8: a.color = 0x3333ff; break;
				}
				_atomGroup.push_back(a);
			}
			_x++;
		}
		_y++;
	}
}

void BeatHeart()
{
	// 时间变量 t
	static float t = 0.0f;
	t += (t > 100) ? -100 : 0.1f;
	// 一系列复杂运算...
	float s = sinf(t);
	float a = s * s * s * s * 0.2f;
	// 清空粒子群
	_atomGroup.clear();
	// 添加粒子群
	int _x = 0;
	int _y = -20;
	for (float z = 1.3f; z > -1.2f; z -= 0.05f)
	{
		float tz = z * (1.2f - a);
		_x = -60;
		for (float x = -1.5f; x < 1.5f; x += 0.025f)
		{
			float tx = x * (1.2f + a);
			float v = f(tx, 0.0f, tz);
			if (v <= 0.0f)
			{
				// 创建粒子
				Atom atom;
				// 设置粒子坐标
				atom.x = _x;
				atom.y = _y;
				// 设置粒子颜色
				// 一系列复杂运算...
				float y0 = h(tx, tz);
				float ny = 0.01f;
				float nx = h(tx + ny, tz) - y0;
				float nz = h(tx, tz + ny) - y0;
				float nd = 1.0f / sqrtf(nx * nx + ny * ny + nz * nz);
				float d = (nx + ny - nz) * nd * 0.5f + 0.5f;
				switch ((int)(d * 5.0f))
				{
				case 0: atom.color = 0x111133; break;
				case 1: atom.color = 0x111144; break;
				case 2: atom.color = 0x111155; break;
				case 3: atom.color = 0x111177; break;
				case 4: atom.color = 0x111199; break;
				case 5: atom.color = 0x1111aa; break;
				case 6: atom.color = 0x2222cc; break;
				case 7: atom.color = 0x2222ee; break;
				case 8: atom.color = 0x3333ff; break;
				}
				// 添加进粒子群
				_atomGroup.push_back(atom);
			}
			_x++;
		}
		_y++;
	}
}

void InitBlessingWord()
{
	char blessing[7][34] = {	// “七夕快乐”文字
		{ 0,0,0,0,0,0,0, 0,0, 0,0,0,1,0,0,0, 0,0, 0,1,0,0,1,0,0, 0,0, 0,0,0,0,1,0,0 },
		{ 0,0,1,0,0,0,1, 0,0, 0,0,1,1,1,0,0, 0,0, 0,1,0,1,1,1,1, 0,0, 0,1,1,1,0,0,0 },
		{ 0,0,1,0,0,1,0, 0,0, 0,1,1,0,0,1,0, 0,0, 0,1,0,0,1,0,1, 0,0, 1,0,0,1,0,0,0 },
		{ 0,0,1,1,1,0,0, 0,0, 1,0,0,1,0,1,0, 0,0, 1,1,1,1,1,1,1, 0,0, 1,1,1,1,1,1,1 },
		{ 0,1,1,0,0,0,1, 0,0, 0,0,0,0,1,0,0, 0,0, 0,1,0,0,1,0,0, 0,0, 0,0,0,1,0,0,0 },
		{ 1,0,0,1,1,1,0, 0,0, 0,0,0,1,0,0,0, 0,0, 0,1,0,1,0,1,0, 0,0, 0,1,0,1,0,1,0 },
		{ 0,0,0,0,0,0,0, 0,0, 0,0,1,0,0,0,0, 0,0, 0,1,1,0,0,0,1, 0,0, 1,0,0,1,0,0,1 }
	};
	// 添加粒子群
	for (int i = 0; i < 7; i++)
	{
		for (int j = -17; j < 17; j++)
		{
			if (blessing[i][j + 17] == 1)
			{
				Atom a;
				a.x = j;
				a.y = 30 + i;
				a.color = WHITE;
				_wordGroup.push_back(a);
			}
		}
	}
}

void Rotate()
{
	// 计算旋转角度
	_angle_x += (_nowMsg.x - _lastMsg.x) / 100.0;
	_angle_y += (_nowMsg.y - _lastMsg.y) / 100.0;
	// 防止角度过大或过小
	if (fabs(_angle_x) > 2 * M_PI)
	{
		_angle_x > 0 ? _angle_x -= 2 * M_PI : _angle_x += 2 * M_PI;
	}
	if (fabs(_angle_y) > 2 * M_PI)
	{
		_angle_y > 0 ? _angle_y -= 2 * M_PI : _angle_y += 2 * M_PI;
	}
}

void Draw()
{
	static Point p;
	// 开启批量绘图
	BeginBatchDraw();
	// 清空当前画面
	cleardevice();
	// 绘制粒子群
	for (Atom a : _atomGroup)
	{
		// 设置颜色
		setfillcolor(a.color);
		// 获取投影坐标
		p = GetConvertPoint(a.x, a.y);
		// 绘制粒子
		solidrectangle(p.x - Atom::width / 2, p.y - Atom::height / 2, p.x + Atom::width / 2, p.y + Atom::height / 2);
	}
	// 绘制文字粒子群
	setfillcolor(WHITE);
	for (Atom a : _wordGroup)
	{
		// 获取投影坐标
		p = GetConvertPoint(a.x, a.y);
		// 绘制粒子
		solidrectangle(p.x - 1, p.y - 1, p.x + 1, p.y + 1);
	}
	// 停止批量绘图
	EndBatchDraw();
}

Point GetConvertPoint(int x, int y)
{
	static int x0, y0;
	// 将坐标乘以宽高
	x *= Atom::width;
	y *= Atom::height;
	// 计算转换坐标
	x0 = int(x * cos(_angle_x) + y * sin(_angle_y) * sin(_angle_x));
	y0 = int(y * cos(_angle_y) + x * sin(_angle_x) * sin(_angle_y));
	return Point({ x0, y0 });
}
