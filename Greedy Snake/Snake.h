#ifndef _SNAKE_H_
#define _SNAKE_H_

#include<vector>
#include<conio.h>
#include<easyx.h>
#include<cmath>
#include<ctime>
#include<cstdlib>
using std::vector;

class Snake
{
public:
	Snake() { direction = 'd'; x_head = 100; y_head = 240; x_corner.push_back(0), y_corner.push_back(240); }
	void operator++();
	friend class Fruit;
	void draw(int);
	void ifhit();
	void longer();
	char getdir() { return direction; }
	void move_up(char direct, int speed) { if (direction != direct) { x_corner.push_back(x_head); y_corner.push_back(y_head); }y_head -= speed; direction = direct; }
	void move_down(char direct, int speed) { if (direction != direct) { x_corner.push_back(x_head); y_corner.push_back(y_head); }y_head += speed; direction = direct; }
	void move_left(char direct, int speed) { if (direction != direct) { x_corner.push_back(x_head); y_corner.push_back(y_head); }x_head -= speed; direction = direct; }
	void move_right(char direct, int speed) { if (direction != direct) { x_corner.push_back(x_head); y_corner.push_back(y_head); }x_head += speed; direction = direct; }

private:
	char direction;
	int x_head;
	int y_head;
	vector<int> x_corner;
	vector<int> y_corner;
};

class Fruit
{
public:
	Fruit() { x = 0; y = 0; radius = 4; score = 0; }
	Fruit(int x_first, int y_first) { x = x_first; y = y_first; radius = 4; score = 0; }
	void drawfruit(Snake&, int);
	bool eaten(Snake&);
private:
	int x;
	int y;
	int radius;
	int score;
};

void gameover();
#endif