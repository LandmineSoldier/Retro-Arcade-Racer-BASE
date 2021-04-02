#pragma warning(disable : 4996)
#include <stdio.h>
#include <Windows.h>
#include <WinUser.h> //필터키 삭제용으로 추가 했으나 변경방법 의문
#include <conio.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

//#define UP 72
//#define LEFT 75
//#define RIGHT 77
//#define DOWN 80

#define MAXY 40
#define MAXX 60
#define SCREENSTARTX 20
#define SCREENSTARTY 10

#define BLANK_IMAGE -1
#define NORMAL_IMAGE_SIZE 16

#define runSound "./Sound\\8-Bit-Fantasy-Racing-Music.wav"

int roadAniFrame = 0;
int roadAniDelaySpeed = 0;
int roadAniDelayCount = 0;

int flat[MAXY][MAXX] = { 0, }; //땅
int road[4][MAXY / 2][MAXX] = { 0, }; //도로

float max_x = 0, min_x = 0;
float max_y = 0, min_y = 0;

float x[8];
float y[8];

int LRspeed = 4;

char playerCar[NORMAL_IMAGE_SIZE][NORMAL_IMAGE_SIZE] = {
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1, 4, 4, 4, 4, 4, 4, 4, 4,-1,-1,-1,-1},
	{-1,-1, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4,-1,-1},
	{-1, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 4,-1},
	{ 4,12,12,12, 4, 4, 4, 4, 4, 4, 4, 4,12,12,12, 4},
	{ 4,12,12,12, 4, 4, 4, 4, 4, 4, 4, 4,12,12,12, 4},
	{ 4, 8, 8, 8, 4, 4,15,15,15,15, 4, 4, 8, 8, 8, 4},
	{ 4, 8, 8, 8, 4, 4,15,15,15,15, 4, 4, 8, 8, 8, 4},
	{ 0, 0, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 0}
};

int cloud[6][SCREENSTARTY] = {
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, //
	{-1,-1,15,15,15,-1,-1,-1,-1,-1}, //  ■■■     
	{15,15,15,15,15,15,15,15,15,15}, //■■■■■■■■■■
	{15,15,15,15,15,15,15,15,15,15}, //■■■■■■■■■■
	{-1,-1,-1,-1,-1,-1,15,15,15,-1}, //      ■■■ 
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}  //
};

enum colorName
{
	BLACK, D_BLUE, D_GREEN, D_SKYBLUE,	//	0,	1,	2,  3
	D_RED, D_VIOLET, D_YELLOW, GRAY,	//	4,	5,	6,  7
	D_GRAY, BLUE, GREEN, SKYBLUE,		//	8,	9, 10, 11
	RED, VIOLET, YELLOW, WHITE			// 12, 13, 14, 15
};

void color(int bgColor, int textColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bgColor * 16 + textColor);
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void lining(float x1, float x2, float y1, float y2, float max_x, float min_x, float max_y, float min_y) // x1,y1의 좌표와 x2,y2의 좌표를 선으로 이음
{
	float x, y, a;
	if (x2 - x1 >= y2 - y1)
	{
		for (int i = (int)min_x; i <= (int)max_x; i++)
		{
			if ((y2 - y1) < (x2 - x1)) {
				a = ((y2 - y1) / (x2 - x1));
				if ((x2 - x1) == 0)
					a = 0;
				y = a * (i - x1) + 0.5 + y1;
				if ((0 <= (int)y && (int)y < MAXY) && (0 <= i && i < MAXX))
					flat[(int)y][i] = 1;
			}
			else {
				a = ((y1 - y2) / (x1 - x2));
				if ((x1 - x2) == 0)
					a = 0;
				y = a * (i - x2) + 0.5 + y2;
				if ((0 <= (int)y && (int)y < MAXY) && (0 <= i && i < MAXX))
					flat[(int)y][i] = 1;
			}
		}
		for (int i = (int)min_y; i <= (int)max_y; i++)
		{
			if ((y2 - y1) > (x2 - x1)) {
				a = ((x2 - x1) / (y2 - y1));
				if ((y2 - y1) == 0)
					a = 0;
				x = a * (i - y1) + 0.5 + x1;
				if ((0 <= i && i < MAXY) && (0 <= (int)x && (int)x < MAXX))
					flat[i][(int)x] = 1;
			}
			else {
				a = ((x1 - x2) / (y1 - y2));
				if ((y1 - y2) == 0)
					a = 0;
				x = a * (i - y2) + 0.5 + x2;
				if ((0 <= i && i < MAXY) && (0 <= (int)x && (int)x < MAXX))
					flat[i][(int)x] = 1;
			}
		}
	}

	else if (y2 - y1 > x2 - x1)
	{
		for (int i = (int)min_x; i <= (int)max_x; i++)
		{
			if ((y2 - y1) < (x2 - x1)) {
				a = ((y2 - y1) / (x2 - x1));
				if ((x2 - y1) == 0)
					a = 0;
				y = a * (i - x1) + 0.5 + y1;
				if ((0 <= (int)y && (int)y < MAXY) && (0 <= i && i < MAXX))
					flat[(int)y][i] = 1;
			}
			else {
				a = ((y1 - y2) / (x1 - x2));
				if ((x1 - x2) == 0)
					a = 0;
				y = a * (i - x2) + 0.5 + y2;
				if ((0 <= (int)y && (int)y < MAXY) && (0 <= i && i < MAXX))
					flat[(int)y][i] = 1;
			}
		}
		for (int i = (int)min_y; i <= (int)max_y; i++)
		{
			if ((y2 - y1) > (x2 - x1)) {
				a = ((x2 - x1) / (y2 - y1));
				if ((y2 - y1) == 0)
					a = 0;
				x = a * (i - y1) + 0.5 + x1;
				if ((0 <= i && i < MAXY) && (0 <= (int)x && (int)x < MAXX))
					flat[i][(int)x] = 1;
			}
			else {
				a = ((x1 - x2) / (y1 - y2));
				if ((y1 - y2) == 0)
					a = 0;
				x = a * (i - y2) + 0.5 + x2;
				if ((0 <= i && i < MAXY) && (0 <= (int)x && (int)x < MAXX))
					flat[i][(int)x] = 1;
			}
		}
	}
}

void printMap() // FLAT 맵 출력
{
	for (int i = MAXY / 2; i < MAXY - 1; i++)
	{
		char isRoad = 0;
		char drawRoad = 0;
		int drawRoadStarted = 0;
		for (int j = 0; j < MAXX; j++)
		{
			int blankCheck = 0;
			gotoxy(j * 2 + SCREENSTARTX, i + SCREENSTARTY);
			if ((MAXY / 2 + 7 <= i && i <= MAXY - 5) && (MAXX / 2 - 8 <= j && j <= MAXX / 2 + 7)) // 자동차 범위
			{
				if (playerCar[i - MAXY / 2 + 1][j - MAXX / 2 - 8] == BLANK_IMAGE)
					blankCheck = 1; // 현재 출력 위치가 플레이어 이미지의 색상있는 곳에 있으면 0, 없으면 1
			}
			if (!((MAXY / 2 + 7 <= i && i <= MAXY - 5) && (MAXX / 2 - 8 <= j && j <= MAXX / 2 + 7)) || blankCheck == 1)
			{
				if (flat[i][j] == 0)
				{
					if (isRoad == 1)
					{
						drawRoad = 1;
						drawRoadStarted = 1;
						color(D_GRAY, D_GRAY);
						flat[i][j] = 1;
						if (road[roadAniFrame][i - MAXY / 2][j] == 0 && i >= MAXY / 2)
							puts("  ");
					}
					else
					{
						color(D_GREEN, D_GREEN);
						if (road[roadAniFrame][i - MAXY / 2][j] == 0 && i >= MAXY / 2)
							puts("  ");
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
						if (drawRoadStarted == 1)
							isRoad = 0;
						else
							isRoad = 1;
					}
					color(D_GRAY, D_GRAY);
					if (road[roadAniFrame][i - MAXY / 2][j] == 0 && i >= MAXY / 2)
						puts("  ");
				}
				if (road[roadAniFrame][i - MAXY / 2][j] == 1)
				{
					if (flat[i][j] == 1)
						color(GRAY, GRAY);
					if (flat[i][j] == 0)
						color(GREEN, GREEN);
					puts("  ");
				}
			}
		}
	}
	puts("\n");
	flatClear();
}

void background()
{
	for (int i = 0; i < MAXY / 2; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			gotoxy(j * 2 + SCREENSTARTX, i + SCREENSTARTY);
			putColor(SKYBLUE);
		}
	}
	for (int i = 0; i < MAXY / 2; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			gotoxy(j * 2 + SCREENSTARTX, i + SCREENSTARTY);
			if ((SCREENSTARTY <= i && i <= 15) && (25 <= j && j < 35))
			{
				putColor(cloud[i - SCREENSTARTY][j - 25]);
			}
			else if ((4 <= i && i <= 9) && (7 <= j && j < 17))
			{
				putColor(cloud[i - 4][j - 7]);
			}
			else if ((6 <= i && i <= 11) && (45 <= j && j < 55))
			{
				putColor(cloud[i - 6][j - 45]);
			}
		}
	}
	color(BLACK, WHITE);
}

int flatClear() // 클린업
{
	int blankFlat[MAXY][MAXX] = { 0, };
	memcpy(flat, blankFlat, sizeof(blankFlat));
}

void MAXorMIN(float x1, float x2, float  y1, float y2) // x1과 x2, y1과 y2의 위치 정리
{
	if (x2 >= x1) { max_x = x2; min_x = x1; }
	else { max_x = x1; min_x = x2; }

	if (y2 >= y1) { max_y = y2; min_y = y1; }
	else { max_y = y1; min_y = y2; }
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

void moveControl()
{
	if (kbhit())
	{
		int moveControl = getch();
		switch (moveControl)
		{
		case 'w':
			if (roadAniDelaySpeed != 0)
			{
				roadAniDelaySpeed--;
			}
			break;
		case 's':
			if (roadAniDelaySpeed != 5)
			{
				roadAniDelaySpeed++;
			}
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
			roadAniDelayCount--;
			//playSound(BREAK_SOUND);
			break;
		case 27: //esc
			gotoxy(MAXX - 5 + SCREENSTARTX, MAXY + SCREENSTARTY);
			puts("ESC PRESSED");
			gotoxy(MAXX - 4 + SCREENSTARTX, MAXY + 11);
			puts("GAME OVER");
			exit(1);
			break;
		}
	}
}

void roadDownAnimation()
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
					road[i][j][k] = 1;
				}
			}
		}
	}
}

void fullScreen()
{
	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}

void playSound()
{
	PlaySound(TEXT(runSound), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	/*switch (sound)
	{
	case BACKGROUND_SOUND :
		PlaySound(TEXT(runSound), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case BREAK_SOUND :
		PlaySound(TEXT(breakSound), NULL, SND_FILENAME | SND_ASYNC);
		break;
	}*/
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

void car()
{
	for (int i = 0; i < NORMAL_IMAGE_SIZE; i++)
	{
		for (int j = 0; j < NORMAL_IMAGE_SIZE; j++)
		{
			gotoxy((j + (MAXX / 2) - (NORMAL_IMAGE_SIZE / 2)) * 2 + SCREENSTARTX, i + (MAXY / 2) + SCREENSTARTY);
			putColor(playerCar[i][j]);
		}
	}
}

int putColor(int _color)
{
	if (_color != -1) { color(_color, _color); puts("  "); }
}

void roadAnimation()
{
	if (roadAniDelayCount >= roadAniDelaySpeed)
	{
		if (roadAniFrame != 3) // 애니메이션 장 수
			roadAniFrame++;
		else if (roadAniFrame == 3) // 마지막 애니메이션 장
			roadAniFrame = 0;
		roadAniDelayCount = 0;
	}
	else
		roadAniDelayCount++;
}

void gameSettingReady()
{
	CursorView(0);
	system("mode con:lines=61  cols=141"); //콘솔창 세로, 가로 조정 (이를 사용하면 우측의 슬라이드바가 사라진다.)

	fullScreen(); //전체화면
	Sleep(100); //fullScreen()은 적용되는데에 조금 느리다. 따라서 적용되는 시간을 주기 위해 100 밀리세컨드를 기다려 준다.
	roadDownAnimation(); //도로 애니메이션 설정
	background(); //배경 이미지

	/* 도로 */
	{
		//지평선의 좌측 도로 꼭짓점
		x[0] = (MAXX - 5) / 2; 
		y[0] = (MAXY) / 2;
		//지평선의 우측 도로 꼭짓점
		x[1] = (MAXX + 5) / 2; 
		y[1] = (MAXY) / 2;

		//도로 하단의 좌측 꼭짓점
		x[2] = (0);
		y[2] = (MAXY - 1);
		//도로 하단의 우측 꼭짓점
		x[3] = (MAXX - 1); 
		y[3] = (MAXY - 1);

		//도로 하단의 좌측 (보조) : 이는 도로가 화면 범위를 벗어날 시 꺾어줄 때 도움을 준다.
		x[4] = (0);
		y[4] = (MAXY - 1);
		//도로 하단의 우측 (보조) : 이는 도로가 화면 범위를 벗어날 시 꺾어줄 때 도움을 준다.
		x[5] = (MAXX - 1); 
		y[5] = (MAXY - 1);
	}

	//playSound(runSound); //음악 출력
	car(); //자동차 출력
	CursorView(0); //커서 제거
}

void gamePlaying()
{
	while (TRUE)
	{
		color(BLACK, WHITE);
		moveControl();

		/* 맵 도로 생성 */
		{
			/* 맵 지평선 도로끝 줄 선잇기 */
			MAXorMIN(x[0], x[1], y[0], y[1]);
			lining(x[0], x[1], y[0], y[1], max_x, min_x, max_y, min_y);


			/* 보조선 연결 */
			MAXorMIN(x[2], x[4], y[2], y[4]);
			lining(x[2], x[4], y[2], y[4], max_x, min_x, max_y, min_y);

			MAXorMIN(x[4], x[5], y[4], y[5]);
			lining(x[4], x[5], y[4], y[5], max_x, min_x, max_y, min_y);

			MAXorMIN(x[5], x[3], y[5], y[3]);
			lining(x[5], x[3], y[5], y[3], max_x, min_x, max_y, min_y);


			/*도로 양 끝 꼭짓점과 하단 도로줄 꼭짓점 연결*/

			/*좌측*/
			MAXorMIN(x[0], x[2], y[0], y[2]);
			lining(x[0], x[2], y[0], y[2], max_x, min_x, max_y, min_y);
			/*우측*/
			MAXorMIN(x[1], x[3], y[1], y[3]);
			lining(x[1], x[3], y[1], y[3], max_x, min_x, max_y, min_y);
		}

		printMap();
		roadAnimation();
	}
}

int main(void)
{
	/* 게임 시작전 설정 */
	gameSettingReady();

	/* 게임 동작 */
	gamePlaying();
}
