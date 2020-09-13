// 程序名称：Hilbert (希尔伯特)曲线 (基于 L 系统绘制)
// 编译环境：Visual C++ 6.0，EasyX 2011惊蛰版
// 最后修改：2011-3-7
//
#include <graphics.h>
#include <conio.h>

int g_len;  // Hilbert 曲线的单位长度

// 递归绘制 Hilbert 曲线
void hilbert(const char* cmd, int level)
{
	static BYTE d = 0;                              // 方向
	static POINT c[4] = { 1, 0, 0, -1, -1, 0, 0, 1 }; // 方向对应的轴系数

	if (level < 0) return;

	// 处理命令字符串
	int i = 0;
	while (cmd[i])
	{
		switch (cmd[i++])
		{
		case '+':   d = (d + 1) & 3;                            break;
		case '-':   d = (d - 1) & 3;                            break;
		case 'X':   hilbert("+YF-XFX-FY+", level - 1);          break;
		case 'Y':   hilbert("-XF+YFY+FX-", level - 1);          break;
		case 'F':   linerel(c[d].x * g_len, c[d].y * g_len);    break;
		}
	}
}

// 主函数
int main()
{
	// 设置绘图环境
	initgraph(800, 600);                        // 设置窗口大小
	outtextxy(20, 550, "Press 1~8 to show other levels of Hilbert. Press ESC to exit.");
	rectangle(143, 23, 657, 537);               // 绘制表示范围的矩形框
	setorigin(144, 24);                         // 设置原点坐标
	setcolor(RED);                              // 设置颜色
	setfillstyle(BLACK);

	int level = '5';                            // 设置初始级别

	do
	{
		if (level >= '1' && level <= '8')     // 仅处理 1～8
		{
			level -= '0';                       // 转换为对应的数字值
			bar(0, 0, 511, 511);                // 清空绘图区
			g_len = 512 >> level;             // 计算单位长度
			moveto(g_len / 2, 512 - g_len / 2); // 设定起点

			hilbert("X", level);                // 递归绘制 Hilbert 曲线
		}
	} while ((level = getch()) != 27);           // 按 ESC 退出

	closegraph();
	return 0;
}