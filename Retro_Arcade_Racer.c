#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include<mmsystem.h>


#pragma comment(lib, "winmm.lib")
#pragma warning(disable : 4996)


#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

#define MAXY 40
#define MAXX 60

//#define ROAD_SPACE (MAXX / 2 - 2);

#define runSound "C:\\Users\\user\\Desktop\\RaceGameSound\\y2mate.com-8-Bit-Fantasy-Racing-Music_AKQPbjzMJvw.wav"
#define breakSound // 브레이크 밟는 소리


int layer = 0;

int delaySpeed = 0;

int delayCount = 0;

int flat[MAXY][MAXX] = { 0, };
int obstacleFlat[MAXY][MAXX] = { 0, };
int roadForward[4][MAXY / 2][MAXX] = { 0, };


float max_x = 0, min_x = 0;
float max_y = 0, min_y = 0;

int mapx[10][10] = { 0, };
int mapy[10][10] = { 0, };

int LroadLR = 0;
int RroadLR = 0;

int LroadUD = 0;
int RroadUD = 0;

float x[8];
float y[8];

float obstacleLineX[4];
float obstacleLineY[4];

float midX[4];
float midY[4];

int LRspeed = 4;


/*
	1 == car(red)
	2 == car(d_red)
	3 == dark gray
	4 == window (gray)
	5 == tire (black)
	6 == number(white)
	7 == logo
	*/
int audi[16][16] = {
	//{0,0,0,0,0,0,0,0},
	//{0,0,0,0,0,0,0,0},
	//{0,0,0,0,0,0,0,0},
	//{0,0,0,0,0,0,0,0},
	//{1,1,2,2,2,2,1,1},
	//{3,3,2,2,2,2,3,3},
	//{3,3,2,6,6,2,3,3},
	//{5,5,0,0,0,0,5,5},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0},
	{0,0,2,2,4,4,4,4,4,4,4,4,2,2,0,0},
	{0,2,2,2,4,4,4,4,4,4,4,4,2,2,2,0},
	{2,1,1,1,2,2,7,7,7,7,2,2,1,1,1,2},
	{2,1,1,1,2,2,2,2,2,2,2,2,1,1,1,2},
	{2,3,3,3,2,2,6,6,6,6,2,2,3,3,3,2},
	{2,3,3,3,2,2,6,6,6,6,2,2,3,3,3,2},
	{5,5,5,5,0,0,0,0,0,0,0,0,5,5,5,5}
};

int cloud[6][10] = {
	{0,0,0,0,0,0,0,0,0,0}, //
	{0,0,1,1,1,0,0,0,0,0}, //  ■■■     
	{1,1,1,1,1,1,1,1,1,1}, //■■■■■■■■■■
	{1,1,1,1,1,1,1,1,1,1}, //■■■■■■■■■■
	{0,0,0,0,0,0,1,1,1,0}, //      ■■■ 
	{0,0,0,0,0,0,0,0,0,0}, //
};

enum colorName
{
	BLACK,
	D_BLUE,
	D_GREEN,
	D_SKYBLUE,
	D_RED,
	D_VIOLET,
	D_YELLOW,
	GRAY,
	D_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};

void color(int bgColor, int textColor);

int gotoxy(int x, int y);

int lining(float* px1, float* px2, float* py1, float* py2, float max_x, float min_x, float max_y, float min_y);

int liningOtc(float* px1, float* px2, float* py1, float* py2, float max_x, float min_x, float max_y, float min_y);

int print_map();

int background();

int flatClear();

int MAXorMIN(float x1, float x2, float  y1, float y2);

void CursorView(int show);

int moveControl();

int roadDownAnimation();

int fullScreen();

int playSound();

int car();

int main(void)
{
	/* 게임 시작전 설정 */
	{
		CursorView(0);
		system("mode con:lines=41  cols=121"); //콘솔창 세로, 가로 조정

		fullScreen();

		Sleep(100);

		roadDownAnimation();

		background();

		/* 도로 */
		{
			x[0] = (MAXX - 5) / 2; //도로 중앙의 좌측 
			y[0] = (MAXY) / 2;

			x[1] = (MAXX + 5) / 2; //도로 중앙의 우측
			y[1] = (MAXY) / 2;



			x[2] = (0); //도로 하단의 좌측 
			y[2] = (MAXY - 1);

			x[3] = (MAXX - 1); //도로 하단의 우측
			y[3] = (MAXY - 1);



			x[4] = (0); //도로 하단의 좌측 (보조)
			y[4] = (MAXY - 1);

			x[5] = (MAXX - 1); //도로 하단의 우측 (보조)
			y[5] = (MAXY - 1);

			//lineX[0] = x[1] - x[0] - 1;
			//lineY[0] = y[1];

			//lineX[1] = x[1] - x[0] - 1;
			//lineY[1] = y[1];

			//midX[0] = x[0] + 3;
			//midY[0] = y[0];
			//midX[1] = x[1] - 3;
			//midY[1] = y[1];

			//midX[2] = (MAXX / 3) * 1 + 3 - 1;
			//midY[2] = MAXY - 1;
			//midX[3] = (MAXX / 3) * 2 - 3;
			//midY[3] = MAXY - 1;
		}


		/* 장애물 라인 */
		{
			//최상단 장애물 라인 flat에서 Y축 중앙, //obsatcleMap에서 Y축 0
			obstacleLineX[0] = (MAXX / 2) - 2;
			obstacleLineY[0] = MAXY / 2;
			obstacleLineX[1] = (MAXX / 2) + 1;
			obstacleLineY[1] = MAXY / 2;

			//최하단 장애물 라인 flat에서 Y축 최하단, //obstacleMap에서 Y축 최하단
			obstacleLineX[2] = ((MAXX) / 4) * 1;
			obstacleLineY[2] = MAXY - 1;
			obstacleLineX[3] = ((MAXX) / 4) * 3 - 1;
			obstacleLineY[3] = MAXY - 1;
		}


		//pre_x = x[2];
		//pre_y = y[2];

		//playSound();
		car();
		CursorView(0);
	}

	/* 게임 시작후 설정 */
	{
		while (TRUE)
		{
			color(BLACK, WHITE);
			moveControl();

			/* 맵 도로 생성 */
			/*if (x[2] != pre_x || y[2] != pre_y)*/
			{
				//flatClear();
				/* 맵 중앙 도로줄 선잇기 */
				MAXorMIN(x[0], x[1], y[0], y[1]);
				lining(&x[0], &x[1], &y[0], &y[1], max_x, min_x, max_y, min_y);


				/* 보조선 연결 */
				MAXorMIN(x[2], x[4], y[2], y[4]);
				lining(&x[2], &x[4], &y[2], &y[4], max_x, min_x, max_y, min_y);

				MAXorMIN(x[4], x[5], y[4], y[5]);
				lining(&x[4], &x[5], &y[4], &y[5], max_x, min_x, max_y, min_y);

				MAXorMIN(x[5], x[3], y[5], y[3]);
				lining(&x[5], &x[3], &y[5], &y[3], max_x, min_x, max_y, min_y);


				/*맵 중앙 도로줄 꼭짓점과 하단 도로줄 꼭짓점 연결*/

				/*좌측*/
				MAXorMIN(x[0], x[2], y[0], y[2]);
				lining(&x[0], &x[2], &y[0], &y[2], max_x, min_x, max_y, min_y);
				/*우측*/
				MAXorMIN(x[1], x[3], y[1], y[3]);
				//copyMap();
				lining(&x[1], &x[3], &y[1], &y[3], max_x, min_x, max_y, min_y);









				//pre_x = x[2];
				//pre_y = y[2];


				//MAXorMIN(midX[0], midX[1], midY[0], midY[1]);
				//lining(&midX[0], &midX[1], &midY[0], &midY[1], max_x, min_x, max_y, min_y);
				//MAXorMIN(midX[1], midX[3], midY[1], midY[3]);
				//lining(&midX[1], &midX[3], &midY[1], &midY[3], max_x, min_x, max_y, min_y);
				//MAXorMIN(midX[3], midX[2], midY[3], midY[2]);
				//lining(&midX[3], &midX[2], &midY[3], &midY[2], max_x, min_x, max_y, min_y);
				//MAXorMIN(midX[2], midX[0], midY[2], midY[0]);
				//lining(&midX[2], &midX[0], &midY[2], &midY[0], max_x, min_x, max_y, min_y);
			}


			/* 장애물 생성라인 */
			{
				MAXorMIN(obstacleLineX[0], obstacleLineX[2], obstacleLineY[0], obstacleLineY[2]);
				liningOtc(&obstacleLineX[0], &obstacleLineX[2], &obstacleLineY[0], &obstacleLineY[2], max_x, min_x, max_y, min_y);

				MAXorMIN(obstacleLineX[1], obstacleLineX[3], obstacleLineY[1], obstacleLineY[3]);
				liningOtc(&obstacleLineX[1], &obstacleLineX[3], &obstacleLineY[1], &obstacleLineY[3], max_x, min_x, max_y, min_y);

			}

			print_map();

			//flatClear(); /*   <==   print_map(); 하단에 이동됨. */

			if (delayCount >= delaySpeed)
			{
				if (layer != 3) // 애니메이션 장 수
					layer++;
				else if (layer == 3) // 마지막 애니메이션 장
					layer = 0;
				delayCount = 0;
			}
			else
				delayCount++;
		}
	}

}

void color(int bgColor, int textColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bgColor * 16 + textColor);
}

int gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int lining(float* px1, float* px2, float* py1, float* py2, float max_x, float min_x, float max_y, float min_y) // x1,y1의 좌표와 x2,y2의 좌표를 선으로 이음
{
	float x, y, a;
	if (*px2 - *px1 >= *py2 - *py1)
	{
		for (int i = min_x; i <= max_x; i++)
		{
			if ((*py2 - *py1) < (*px2 - *px1)) {
				a = ((*py2 - *py1) / (*px2 - *px1));
				if ((*px2 - *px1) == 0)
					a = 0;
				y = a * (i - *px1) + 0.5 + *py1;
				flat[(int)y][i] = 1;
			}
			else {
				a = ((*py1 - *py2) / (*px1 - *px2));
				if ((*px1 - *px2) == 0)
					a = 0;
				y = a * (i - *px2) + 0.5 + *py2;
				flat[(int)y][i] = 1;
			}
		}
		for (int i = min_y; i <= max_y; i++)
		{
			if ((*py2 - *py1) > (*px2 - *px1)) {
				a = ((*px2 - *px1) / (*py2 - *py1));
				if ((*py2 - *py1) == 0)
					a = 0;
				x = a * (i - *py1) + 0.5 + *px1;
				flat[i][(int)x] = 1;
			}
			else {
				a = ((*px1 - *px2) / (*py1 - *py2));
				if ((*py1 - *py2) == 0)
					a = 0;
				x = a * (i - *py2) + 0.5 + *px2;
				flat[i][(int)x] = 1;
			}
		}
	}

	else if (*py2 - *py1 > * px2 - *px1)
	{
		for (int i = min_x; i <= max_x; i++)
		{
			if ((*py2 - *py1) < (*px2 - *px1)) {
				a = ((*py2 - *py1) / (*px2 - *px1));
				if ((*px2 - *py1) == 0)
					a = 0;
				y = a * (i - *px1) + 0.5 + *py1;
				flat[(int)y][i] = 1;
			}
			else {
				a = ((*py1 - *py2) / (*px1 - *px2));
				if ((*px1 - *px2) == 0)
					a = 0;
				y = a * (i - *px2) + 0.5 + *py2;
				flat[(int)y][i] = 1;
			}
		}
		for (int i = min_y; i <= max_y; i++)
		{
			if ((*py2 - *py1) > (*px2 - *px1)) {
				a = ((*px2 - *px1) / (*py2 - *py1));
				if ((*py2 - *py1) == 0)
					a = 0;
				x = a * (i - *py1) + 0.5 + *px1;
				flat[i][(int)x] = 1;
			}
			else {
				a = ((*px1 - *px2) / (*py1 - *py2));
				if ((*py1 - *py2) == 0)
					a = 0;
				x = a * (i - *py2) + 0.5 + *px2;
				flat[i][(int)x] = 1;
			}
		}
	}
}

int liningOtc(float* px1, float* px2, float* py1, float* py2, float max_x, float min_x, float max_y, float min_y) // x1,y1의 좌표와 x2,y2의 좌표를 선으로 이음
{
	float x, y, a;
	if (*px2 - *px1 >= *py2 - *py1)
	{
		for (int i = min_x; i <= max_x; i++)
		{
			if ((*py2 - *py1) < (*px2 - *px1)) {
				a = ((*py2 - *py1) / (*px2 - *px1));
				if ((*px2 - *px1) == 0)
					a = 0;
				y = a * (i - *px1) + 0.5 + *py1;
				obstacleFlat[(int)y][i] = 2;
			}
			else {
				a = ((*py1 - *py2) / (*px1 - *px2));
				if ((*px1 - *px2) == 0)
					a = 0;
				y = a * (i - *px2) + 0.5 + *py2;
				obstacleFlat[(int)y][i] = 2;
			}
		}
		for (int i = min_y; i <= max_y; i++)
		{
			if ((*py2 - *py1) > (*px2 - *px1)) {
				a = ((*px2 - *px1) / (*py2 - *py1));
				if ((*py2 - *py1) == 0)
					a = 0;
				x = a * (i - *py1) + 0.5 + *px1;
				obstacleFlat[i][(int)x] = 2;
			}
			else {
				a = ((*px1 - *px2) / (*py1 - *py2));
				if ((*py1 - *py2) == 0)
					a = 0;
				x = a * (i - *py2) + 0.5 + *px2;
				obstacleFlat[i][(int)x] = 2;
			}
		}
	}

	else if (*py2 - *py1 > * px2 - *px1)
	{
		for (int i = min_x; i <= max_x; i++)
		{
			if ((*py2 - *py1) < (*px2 - *px1)) {
				a = ((*py2 - *py1) / (*px2 - *px1));
				if ((*px2 - *py1) == 0)
					a = 0;
				y = a * (i - *px1) + 0.5 + *py1;
				obstacleFlat[(int)y][i] = 2;
			}
			else {
				a = ((*py1 - *py2) / (*px1 - *px2));
				if ((*px1 - *px2) == 0)
					a = 0;
				y = a * (i - *px2) + 0.5 + *py2;
				obstacleFlat[(int)y][i] = 2;
			}
		}
		for (int i = min_y; i <= max_y; i++)
		{
			if ((*py2 - *py1) > (*px2 - *px1)) {
				a = ((*px2 - *px1) / (*py2 - *py1));
				if ((*py2 - *py1) == 0)
					a = 0;
				x = a * (i - *py1) + 0.5 + *px1;
				obstacleFlat[i][(int)x] = 2;
			}
			else {
				a = ((*px1 - *px2) / (*py1 - *py2));
				if ((*py1 - *py2) == 0)
					a = 0;
				x = a * (i - *py2) + 0.5 + *px2;
				obstacleFlat[i][(int)x] = 2;
			}
		}
	}
}

int print_map() // FLAT 맵 출력
{
	for (int i = MAXY / 2; i < MAXY - 1; i++)
	{
		int isRoad = 0;
		int drawRoad = 0;
		int count = 0;
		for (int j = 0; j < MAXX; j++)
		{
			int check = 0;
			gotoxy(j * 2 + 20, i + 10);
			if ((MAXY / 2 + 7 <= i && i <= MAXY - 5) && (MAXX / 2 - 8 <= j && j <= MAXX / 2 + 7)) // 자동차 범위
			{
				if (audi[i - MAXY / 2 + 1][j - MAXX / 2 - 8] == 0)
					check = 1; // 자동차가 있으면 0, 없으면 1
			}
			if (!((MAXY / 2 + 7 <= i && i <= MAXY - 5) && (MAXX / 2 - 8 <= j && j <= MAXX / 2 + 7)) || check == 1)
			{
				if (flat[i][j] == 0)
				{
					if (isRoad == 1)
					{
						drawRoad = 1;
						count = 1;
						color(D_GRAY, D_GRAY);
						flat[i][j] = 1;
						if (roadForward[layer][i - MAXY / 2][j] == 0 && i >= MAXY / 2)
							printf("  ");
					}
					else
					{
						color(D_GREEN, D_GREEN);
						if (roadForward[layer][i - MAXY / 2][j] == 0 && i >= MAXY / 2)
							printf("  ");
					}
				}
				else if (flat[i][j] == 1)
				{
					if (j < MAXX - 1 && i > MAXY / 2 && flat[i][j + 1] == 1)
					{
						isRoad = 0;
					}
					else if (j < MAXX - 1 && i > MAXY / 2 && flat[i][j + 1] == 0)
					{
						if (count == 1)
							isRoad = 0;
						else
							isRoad = 1;
					}
					color(D_GRAY, D_GRAY);
					if (roadForward[layer][i - MAXY / 2][j] == 0 && i >= MAXY / 2)
						printf("  ");
				}
				if (roadForward[layer][i - MAXY / 2][j] == 1)
				{
					if (flat[i][j] == 1)
						color(GRAY, GRAY);
					if (flat[i][j] == 0)
						color(GREEN, GREEN);
					printf("  ");
				}
				//if (obstacleFlat[i][j] == 2)
				//{
				//	gotoxy(j * 2 + 20, i + 10);
				//	color(BLUE, BLUE);
				//	printf("  ");
				//}
			}
		}
	}
	printf("\n");
	flatClear();
}

int background()
{
	for (int i = 0; i < MAXY / 2; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			gotoxy(j * 2 + 20, i + 10);
			if ((10 <= i && i <= 15) && (25 <= j && j < 35))
			{
				if (cloud[i - 10][j - 25] == 1)
				{
					color(WHITE, WHITE);
				}
				else
				{
					color(SKYBLUE, WHITE);
				}
			}
			else if ((4 <= i && i <= 9) && (7 <= j && j < 17))
			{
				if (cloud[i - 4][j - 7] == 1)
				{
					color(WHITE, WHITE);
				}
				else
				{
					color(SKYBLUE, WHITE);
				}
			}
			else if ((6 <= i && i <= 11) && (45 <= j && j < 55))
			{
				if (cloud[i - 6][j - 45] == 1)
				{
					color(WHITE, WHITE);
				}
				else
				{
					color(SKYBLUE, WHITE);
				}
			}
			else
			{
				color(SKYBLUE, WHITE);
			}
			printf("  ");
		}
	}
	color(BLACK, WHITE);
}

int flatClear() // 클린업
{
	for (int i = 0; i < MAXY; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			flat[i][j] = 0;
			obstacleFlat[i][j] = 0;
		}
	}
}

int MAXorMIN(float x1, float x2, float  y1, float y2) // x1과 x2, y1과 y2의 위치 정리
{
	max_x = x2 >= x1 ?
		x2 : x1;
	min_x = x1 <= x2 ?
		x1 : x2;

	max_y = y2 >= y1 ?
		y2 : y1;
	min_y = y1 <= y2 ?
		y1 : y2;
}

void CursorView(int show) // 입력 커서 제거
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

int moveControl()
{
	if (kbhit())
	{
		int moveControl = getch();
		switch (moveControl)
		{
		case 'w':
			if (delaySpeed != 0)
				delaySpeed--;
			break;
		case 's':
			if (delaySpeed != 5)
				delaySpeed++;
			break;
		case 'd':
			if (y[2] > (MAXY / 2 + 3))
			{
				if (x[2] > 0)
				{
					x[2] -= LRspeed;
					x[4] -= LRspeed;

					if (y[3] == 0)
					{
						x[3] -= LRspeed;
					}
					else if (y[3] < MAXY)
					{
						y[3] += LRspeed;
					}
				}
				else if (x[2] <= 0)
				{
					x[2] = 0;
					x[3] -= LRspeed;
					x[5] -= LRspeed;
					y[2] -= LRspeed;
				}
			}
			break;
		case 'a':
			if (y[3] > (MAXY / 2 + 3))
			{
				if (x[3] < MAXX - 1)
				{
					x[3] += LRspeed;
					x[5] += LRspeed;

					if (y[2] == 0)
					{
						x[2] += LRspeed;
					}
					else if (y[2] < MAXY)
					{
						y[2] += LRspeed;
					}
				}
				else if (x[3] >= MAXX - 1)
				{
					x[3] = MAXX - 1;
					x[2] += LRspeed;
					x[4] += LRspeed;
					y[3] -= LRspeed;
				}
			}
			break;
		case ' ': //브레이크
			delayCount--;
			break;
		case 27:
			gotoxy(MAXX - 5 + 20, MAXY + 10);
			printf("ESC PRESSED");
			gotoxy(MAXX - 4 + 20, MAXY + 11);
			printf("GAME OVER");
			exit(1);
			break;
		}
	}
}

int roadDownAnimation()
{
	int roadMoveScene[4][MAXY / 2] = {
		{0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1},
		{1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1},
		{1,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
		{0,1,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0}
	};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < MAXY / 2; j++)
		{
			if (roadMoveScene[i][j] == 1)
			{
				for (int k = 0; k < MAXX; k++)
				{
					roadForward[i][j][k] = 1;
				}
			}
		}
	}
}

int fullScreen()
{
	//int width = GetSystemMetrics(SM_CXSCREEN);
	//int height = GetSystemMetrics(SM_CXSCREEN);

	//HWND hwnd = CreateWindow(szWindowClass, szTitle, WS_EX_TOPMOST | WS_POPUP,
	//	0, 0, width, height, NULL, NULL, hInstance, NULL);

	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}

int playSound()
{
	PlaySound(TEXT(runSound), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//사운드 파일 위치, SND_ASYNC, SND_LOOP 세가지가 제일 중요
	//SND_ASYNC : 재생하면서 다음코드 실행
	//SND_LOOP : 반복재생
	//while (1)
	//{
	//	int a;
	//	printf("종료?");
	//	scanf("%d", &a);
	//	if (a == 0)
	//	{
	//		PlaySound(NULL, 0, 0);
	//		Sleep(1000);
	//		printf("노래를 종료했습니다. 함수도 종료합니다.");
	//		return;
	//	}
	//}
}

int car()
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			gotoxy(j * 2 + 34 + (MAXX / 2), i + 10 + (MAXY / 2));
			switch (audi[i][j])
			{
			case 1:	//	1 == car(red)
				color(RED, RED);
				printf("  ");
				break;
			case 2: //	2 == car(d_red)    Back light
				color(D_RED, D_RED);
				printf("  ");
				break;
			case 3: //	3 == dark gray     Under of Back light
				color(D_GRAY, D_GRAY);
				printf("  ");
				break;
			case 4: // 	4 == window (gray)
				color(GRAY, BLACK);
				printf("〓");
				break;
			case 5: //	5 == tire (black)
				color(BLACK, BLACK);
				printf("  ");
				break;
			case 6: //	6 == number(white)
				color(WHITE, GRAY);
				if (6 <= j && j <= 9)
					printf("  ");
				else
					printf("  ");
			case 7: //	7 == logo
				color(D_RED, GRAY);
				printf("○");
				break;
			}
		}
	}
}
