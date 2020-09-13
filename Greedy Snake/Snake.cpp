#include "Snake.h"

void Snake::draw(int speed)
{
	int b, s, n = 1;

	if (x_head == x_corner[x_corner.size() - 1])
		line(x_head, y_head, x_corner[x_corner.size() - 1], y_corner[y_corner.size() - 1]);
	else
		line(x_head, y_head, x_corner[x_corner.size() - 1], y_corner[y_corner.size() - 1]);

	fillcircle(x_head, y_head, 4);

	for (; n < x_corner.size(); ++n)
	{
		if (x_corner[n] == x_corner[n - 1])
		{
			line(x_corner[n], y_corner[n], x_corner[n - 1], y_corner[n - 1]);
			if (x_head != x_corner[n])
				continue;

			if (y_corner[n] > y_corner[n - 1])
				b = y_corner[n], s = y_corner[n - 1];
			else
				b = y_corner[n - 1], s = y_corner[n];

			if (y_head < b && y_head > s)
				gameover();
		}
		else
		{
			line(x_corner[n], y_corner[n], x_corner[n - 1], y_corner[n - 1]);
			if (y_head != y_corner[n])
				continue;

			if (x_corner[n] > x_corner[n - 1])
				b = x_corner[n], s = x_corner[n - 1];
			else
				b = x_corner[n - 1], s = x_corner[n];

			if ((x_head < b) && (x_head > s)) gameover();
		}
	}

	if (x_corner.size() != 1)
	{
		if (x_corner[0] == x_corner[1])
		{
			y_corner[0] > y_corner[1] ? y_corner[0] -= speed : y_corner[0] += speed;
			if (y_corner[0] == y_corner[1])
			{
				y_corner.erase(y_corner.begin());
				x_corner.erase(x_corner.begin());
			}
		}
		else if (y_corner[0] == y_corner[1])
		{
			x_corner[0] > x_corner[1] ? x_corner[0] -= speed : x_corner[0] += speed;
			if (x_corner[0] == x_corner[1])
			{
				y_corner.erase(y_corner.begin());
				x_corner.erase(x_corner.begin());
			}
		}
	}
	else if (x_head == x_corner[0])
		y_corner[0] > y_head ? y_corner[0] -= speed : y_corner[0] += speed;
	else
		x_corner[0] > x_head ? x_corner[0] -= speed : x_corner[0] += speed;
}

bool Fruit::eaten(Snake& snake)
{
	return (pow(float(snake.x_head - x), 2) + pow(float(snake.y_head - y), 2) < pow(float(radius), 2) + 4);
}

void Fruit::drawfruit(Snake& snake, int speed)
{
	TCHAR wch[10];
	srand(unsigned(time(NULL)));
	if (eaten(snake))
	{
		x = rand() % (640 - radius) + 8;
		y = rand() % (480 - radius) + 8;
		score += 100 * speed;
		++snake;
	}
	fillcircle(x, y, radius);
	_itot(score, wch, 10);
	outtextxy(160, 10, _T("Your score is:"));
	outtextxy(300, 10, wch);
}

void Snake::ifhit(/*...待添加(障碍物等)...*/)
{
	if (x_head < 4 || x_head > 636 || y_head < 4 || y_head > 476)
		gameover();
	//...待添加...
}

void Snake::operator++()
{
	switch (direction)
	{
	case 'w': y_head -= 8; break;
	case 's': y_head += 8; break;
	case 'a': x_head -= 8; break;
	case 'd': x_head += 8; break;
	default:;
	}
}

void gameover()
{
	for (int x = 160; x < 420; ++x)
		for (int y = 350; y < 450; ++y)
			putpixel(x, y, RED);

	setlinecolor(GREEN);
	outtextxy(200, 400, _T("You Lose! But merry xmas!"));
	outtextxy(300, 420, _T("Made by wysaid"));

	EndBatchDraw();
	getch();
	closegraph();
	exit(0);
}