/////////////////////////////////////////////////////////
// 程序名称：种地要浇水II（创意源自《金庸无双》）
// 编译环境：Visual C++ 6.0 / 2012，EasyX 20130610(beta)
// 作    者：i92076 <http://www.baidu.com/p/i92076>
// 最后修改：2013-9-3
//
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string>
using namespace std;

#define PI 3.1415926535
int coin = 10;			// 初始10枚金币（赢赚5，输或重置扣2，旋跳扣3，金币不足不允许重置旋跳，仅允许输且金币归零）
int row = 0, col = 0;	// 当前选中的田地块


int my_rand()	// 产生1到11的随机数
{
	return (rand() % 11) + 1;
}


//11种随机数对应的11种情况：
//		1╋    2┫    3┣   4┻     5┳      6┏      7┗      8┓      9┛     10━     11┃
//0空，1、2， 3、4， 5、6， 7、8， 9、10， 11、12， 13、14， 15、16， 17、18， 19、20， 21、22
//上一行每种类型前者为有水（奇数），后者为干涸（偶数），是对11种情况的细化
bool tongchang[11][4] = {
	//上下左右通畅情况(针对以上11种情况的田地块)
	  1,1,1,1,/* ╋ */		1,1,1,0,/* ┫ */

	  1,1,0,1,/* ┣ */		1,0,1,1,/* ┻ */

	  0,1,1,1,/* ┳ */		0,1,0,1,/* ┏ */

	  1,0,0,1,/* ┗ */		0,1,1,0,/* ┓ */

	  1,0,1,0,/* ┛ */		0,0,1,1,/* ━ */

	  1,1,0,0/* ┃ */
};


int map[6][6] = { 0 };//0到22,保存6*6的田地信息


/*******************23种情况对应图片********************/
IMAGE img[23];
//加载图片
void load_field_img()
{
	IMAGE field_img;//用于加载竖条图
	loadimage(&field_img, ".\\img\\img1.jpg", 0, 0, false);
	SetWorkingImage(&field_img);
	int height = field_img.getheight() / 9;
	int width = field_img.getwidth();
	getimage(&img[0], 0, 0 * height, width, height);	// 空地
	getimage(&img[22], 0, 1 * height, width, height);	// 干涸┃
	getimage(&img[16], 0, 2 * height, width, height);	// 干涸┓
	getimage(&img[4], 0, 3 * height, width, height);	// 干涸┫
	getimage(&img[1], 0, 4 * height, width, height);	// 有水╋
	getimage(&img[17], 0, 5 * height, width, height);	// 有水┛
	getimage(&img[21], 0, 6 * height, width, height);	// 有水┃
	getimage(&img[7], 0, 7 * height, width, height);	// 有水┻
	getimage(&img[2], 0, 8 * height, width, height);	// 干涸╋
	SetWorkingImage();
	//以上9种靠直接加载图片分块，其余14种靠逆时针旋转前9种获取
	rotateimage(&img[3], &img[7], PI / 2);		// 有水┫
	rotateimage(&img[5], &img[7], 3 * PI / 2);	// 有水┣
	rotateimage(&img[6], &img[4], PI);			// 干涸┣
	rotateimage(&img[8], &img[4], 3 * PI / 2);	// 干涸┻
	rotateimage(&img[9], &img[7], PI);			// 有水┳
	rotateimage(&img[10], &img[4], PI / 2);		// 干涸┳
	rotateimage(&img[11], &img[17], PI);			// 有水┏
	rotateimage(&img[12], &img[16], PI / 2);		// 干涸┏
	rotateimage(&img[13], &img[17], 3 * PI / 2);		// 有水┗
	rotateimage(&img[14], &img[16], PI);			// 干涸┗
	rotateimage(&img[15], &img[17], PI / 2);		// 有水┓
	rotateimage(&img[18], &img[16], 3 * PI / 2);		// 干涸┛
	rotateimage(&img[19], &img[21], PI / 2);		// 有水━
	rotateimage(&img[20], &img[22], PI / 2);		// 干涸━
}


/**************水渠缓冲区（随机产生）************************/
int sq_buff[4];//水渠缓冲区（数组队列,数组尾先入先出，存11种随机数）


void init_sq_buff()//初始水渠缓冲区
{
	for (int i = 3; i >= 0; i--)
	{
		sq_buff[i] = my_rand();
	}
}


int get_sq_buff()//变化水渠缓冲区
{
	int result = sq_buff[3];
	for (int i = 2; i >= 0; i--)
	{
		sq_buff[i + 1] = sq_buff[i];
	}
	sq_buff[0] = my_rand();
	return result;
}


void print_sq_buff()//显示水渠缓冲区
{//左上31,35;31,99;31,162;31,233;
	putimage(31, 35, &img[sq_buff[0] * 2]);
	putimage(31, 99, &img[sq_buff[1] * 2]);
	putimage(31, 162, &img[sq_buff[2] * 2]);
	putimage(31, 233, &img[sq_buff[3] * 2]);
}


/*******************  11种情况旋转映射(先逆后顺)****************************/
int rotated_list[][2] = {
	-1,-1,//无意义占位
	1,1,		5,4,
	4,5,		2,3,
	3,2,		7,8,
	9,6,		6,9,
	8,7,		11,11,
	10,10
};


/*******************************************************/
//水渠通水情况的递归判断，之前务必先让所有水渠干涸（即map的所有为奇数的元素加1）
void judge(int i, int j, int direction)//参数为map的行和列、以及水源自哪个方向而来：0上1下2左3右
{
	if (i < 0 || j < 0 || map[i][j] == 0 || map[i][j] % 2 == 1)
	{
		//递归结束条件：越界、无渠、已经有水
		return;
	}
	int type = (map[i][j] + 1) / 2;//得到1到11的数字，对应类型如上
	if (tongchang[type - 1][direction])//本块与水源相通
	{
		map[i][j] -= 1;//本块注水
		//依次向本块的其它三个方向递归
		for (int n = 0; n < 4; n++)
		{
			if (n == direction)
			{
				continue;//跳过水源方向
			}
			if (tongchang[type - 1][n] == 1)//其它三个方向的某一个通畅
			{
				if (n == 0)//上通畅
				{
					judge(i - 1, j, 1);//下一个递归块水源自下
				}
				else if (n == 1)//下通畅
				{
					judge(i + 1, j, 0);//下一个递归块水源自上
				}
				else if (n == 2)//左通畅
				{
					judge(i, j - 1, 3);//下一个递归块水源自右
				}
				else if (n == 3)//右通畅
				{
					judge(i, j + 1, 2);//下一个递归块水源自左
				}
			}
		}
	}
	else
	{
		return;
	}
}


//总的调用以上
void check_water()
{
	//先让所有水渠干涸
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (map[i][j] % 2 == 1)//该块有水
				map[i][j] += 1;
		}
	}
	//再递归判断
	judge(0, 3, 0);
}


/***************输出金币数*********************/
//数字转字符串：智能判断位数
//计算a的b次方
int pw(int a, int b)
{
	int result = 1;
	for (int i = 0; i < b; i++)
		result *= a;
	return result;
}


void int_str(char str[11], int num, int jinzhi)
{
	if (num == 0) { str[0] = '0'; str[1] = '\0'; return; }
	int i = 0, temp;
	while (num >= pw(jinzhi, i))i++;
	//此时需分配i+1位（含'\0'）长度字符串
	str[10] = '\0';
	for (int n = i; n < 10; n++)
	{
		str[n] = ' ';
	}
	for (int k = i - 1; k >= 0; k--)
	{
		temp = num % jinzhi;//当前最后一位
		num /= jinzhi;
		if (temp >= 10 && temp <= 15)temp += ('A' - 10);
		else if (temp >= 0 && temp <= 9)temp += '0';
		else//异常处理
		{
			str[0] = '\0';
			return;
		}
		str[k] = (char)temp;
	}
}


void print_coin()
{
	char coin_str[11];
	int_str(coin_str, coin, 10);
	setcolor(BLACK);
	outtextxy(552, 32, coin_str);
}


/**********************整体重绘田地********************/
void draw_field(int i, int j)//准备工作一：i、j都是0到5的数，对应某块田地，将其画出
{
	putimage(169 + j * 54, 98 + i * 54, &img[map[i][j]]);
}


void draw_frame()//准备工作二：画黄色的选中框
{
	setcolor(YELLOW);
	int a = col * 54, b = row * 54;
	rectangle(170 + a, 99 + b, 220 + a, 149 + b);
	rectangle(171 + a, 100 + b, 219 + a, 148 + b);
	rectangle(172 + a, 101 + b, 218 + a, 147 + b);
}


void refresh_all()//绘制所有田地块
{
	//背景色0xCACEB7
	//缓冲区左上31,35;31,99;31,162;31,233;
	//田地左上168,97;左上右222,97;左上下168,151;右下493,422
	print_sq_buff();
	check_water();
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			draw_field(i, j);
		}
	}
	draw_frame();
}


/******************胜利判断********************/
void check_win()
{
	int water_num = 0;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (map[i][j] == 0)
			{
				return;//还没填满呢
			}
			if (map[i][j] % 2 == 1)
			{
				water_num++;
			}
		}
	}
	if (water_num == 36)
	{
		string temp = "完成奖励：5金币\n";
		int add = 5;
		if (my_rand() % 3 == 0)
		{
			temp += "人品追加：2金币\n";
			add += 2;
		}
		if (row == 0 && col == 3)
		{
			temp += "达成成就【瞬息贯通】，追加10金币\n";
			add += 10;
		}
		int n;
		bool feishui = true;//【肥水不流外人田】成就判定
		for (n = 0; n < 6; n++)
		{
			if (tongchang[map[0][n] / 2][0] && n != 3)//上有出口，判定失败
			{
				feishui = false;
				break;
			}
			else if (tongchang[map[n][0] / 2][2])//左有出口，判定失败
			{
				feishui = false;
				break;
			}
			else if (tongchang[map[5][n] / 2][1])//下有出口，判定失败
			{
				feishui = false;
				break;
			}
			else if (tongchang[map[n][5] / 2][3])//右有出口，判定失败
			{
				feishui = false;
				break;
			}
		}
		if (feishui)
		{
			temp += "达成成就【肥水不流外人田】，追加15金币\n";
			add += 15;
		}
		coin += add;
		char str[11];
		int_str(str, add, 10);
		str[3] = '\0';
		temp += "总计：";
		temp += str;
		temp += "金币";
		MessageBox(NULL, temp.c_str(), "你很有想法", MB_SYSTEMMODAL);
	}
	else
	{
		coin -= 2;
		if (coin <= 0)
		{
			coin = 0;
			MessageBox(NULL, "任务失败，钱扣至0后不会再扣", "请允悲", MB_SYSTEMMODAL);
		}
		else
		{
			MessageBox(NULL, "任务失败，失去金币2", "请允悲", MB_SYSTEMMODAL);
		}
	}
	//后续工作
	init_sq_buff();
	memset(map, 0, sizeof(int) * 36);
	row = 0, col = 0;
	print_coin();
	print_sq_buff();
	refresh_all();
	FlushBatchDraw();
}


int main()
{
	srand((unsigned)time(NULL));
	init_sq_buff();
	char c;
	// 绘图环境初始化
	initgraph(640, 480);
	load_field_img();
	setbkcolor(0xcaceb7);
	BeginBatchDraw();
	// 加载界面
	IMAGE img_main;
	loadimage(&img_main, _T(".\\img\\main.jpg"));
	putimage(0, 0, &img_main);
	//其他初始工作
	print_coin();
	init_sq_buff();
	print_sq_buff();
	refresh_all();
	FlushBatchDraw();

	// 扫描按键
	while (1)
	{//方向键-32and（72，80，75，77）上下左右；空格32；
		c = _getch();
		if (c == -32)
		{
			c = _getch();
			if (c == 72)//↑
			{
				if (row == 0)//不能再向上
					continue;
				else
				{
					row--;
					refresh_all();
					FlushBatchDraw();
				}
			}
			else if (c == 80)//↓
			{
				if (row == 5)//不能再向下
					continue;
				else
				{
					row++;
					refresh_all();
					FlushBatchDraw();
				}
			}
			else if (c == 75)//←
			{
				if (col == 0)//不能再向左
					continue;
				else
				{
					col--;
					refresh_all();
					FlushBatchDraw();
				}
			}
			else if (c == 77)//→
			{
				if (col == 5)//不能再向右
					continue;
				else
				{
					col++;
					refresh_all();
					FlushBatchDraw();
				}
			}
		}
		else if (c == 32)//空格
		{
			if (map[row][col] == 0)
			{
				map[row][col] = get_sq_buff() * 2;
				print_sq_buff();
				check_water();
				refresh_all();
				FlushBatchDraw();
				check_win();
			}
			else
			{
				MessageBox(NULL, "该区域已放置水渠，不可覆盖", "囧", MB_SYSTEMMODAL);
			}
		}
		else if (c == 'R' || c == 'r')//重置
		{
			if (coin > 1)
			{
				coin -= 2;
				init_sq_buff();
				memset(map, 0, sizeof(int) * 36);
				row = 0, col = 0;
				print_coin();
				print_sq_buff();
				refresh_all();
				FlushBatchDraw();
			}
			else
			{
				MessageBox(NULL, "金币不足2，不能使用该特技", "囧", MB_SYSTEMMODAL);
			}
		}
		else if (c == 'S' || c == 's')//顺时针
		{
			if (coin > 2)
			{
				sq_buff[3] = rotated_list[sq_buff[3]][1];
				putimage(31, 233, &img[sq_buff[3] * 2]);
				coin -= 3;
				print_coin();
				FlushBatchDraw();
			}
			else
			{
				MessageBox(NULL, "金币不足3，不能使用该特技", "囧", MB_SYSTEMMODAL);
			}
		}
		else if (c == 'N' || c == 'n')//逆时针
		{
			if (coin > 2)
			{
				sq_buff[3] = rotated_list[sq_buff[3]][0];
				putimage(31, 233, &img[sq_buff[3] * 2]);
				coin -= 3;
				print_coin();
				FlushBatchDraw();
			}
			else
			{
				MessageBox(NULL, "金币不足3，不能使用该特技", "囧", MB_SYSTEMMODAL);
			}
		}
		else if (c == 'T' || c == 't')//跳过一个
		{
			if (coin > 2)
			{
				get_sq_buff();
				print_sq_buff();
				coin -= 3;
				print_coin();
				FlushBatchDraw();
			}
			else
			{
				MessageBox(NULL, "金币不足3，不能使用该特技", "囧", MB_SYSTEMMODAL);
			}
		}
		else if (c == 'H' || c == 'h')//帮助
		{
			MessageBox(NULL, "赢赚5（人品好追加2），输或重置扣2，其他特技扣3；\n金币不足不允许使用特技，输到金币0以后不另扣钱。\n特殊成就：\n【瞬息贯通】：最后放置入水口水渠，追加10金币；\n【肥水不流外人田】：水渠封闭，追加15金币。", "帮助", MB_SYSTEMMODAL | MB_ICONINFORMATION);
		}
		else if (c == 27)//esc
		{
			break;
		}
	}
	EndBatchDraw();
	closegraph();
	return 0;
}