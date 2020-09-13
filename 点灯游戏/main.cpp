// 点灯游戏.cpp : Defines the entry point for the application.
//

#include    <graphics.h>
#include    <conio.h>
#define     MaxNum              14      //单边最大格子数
#define     G_length            30      //格子边长
#define     USER_LBUTTONDOWN    101
#define     USER_RBUTTONDOWN    102
#define     USER_MBUTTONDOWN    103

///////////////////////////////////////////////
void PaintGrid(int Mid_x, int Mid_y, int num, int color);   // 绘制游戏格子，初始化格子
void OnLButtonDown(MOUSEMSG m, int num);                    // 左键按下
void OnRButtonDown(int num);                                // 右键按下
void OnMButtonDown();                                       // 中键按下*暂无定义*
void welcome();                                             // 显示游戏主界面
void goodbye(int num);                                      // 显示结束画面
void NextLevel(int num);                                    // 下一关
int  GetMessage(MOUSEMSG m);                                // 获取鼠标信息
int  DispatchMessage(MOUSEMSG m, int opt);                  // 分发鼠标信息
int  JudgeFull(int num, int array[MaxNum][MaxNum]);         // 格子是否填满

///////////////////////////////////////////////
// 定义游戏格子结构体
struct Grid
{
    int left;   // 游戏区域边界
    int right;
    int top;
    int bottom;
    int array[MaxNum][MaxNum];  // 记录格子状态
    int num;    // 记录边界格子数目
}grid;


///////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    int opt, end = 0;
    grid.num = 4;
    welcome();
    PaintGrid(320, 240, grid.num, RGB(0, 255, 0));
    MOUSEMSG m;
    while (end != 1)
    {
        m = GetMouseMsg();
        opt = GetMessage(m);
        end = DispatchMessage(m, opt);
    }
    goodbye(grid.num);
    closegraph();
    return 0;
}


///////////////////////////////////////////////
// 获取鼠标信息
int GetMessage(MOUSEMSG m)
{
    //鼠标循环
    switch (m.uMsg)
    {
    case WM_LBUTTONDOWN:
        return USER_LBUTTONDOWN;
    case WM_RBUTTONDOWN:
        return USER_RBUTTONDOWN;
    case WM_MBUTTONDOWN:
        return USER_MBUTTONDOWN;
    }
    return 0;
}


///////////////////////////////////////////////
// 分发消息
int DispatchMessage(MOUSEMSG m, int opt)
{
    switch (opt)
    {
    case USER_LBUTTONDOWN:
        // 左键填色
        OnLButtonDown(m, grid.num);
        // 判断是否填满
        if (JudgeFull(grid.num, grid.array) == 1)
        {
            grid.num++;
            // 格子数目超过最大值通关
            if (grid.num > MaxNum)
            {
                return 1;
                break;
            }
            else
                NextLevel(grid.num);
        }
        break;

    case USER_RBUTTONDOWN:
        // 右键清除
        OnRButtonDown(grid.num);
        break;

    case USER_MBUTTONDOWN:
        return 1;
        break;
    }

    return 0;
}


///////////////////////////////////////////////
// 左键按下
void OnLButtonDown(MOUSEMSG m, int num)
{
    int nx, ny, x, y;
    if (m.x > grid.left && m.x<grid.right && m.y>grid.top && m.y < grid.bottom)
    {
        // 计算位置
        nx = (int)(m.x - grid.left) / G_length;
        ny = (int)(m.y - grid.top) / G_length;
        // 转换格子状态
        grid.array[nx][ny] = -grid.array[nx][ny];
        if (nx >= 0 && nx < num - 1) grid.array[nx + 1][ny] = -grid.array[nx + 1][ny];
        if (nx > 0 && nx <= num - 1) grid.array[nx - 1][ny] = -grid.array[nx - 1][ny];
        if (ny >= 0 && ny < num - 1) grid.array[nx][ny + 1] = -grid.array[nx][ny + 1];
        if (ny > 0 && ny <= num - 1) grid.array[nx][ny - 1] = -grid.array[nx][ny - 1];
        // 扫描填色
        for (nx = 0; nx < num; nx++)
            for (ny = 0; ny < num; ny++)
            {
                if (grid.array[nx][ny] == 1)
                    setfillcolor(GREEN);
                else
                    setfillcolor(BLACK);
                x = nx * G_length + grid.left;
                y = ny * G_length + grid.top;
                solidrectangle(x + 1, y + 1, x + G_length - 1, y + G_length - 1);
            }
    }
}


///////////////////////////////////////////////
// 右键按下清空
void OnRButtonDown(int num)
{
    int x, y, nx, ny;

    for (x = 0; x < num; x++)
        for (y = 0; y < num; y++)
            grid.array[x][y] = -1;

    for (nx = 0; nx < num; nx++)
        for (ny = 0; ny < num; ny++)
        {
            setfillcolor(BLACK);
            x = nx * G_length + grid.left;
            y = ny * G_length + grid.top;
            solidrectangle(x + 1, y + 1, x + G_length - 1, y + G_length - 1);
        }
}


///////////////////////////////////////////////
// 显示下一关
// 参数：
//  num:    下一关的边界格子数
void NextLevel(int num)
{
    // 清屏
    BeginBatchDraw();
    for (int y = 0; y <= 480; y += 5)
    {
        setlinecolor(RGB(0, 255, 0));
        settextcolor(RGB(0, 255, 0));
        line(0, y, 640, y);
        line(0, 480 - y, 640, 480 - y);
        outtextxy(300, y, "下一关");
        FlushBatchDraw();
        Sleep(16);
        setfillcolor(BLACK);
        solidrectangle(0, y + 4, 640, y - 5);
        solidrectangle(0, 480 - y, 640, 480 - y + 5);
    }
    EndBatchDraw();

    // 绘制下一关格子
    PaintGrid(320, 240, num, RGB(0, 255, 0));
}


///////////////////////////////////////////////
// 判断格子是否填满
// 参数：
//  num:    单边格子数目
//  array:  生成数组接收实参
int JudgeFull(int num, int array[MaxNum][MaxNum])
{
    int c = -1;
    int nx = 0, ny = 0;
    while (nx < num && ny < num)
    {
        for (nx = 0; nx < num; nx++)
            for (ny = 0; ny < num; ny++)
                if (array[nx][ny] == 1)
                    continue;
                else
                    return c;
    }
    c = 1;
    return c;
}


///////////////////////////////////////////////
// 绘制游戏格子，初始化格子
// 参数：
//  Mid_x:  屏幕中心 x 坐标
//  Mid_y:  屏幕中心 y 坐标
//  num:    单边格子数目
//  color:  格子线条颜色
void PaintGrid(int Mid_x, int Mid_y, int num, int color)
{
    int x, y, nx, ny;
    // 游戏区域大小
    grid.left = Mid_x - num * G_length / 2;
    grid.right = Mid_x + num * G_length / 2;
    grid.top = Mid_y - num * G_length / 2;
    grid.bottom = Mid_y + num * G_length / 2;
    // 绘制格子
    setlinecolor(color);
    for (x = grid.left; x <= grid.right; x += G_length)
    {
        line(x, grid.top, x, grid.bottom);
        Sleep(10);
    }
    for (y = grid.top; y <= grid.bottom; y += G_length)
    {
        line(grid.left, y, grid.right, y);
        Sleep(10);
    }
    // 外边框
    for (x = 20; x > 10; x--)
    {
        line(grid.left - x, grid.top - x, grid.right + x, grid.top - x);
        line(grid.left - x, grid.bottom + x, grid.right + x, grid.bottom + x);
        line(grid.left - x, grid.top - x, grid.left - x, grid.bottom + x);
        line(grid.right + x, grid.top - x, grid.right + x, grid.bottom + x);
        Sleep(5);
    }
    // 清空单元格
    for (x = 0; x < num; x++)
        for (y = 0; y < num; y++)
            grid.array[x][y] = -1;
    for (nx = 0; nx < num; nx++)
        for (ny = 0; ny < num; ny++)
        {
            setfillcolor(BLACK);
            x = nx * G_length + grid.left;
            y = ny * G_length + grid.top;
            solidrectangle(x + 1, y + 1, x + G_length - 1, y + G_length - 1);
        }
}


///////////////////////////////////////////////
// 显示游戏主界面
void welcome()
{
    // 初始化窗口
    initgraph(640, 480);

    // 输出屏幕提示
    cleardevice();
    settextcolor(RGB(0, 255, 0));
    settextstyle(64, 0, "黑体");
    outtextxy(70, 50, "涂格子游戏(点灯)");
    settextcolor(WHITE);
    settextstyle(16, 0, "宋体");
    outtextxy(100, 200, "每点一个格子，上下左右的格子也会做出于现状相反的动作");
    outtextxy(100, 240, "总共11关，左键填色，右键重来，中键退出");
    outtextxy(100, 280, "只是一个功能演示版本，以后再改进");
    outtextxy(400, 320, "by:doufuguolyl");
    settextstyle(16, 0, "黑体");
    outtextxy(400, 340, "Ver 0.1");

    // 实现闪烁的"按任意键继续"
    int c = 255;
    // while(Mo)
    {
        settextcolor(RGB(0, c, 0));
        outtextxy(280, 400, "按任意键继续");
        c -= 8;
        if (c < 0) c = 255;
        Sleep(20);
    }

    _getch();
    cleardevice();
}


///////////////////////////////////////////////
// 显示结束画面
void goodbye(int num)
{
    int c = 255;

    // 清屏
    BeginBatchDraw();
    for (int y = 0; y <= 480; y += 5)
    {
        setlinecolor(RGB(0, 255, 0));
        line(0, y, 640, y);
        line(0, 480 - y, 640, 480 - y);
        FlushBatchDraw();
        Sleep(16);
        setfillcolor(BLACK);
        solidrectangle(0, y + 4, 640, y - 5);
        solidrectangle(0, 480 - y, 640, 480 - y + 5);
    }
    EndBatchDraw();

    //判断显示文字
    if (num == MaxNum + 1)
    {
        settextcolor(RGB(0, c, 0));
        settextstyle(48, 0, "黑体");
        outtextxy(280, 200, "通关");
        settextstyle(20, 0, "黑体");
    }
    else
    {
        settextcolor(RGB(0, c, 0));
        settextstyle(48, 0, "黑体");
        outtextxy(200, 200, "再接再厉");
        settextstyle(20, 0, "黑体");
    }

    while (!_kbhit())
    {
        settextcolor(RGB(0, c, 0));
        outtextxy(400, 400, "QQ: 372480348");
        c -= 8;
        if (c < 0) c = 255;
        Sleep(20);
    }

    _getch();
}

