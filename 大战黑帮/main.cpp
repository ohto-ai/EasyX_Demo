#include "ObjectDefine.h"


int main()
{
	int enemy_generate_gap;
	float enemy_MAX;
	char scoretext[10];


	srand((unsigned)time(NULL));		//初始化随机函数
	InitImages();						//加载图片
	mciSendString("open res\\backmusic.wma alias BackMusic", NULL, 0, NULL); // 打开音乐
	mciSendString("setaudio BackMusic volume to 300", NULL, 0, NULL);		//设置音量大小
	mciSendString("play BackMusic repeat", NULL, 0, NULL);				//循环播放

	initgraph(640, 480, EW_NOCLOSE);			//初始化画布	
	setbkmode(TRANSPARENT);

RESTART:
	enemy_generate_gap = 6;
	enemy_MAX = 5;
	for (int i = 0; i < 5; i++)					//初始化5个敌人
		addEnemy(new Enemy);

	while (1)							//主循环
	{
		BeginBatchDraw();
		putimage(0, 0, &background);		//重新绘制画布
		for (int i = 0; i < bp; i++)				//更新子弹的运动、行为
		{
			bullets[i]->draw();
			if (!(bullets[i]->act()))
				deleteBullet(bullets[i]);
		}



		if (ep <= enemy_MAX && enemy_generate_gap <= 0)
		{
			addEnemy(new Enemy);
			enemy_generate_gap = 6;
		}

		for (int i = 0; i < ep; i++)				//更新敌人的运动、行为
		{
			enemies[i]->act();
			enemies[i]->draw();
		}
		/*if(hero.judgeDie())				//若英雄死亡，则游戏结束
		{
			if(GameOver())
				return;
			else
				goto RESTART;
		}*/
		GetCommand();					//获得命令
		hero.act();						//更新英雄的运动、行为
		if (cmd.onClick)					//若鼠标左键点击则开火
			hero.fire();
		hero.draw();

		if (cmd.onSpace)					//空格键放技能
			hero.skill();

		for (int i = 0; i < ep; i++)				//若有敌人被杀死，则新创建敌人
		{
			if (enemies[i]->judgeDie())
			{
				score += (int)enemy_MAX;
				deleteEnemy(enemies[i]);
			}
		}

		enemy_MAX += 0.02f;
		if (enemy_generate_gap > 0) enemy_generate_gap--;

		hero.drawStatus();
		sprintf(scoretext, "Score:%d", score);
		settextcolor(BLACK);
		outtextxy(200, 0, scoretext);
		settextcolor(WHITE);
		EndBatchDraw();
		FlushMouseMsgBuffer();
		Sleep(5);
	}//EOF while(1)

	return 0;
}
///////////data.cpp///////////////////

void GetCommand()
{
	MOUSEMSG m;
	cmd.onSpace = false;
	cmd.key = NULL;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { deleteAll(); exit(0); }
	if (GetAsyncKeyState(65) & 0x8000) cmd.key = LEFT;
	if (GetAsyncKeyState(87) & 0x8000) cmd.key = UP;
	if (GetAsyncKeyState(83) & 0x8000) cmd.key = DOWN;
	if (GetAsyncKeyState(68) & 0x8000) cmd.key = RIGHT;
	if (GetAsyncKeyState(68) & 0x8000) cmd.key = RIGHT;
	if (GetAsyncKeyState(80) & 0x8000)
	{
		outtextxy(100, 100, "Pause...");
		FlushBatchDraw(100, 100, 100 + textwidth("Pause..."), 100 + textheight("Pause..."));
		system("pause");
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) cmd.onSpace = true;

	if (MouseHit())
	{
		m = GetMouseMsg();
		cmd.onClick = m.mkLButton;
		cmd.mx = m.x;
		cmd.my = m.y;
	}


}

bool addEnemy(Enemy* p)
{
	if (ep > 20) return false;
	//else
	enemies[ep] = p;
	ep++;
	return true;
}
bool addBullet(Bullet* p)
{
	if (bp > 20) return false;
	//else
	bullets[bp] = p;
	bp++;
	return true;
}
bool deleteEnemy(Enemy* p)
{
	int i;
	for (i = 0; i < ep; i++)
		if (p == enemies[i]) break;
	if (i == ep) return false;
	//else
	delete p;
	for (; i < ep - 1; i++)
		enemies[i] = enemies[i + 1];
	ep--;
	return true;
}
bool deleteBullet(Bullet* p)
{
	int i;
	for (i = 0; i < bp; i++)
		if (p == bullets[i]) break;
	if (i == bp) return false;
	//else
	delete p;
	for (; i < bp - 1; i++)
		bullets[i] = bullets[i + 1];
	bp--;
	return true;
}
void deleteAll()
{
	while (ep > 0)
		deleteEnemy(enemies[0]);
	while (bp > 0)
		deleteBullet(bullets[0]);
}
bool GameOver()				//游戏结束
{
	char gameovertext[256];
	int highest_score;
	std::ifstream fin("data.dat", std::ios::in | std::ios::_Nocreate);
	std::ofstream fout;
	if (!fin)
	{
		highest_score = 0;
		fin.open("data.dat", std::ios::in);
		fin.close();
	}
	else fin >> highest_score;		//读取最高分

	if (score > highest_score)
	{
		sprintf(gameovertext, "Game Over!\nScore:%d\nNew High Score!(Yes to Retry,No to Exit)", score);
		highest_score = score;
	}
	else
	{
		sprintf(gameovertext, "Game Over!\nScore:%d\nHighest Score:%d\n(Yes to Retry,No to Exit)", score, highest_score);
	}
	if (MessageBox(GetHWnd(), gameovertext, "Game Over", MB_YESNO) == IDYES)
	{
		fout.open("data.dat", std::ios::out);
		fout << highest_score;
		fout.close();		//写入最高分
		deleteAll();
		hero.reset();
		return false;
	}
	else
	{
		deleteAll();
		fout.open("data.dat", std::ios::out);
		fout << highest_score;
		fout.close();
		return true;
	}

}



Hero::Hero() :v(4), fullHP(100), fullMP(100)
{

	x = 320;
	y = 240;
	HP = fullHP;
	MP = 50;
	MP_recover_gap = 0;
	attack_gap = 0;
	direction = 0;
	acting_style = 0;

}
Hero::~Hero()
{}
void Hero::act()
{
	switch (cmd.key)
	{
	case UP:
		y -= v * 1;
		if (y < 10) y = 10;
		break;
	case LEFT:
		x -= v * 1;
		if (x < 10) x = 10;
		direction = 1;
		break;
	case DOWN:
		y += v * 1;
		if (y > 470) y = 470;
		break;
	case RIGHT:
		x += v * 1;
		if (y > 630) y = 630;
		direction = 0;
		break;
	}
	acting_style += 0.35f;
	if (acting_style > 3.6)acting_style = 0;
	MP_recover();
}
void Hero::damaged()
{
	HP -= 10;
}
bool Hero::judgeDie()
{
	if (HP <= 0) return true;
	//else
	return false;
}
void Hero::MP_recover()
{
	if (MP_recover_gap == 0)
	{
		if (MP < 100) MP++;
		MP_recover_gap = 12;
	}
	else
		MP_recover_gap--;
}
void Hero::fire()
{
	if (attack_gap == 0)
	{
		addBullet(new Bullet);
		attack_gap = 2;
		if (cmd.mx > x)
			direction = 0;
		else
			direction = 1;
		acting_style = 4;
	}
	else
		attack_gap--;
}
void Hero::draw()
{
	putimage(x - 20, y - 20, &hero_img[(int)(5 * direction + acting_style)][0], SRCAND);
	putimage(x - 20, y - 20, &hero_img[(int)(5 * direction + acting_style)][1], SRCINVERT);
}
Bullet::Bullet() :v(20)
{
	x = hero.x;
	y = hero.y;
	vx = v * (cmd.mx - hero.x) / sqrt((cmd.mx - hero.x) * (cmd.mx - hero.x) + (cmd.my - hero.y) * (cmd.my - hero.y));
	vy = v * (cmd.my - hero.y) / sqrt((cmd.mx - hero.x) * (cmd.mx - hero.x) + (cmd.my - hero.y) * (cmd.my - hero.y));
}
bool Bullet::act()
{
	x += (int)vx * 1;
	y += (int)vy * 1;
	if (x < 0 || x>640 || y < 0 || y>480) return false;
	//else
	for (int i = 0; i < ep; i++)
	{
		if (sqrt((x - enemies[i]->x) * (x - enemies[i]->x) + (y - enemies[i]->y) * (y - enemies[i]->y)) < 15)
		{
			damage(i);
			return false;
		}
	}
	//else
	return true;
}
void Bullet::draw()
{
	setfillcolor(YELLOW);
	solidcircle(x, y, 3);
}
void Bullet::damage(int i)
{
	enemies[i]->damaged();
}

Bullet::~Bullet()
{}
Enemy::Enemy() :v(2), fullHP(4)
{
	do
	{
		x = rand() % 640;
		y = rand() % 480;
	} while (abs(x - hero.x) < 300 && abs(y - hero.y) < 200);
	HP = fullHP;
	attack_gap = 0;
	acting_style = 0;
	direction = 0;

}
Enemy::~Enemy()
{}
void Enemy::act()
{
	if (acting_style < 5)
	{
		if (sqrt((x - hero.x) * (x - hero.x) + (y - hero.y) * (y - hero.y)) <= 20 && attack_gap < 6)
		{
			acting_style = 5;
			if (x <= hero.x) direction = 0;
			else direction = 1;
		}
		else
		{
			if (x > hero.x) direction = 1;
			else direction = 0;

			if (abs(y - hero.y) > 2)
			{
				if (y > hero.y) y -= v;
				else y += v;
			}
			else if (abs(x - hero.x) > 16)
			{
				if (x > hero.x) x -= v;
				else x += v;
			}
			acting_style += 0.19f;
			if (acting_style > 4.8) acting_style = 0;
		}
	}
	else
	{
		if (acting_style > 6.8) acting_style = 0;
		else if (acting_style < 6) acting_style += 0.17f;
		else if (acting_style >= 6) damage(), acting_style += 0.17f;
	}
	if (attack_gap > 0) attack_gap--;
}
void Enemy::damage()
{
	if (attack_gap == 0)
	{
		hero.damaged();
		attack_gap = 24;
	}
	else
		attack_gap--;
}
void Enemy::damaged()
{
	HP--;
}
void Enemy::draw()
{
	putimage(x - 20, y - 20, &enemy_img[(int)(7 * direction + acting_style)][0], SRCAND);
	putimage(x - 20, y - 20, &enemy_img[(int)(7 * direction + acting_style)][1], SRCINVERT);
}
bool Enemy::judgeDie()
{
	if (HP <= 0) return true;
	else return false;
}
void Hero::drawStatus()
{
	char HPtext[20], MPtext[20];

	sprintf(HPtext, "HP       %d/%d", HP, fullHP);
	sprintf(MPtext, "MP       %d/%d", MP, fullMP);

	rectangle(20, 1, 116, 20);
	rectangle(20, 22, 116, 42);
	setfillcolor(RED);
	solidrectangle(21, 2, 15 + HP, 19);
	setfillcolor(BLUE);
	solidrectangle(21, 23, 15 + MP, 41);
	outtextxy(0, 3, HPtext);
	outtextxy(0, 25, MPtext);
}
void Hero::skill()
{
	if (MP >= 50)
	{
		for (int i = 0; i < ep; i++)
		{
			enemies[i]->damaged();
			enemies[i]->damaged();
			enemies[i]->damaged();
		}
		MP -= 50;
		setfillcolor(WHITE);
		solidrectangle(0, 0, 640, 480);
		hero.draw();
		EndBatchDraw();
		Sleep(200);
		BeginBatchDraw();
	}
}
void InitImages()
{
	loadimage(&hero_rc, "res\\hero.jpg");
	SetWorkingImage(&hero_rc);
	for (int i = 0; i < 4; i++)
	{
		getimage(&hero_img[i][1], 40 * i, 0, 40, 40);
		getimage(&hero_img[i][0], 40 * i, 40, 40, 40);
	}
	for (int i = 5; i < 9; i++)
	{
		getimage(&hero_img[i][1], 40 * (i - 5), 80, 40, 40);
		getimage(&hero_img[i][0], 40 * (i - 5), 120, 40, 40);
	}
	getimage(&hero_img[4][1], 240, 160, 40, 40);
	getimage(&hero_img[4][0], 240, 200, 40, 40);
	getimage(&hero_img[9][1], 240, 240, 40, 40);
	getimage(&hero_img[9][0], 240, 280, 40, 40);

	SetWorkingImage(NULL);


	loadimage(&enemy_rc, "res\\enemy.jpg");			//获取图片
	SetWorkingImage(&enemy_rc);
	for (int i = 0; i < 5; i++)
	{
		getimage(&enemy_img[i][1], 40 * i, 0, 40, 40);
		getimage(&enemy_img[i][0], 40 * i, 40, 40, 40);
	}
	for (int i = 7; i < 12; i++)
	{
		getimage(&enemy_img[i][1], 40 * (i - 7), 80, 40, 40);
		getimage(&enemy_img[i][0], 40 * (i - 7), 120, 40, 40);
	}
	getimage(&enemy_img[5][1], 280, 160, 40, 40);
	getimage(&enemy_img[5][0], 280, 200, 40, 40);
	getimage(&enemy_img[6][1], 320, 160, 40, 40);
	getimage(&enemy_img[6][0], 320, 200, 40, 40);
	getimage(&enemy_img[12][1], 280, 240, 40, 40);
	getimage(&enemy_img[12][0], 280, 280, 40, 40);
	getimage(&enemy_img[13][1], 320, 240, 40, 40);
	getimage(&enemy_img[13][0], 320, 280, 40, 40);

	loadimage(&background, "res\\background.jpg");
	SetWorkingImage(NULL);
}
void Hero::reset()
{
	x = 320;
	y = 240;
	HP = fullHP;
	MP = 50;
	MP_recover_gap = 0;
	attack_gap = 0;
	direction = 0;
	acting_style = 0;
}
