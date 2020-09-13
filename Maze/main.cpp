
#include "labyrith.h"

// 主函数
int main()
{
	FrameWindow frame;	// 建立图形界面
	frame.Run();		// 运行控制函数

	while (1)			// 主循环
	{
		frame.Run();	// 运行控制
	}

	_getch();			// 当有按键的时候关闭图形
	closegraph();

	return 0;
}