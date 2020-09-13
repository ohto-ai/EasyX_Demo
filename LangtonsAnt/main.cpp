/******************************
 * 兰顿蚂蚁
 * 编译环境：VC++ 2017
 * 作者：that boy，2018/12/19
 * 最后修改：2018/12/21
 ******************************/

#include <EasyX.h>
#include <string>
#include <conio.h>


namespace LangtonAnt
{
	// 方向
	struct Direct
	{
		int dx;
		int dy;
		Direct& TurnLeft();
		Direct& TurnRight();
	};

	Direct	LEFT{ -1,0 },
		RIGHT{ 1,0 },
		UP{ 0,-1 },
		DOWN{ 0,1 };

	IMAGE	BlockMap;		// 地图图像
	int		BlockSize;		// 方块大小
	bool* Map = nullptr;	// 地图（逻辑）
	// 地图大小（逻辑）
	int		MapWidth;
	int		MapHeight;
	// 当前方向
	Direct	CurrentDirect;
	// 当前位置
	int		CurrentX;
	int		CurrentY;
	// 是否超出边界（开始有规律）
	bool	OutMap = false;

	void Init(int, int, int, int, int, Direct = UP);	// 初始化
	const IMAGE& Update();								// 更新逻辑图像
	void Quit();										// 结束，回收内存
}


// 主函数
int main()
{
	LangtonAnt::Init(10, 80, 60, 40, 30);	// 设置方块大小 10，地图大小 80*60，起点（40,30）

	initgraph(800, 600);

	// 设置字体
	setbkcolor(WHITE);
	settextcolor(BLACK);
	settextstyle(20, 0, L"Arial");

	BeginBatchDraw();

	for (long int step = 0; !LangtonAnt::OutMap; ++step)
	{
		putimage(0, 0, &LangtonAnt::Update());
		outtextxy(0, 0, (std::to_wstring(step) + L" steps").c_str());
		FlushBatchDraw();
		Sleep(1);
	}
	LangtonAnt::Quit();

	// 居中显示结束语
	outtextxy(400 - textwidth(L"Langton's Ant is running in a loop.") / 2, 300 - textheight('L') / 2, L"Langton's Ant is running in a loop.");

	EndBatchDraw();
	_getch();
	closegraph();

	return 0;
}


// 初始化兰顿蚂蚁
void LangtonAnt::Init(int Blocksize, int MapW, int MapH, int StartX, int StartY, Direct StartDirect)
{
	// 设置数据
	MapWidth = MapW;
	MapHeight = MapH;
	CurrentX = StartX;
	CurrentY = StartY;
	CurrentDirect = StartDirect;
	OutMap = false;

	if (Map != nullptr)
		delete[]Map;

	Map = new bool[MapWidth * MapHeight]();

	BlockSize = Blocksize;
	BlockMap.Resize(MapWidth * BlockSize, MapHeight * BlockSize);

	// 填充地图图像
	SetWorkingImage(&BlockMap);
	setbkcolor(WHITE);
	cleardevice();
	SetWorkingImage();
}


// 更新逻辑和图像
const IMAGE& LangtonAnt::Update()
{
	// 处理边界
	if (CurrentX < 0 || CurrentX >= MapWidth || CurrentY < 0 || CurrentY >= MapHeight)
	{
		OutMap = true;
		return BlockMap;
	}

	if (Map[CurrentX + CurrentY * MapWidth])	// 黑色
	{
		CurrentDirect.TurnLeft();	// 左转
	}
	else
	{
		CurrentDirect.TurnRight();	// 右转
	}

	// 绘制图像
	SetWorkingImage(&BlockMap);

	// 转向
	Map[CurrentX + CurrentY * MapWidth] = !Map[CurrentX + CurrentY * MapWidth];

	// 重绘底部
	setfillcolor(Map[CurrentX + CurrentY * MapWidth] ? BLACK : WHITE);
	solidrectangle(CurrentX * BlockSize, CurrentY * BlockSize, (CurrentX + 1) * BlockSize - 1, (CurrentY + 1) * BlockSize - 1);

	// 移动
	CurrentX += CurrentDirect.dx;
	CurrentY += CurrentDirect.dy;

	// 绘制蚂蚁
	setfillcolor(RED);
	solidcircle(CurrentX * BlockSize + BlockSize / 2, CurrentY * BlockSize + BlockSize / 2, BlockSize / 3);

	SetWorkingImage();
	return BlockMap;
}


// 退出兰顿蚂蚁
void LangtonAnt::Quit()
{
	delete[]Map;
	Map = nullptr;
}


// 左转
LangtonAnt::Direct& LangtonAnt::Direct::TurnLeft()
{
	if (dx < 0 && dy == 0)			// 左
		*this = DOWN;
	else if (dx == 0 && dy < 0)		// 上
		*this = LEFT;
	else if (dx > 0 && dy == 0)	// 右
		*this = UP;
	else if (dx == 0 && dy > 0)		// 下
		*this = RIGHT;
	return *this;
}


// 右转
LangtonAnt::Direct& LangtonAnt::Direct::TurnRight()
{
	if (dx < 0 && dy == 0)			// 左
		*this = UP;
	else if (dx == 0 && dy < 0)		// 上
		*this = RIGHT;
	else if (dx > 0 && dy == 0)	// 右
		*this = DOWN;
	else if (dx == 0 && dy > 0)		// 下
		*this = LEFT;
	return *this;
}