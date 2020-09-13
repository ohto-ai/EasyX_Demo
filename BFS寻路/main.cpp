// 程序名称：bfs 寻路 demo
// 作者：光棱坦克
// QQ：3152239285
// 完成日期：2016-7-16
// easyx 版本：2015-10-15
//
#include <graphics.h>
#include <stdio.h>
#include <memory.h>
#include <queue>
using namespace std;

const int WIDTH = 24, HEIGHT = 24;

struct Block
{
    BYTE x, y, depth, data[200];
    bool searched, isblock;

    void getdata(Block father, int dx, int dy)
    {
        depth = father.depth + 1;
        memcpy(data, father.data, (depth - 1) * sizeof(BYTE) * 2);
        data[2 * depth - 2] = father.x + dx;
        data[2 * depth - 1] = father.y + dy;
    }
    void clean()
    {
        memset(data, 0, sizeof(char) * 200);
        searched = false;
        depth = 0;
    }

}map[WIDTH][HEIGHT];

MOUSEMSG msg;
bool success = false;
int startx = 0, starty = 0, endx = WIDTH - 1, endy = HEIGHT - 1, mousemode = 1;
queue<Block> q;

void map_search(int targx, int targy)
{
    if (targx >= 0 && targy >= 0 && targx < WIDTH && targy < HEIGHT && !map[targx][targy].searched)
    {
        map[targx][targy].searched = true;
        if (map[targx][targy].isblock)
            return;
        if (targx == endx && targy == endy)
        {
            success = true;
            int i;
            char jieguo[5000] = "", text[100];
            for (i = 0; i < map[endx][endy].depth; i++)
            {
                char temp[20];
                if (i != map[endx][endy].depth - 1)
                    sprintf(temp, "(%d,%d)->", map[endx][endy].data[i * 2], map[endx][endy].data[i * 2 + 1]);
                else
                    sprintf(temp, "(%d,%d)", map[endx][endy].data[i * 2], map[endx][endy].data[i * 2 + 1]);

                strcat(jieguo, temp);

                if (i != map[endx][endy].depth - 1)
                {
                    setfillcolor(RED);
                    fillrectangle(map[endx][endy].data[i * 2] * 20, map[endx][endy].data[i * 2 + 1] * 20, map[endx][endy].data[i * 2] * 20 + 20,
                        map[endx][endy].data[i * 2 + 1] * 20 + 20);
                }
            }
            sprintf(text, "Success, total: %d steps.", map[endx][endy].depth);
            MessageBox(GetHWnd(), jieguo, text, 0);
        }
        if (targx + 1 < WIDTH)
        {
            map[targx + 1][targy].getdata(map[targx][targy], 1, 0);
            q.push(map[targx + 1][targy]);
        }
        if (targy + 1 < HEIGHT)
        {
            map[targx][targy + 1].getdata(map[targx][targy], 0, 1);
            q.push(map[targx][targy + 1]);
        }
        if (targx > 0)
        {
            map[targx - 1][targy].getdata(map[targx][targy], -1, 0);
            q.push(map[targx - 1][targy]);
        }
        if (targy > 0)
        {
            map[targx][targy - 1].getdata(map[targx][targy], 0, -1);
            q.push(map[targx][targy - 1]);
        }
    }
}

void mapclean()
{
    int i, j;
    for (i = 0; i < WIDTH; i++)
        for (j = 0; j < HEIGHT; j++)
        {
            map[i][j].clean();
            map[i][j].isblock = false;
            map[i][j].x = i;
            map[i][j].y = j;
        }
    startx = 0;
    starty = 0;
    endx = WIDTH - 1;
    endy = HEIGHT - 1;
    success = false;
}

void runxunlu()
{
    int i, j;
    for (i = 0; i < WIDTH; i++)
        for (j = 0; j < HEIGHT; j++)
        {
            map[i][j].clean();
            map[i][j].x = i;
            map[i][j].y = j;
        }
    success = false;

    q.push(map[startx][starty]);

    while (!q.empty() && !success)
    {
        map_search(q.front().x, q.front().y);
        q.pop();
    }

    if (!success)
        MessageBox(GetHWnd(), "Fail", "Tip", 0);

    while (!q.empty())
        q.pop();
}

IMAGE buffer(WIDTH * 20 + 160, HEIGHT * 20);
void render()
{
    SetWorkingImage(&buffer);

    cleardevice();

    setbkmode(TRANSPARENT);

    setfillcolor(GREEN);
    fillrectangle(WIDTH * 20, 0, WIDTH * 20 + 160, HEIGHT * 20);

    setfillcolor(RED);

    fillrectangle(WIDTH * 20, 0, WIDTH * 20 + 160, 30);
    outtextxy(WIDTH * 20 + 10, 10, "Set start");

    fillrectangle(WIDTH * 20, 60, WIDTH * 20 + 160, 90);
    outtextxy(WIDTH * 20 + 10, 70, "Set end");

    fillrectangle(WIDTH * 20, 120, WIDTH * 20 + 160, 150);
    outtextxy(WIDTH * 20 + 10, 130, "Set block");

    fillrectangle(WIDTH * 20, 180, WIDTH * 20 + 160, 210);
    outtextxy(WIDTH * 20 + 10, 190, "Go");

    fillrectangle(WIDTH * 20, 240, WIDTH * 20 + 160, 270);
    outtextxy(WIDTH * 20 + 10, 250, "Reset");

    setfillcolor(BLUE);
    fillrectangle(startx * 20, starty * 20, startx * 20 + 20, starty * 20 + 20);

    setfillcolor(BROWN);
    fillrectangle(endx * 20, endy * 20, endx * 20 + 20, endy * 20 + 20);

    setfillcolor(YELLOW);
    int i, j;
    for (i = 0; i < WIDTH; i++)
        for (j = 0; j < HEIGHT; j++)
        {
            if (map[i][j].isblock)
                fillrectangle(i * 20, j * 20, i * 20 + 20, j * 20 + 20);
        }
    SetWorkingImage();
    putimage(0, 0, &buffer);

}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    initgraph(WIDTH * 20 + 160, HEIGHT * 20);

    while (true)
    {
        while (MouseHit())
        {
            msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN)
            {
                if (msg.x >= WIDTH * 20)
                {
                    if (msg.y >= 0 && msg.y <= 30)
                        mousemode = 1;

                    else if (msg.y >= 60 && msg.y <= 90)
                        mousemode = 2;

                    else if (msg.y >= 120 && msg.y <= 150)
                        mousemode = 3;

                    else if (msg.y >= 180 && msg.y <= 210)
                        runxunlu();

                    else if (msg.y >= 240 && msg.y <= 270)
                        mapclean();
                }
                else
                {
                    switch (mousemode)
                    {
                    case 1:
                        startx = msg.x / 20;
                        starty = msg.y / 20;
                        break;
                    case 2:
                        endx = msg.x / 20;
                        endy = msg.y / 20;
                        break;
                    case 3:
                        map[msg.x / 20][msg.y / 20].isblock = !map[msg.x / 20][msg.y / 20].isblock;
                        break;
                    }
                }
            }
        }
        render();
    }
    return 0;
}