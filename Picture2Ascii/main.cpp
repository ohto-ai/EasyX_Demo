/////////////////////////////////////////////////////
// 程序名称：图片转ASCII点阵demo
// 编译环境：Visual C++ 6.0，EasyX 2012-5-1测试版 
// 作　　者：Geodesic <Geodesicwl-cpro@yahoo.cn> 
// 最后修改：2012-5-16 
///////////////////////////////////////////////////// 
//若提示缺少graphics.h（Easyx）头文件，请到这里下载：
//www.easyx.cn
/////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

/***********************宏定义***********************/
#define ASC_HEIGHT 8
#define ASC_WIDTH 8
#define ASCII_NUM 32

/*********************结构体定义*********************/
struct ascii
{
	char asc[ASCII_NUM];
	int gray[ASCII_NUM];
}as = { ' ', '`', '.', '^', ',', ':', '~', '"', '<', '!', 'c', 't', '+', '{', 'i', '7', '?',
		'u', '3', '0', 'p', 'w', '4', 'A', '8', 'D', 'X', '%', '#', 'H', 'W', 'M',
		0, 5, 7, 9, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43,
		45, 47, 49, 51, 53, 55, 59, 61, 63, 66, 68, 70 };

/**********************函数声明***********************/
char SearchAsc(struct ascii as, int gray);
void Pic2Asc(struct ascii as, char filename[]);

/***********************主函数***********************/
int main(void)
{
	char filename[256];

	system("title, 图片转化为ASCII图画demo");

	printf("请输入文件名(含扩展名)\n\n");
	fgets(filename, sizeof(filename), stdin);
	filename[strlen(filename) - 1] = '\0';
	system("cls");
	Pic2Asc(as, filename);

	printf("转换完成，任意键退出\n\n");
	system("pause");

	return 0;
}

/***********************函数体***********************/

//图片转化为ASCII点阵，并保存到txt和html文件中
void Pic2Asc(struct ascii as, char filename[])
{
	int height, width, gray;
	IMAGE img;
	FILE* fpAscPicText;

	//图像设备初始化
	loadimage(&img, filename);
	SetWorkingImage(&img);

	height = getheight() / ASC_HEIGHT;
	width = getwidth() / ASC_WIDTH;

	if ((fpAscPicText = fopen("ASC_PIC.txt", "w")) == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}

	//开始转化并保存结果
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			gray = 0;

			//扫描每一小块的灰度，并计算出平均灰度
			for (int h = 0; h < ASC_HEIGHT; h++)
				for (int w = 0; w < ASC_WIDTH; w++)
				{
					gray += GetRValue(RGBtoGRAY(getpixel(j * ASC_WIDTH + w, i * ASC_HEIGHT + h)));
				}
			gray /= (ASC_HEIGHT * ASC_WIDTH);
			gray = (255 - gray) * 90 / 255;

			fputc(SearchAsc(as, gray), fpAscPicText);
		}
		fputc('\n', fpAscPicText);
	}
	fclose(fpAscPicText);
}

//二分法查找ASCII字符，就低不就高
char SearchAsc(struct ascii as, int gray)
{
	int lower = 0;
	int higher = ASCII_NUM;
	int mid;

	if (gray <= as.gray[0])
	{
		return as.asc[0];
	}
	else if (gray >= as.gray[ASCII_NUM - 1])
	{
		return as.asc[ASCII_NUM - 1];
	}
	else
	{
		while ((higher - lower) > 1)
		{
			mid = (lower + higher) >> 1;
			if (gray > as.gray[mid])
			{
				lower = mid;
			}
			else
			{
				higher = mid;
			}
		}
		return as.asc[lower];
	}
}