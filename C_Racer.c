#pragma warning(disable : 4996)
#include <stdio.h>
#include <Windows.h>
#include <WinUser.h> //����Ű ���������� �߰� ������ ������ �ǹ� (�׳� �̷��� ������ �˻��ؼ� ����غ��ƶ�)
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

int flat[MAXY][MAXX] = { 0, }; //��
int road[4][MAXY / 2][MAXX] = { 0, }; //����

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
	{-1,-1,15,15,15,-1,-1,-1,-1,-1}, //  ����     
	{15,15,15,15,15,15,15,15,15,15}, //�����������
	{15,15,15,15,15,15,15,15,15,15}, //�����������
	{-1,-1,-1,-1,-1,-1,15,15,15,-1}, //      ���� 
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

void lining(float x1, float x2, float y1, float y2, float max_x, float min_x, float max_y, float min_y) // x1,y1�� ��ǥ�� x2,y2�� ��ǥ�� ������ ����
{
	float x, y, a; //������ �� x��ǥ, y��ǥ �׸��� ����
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

void printMap() // FLAT �� ��� (FLAT�� ���̶� �θ����) //���������ϴ�
{
	for (int i = MAXY / 2; i < MAXY - 1; i++) //�� �κи� ����ϸ� �ǹǷ� ������ y�� ����
	{
		char isRoad = FALSE;
		char drawRoadStarted = FALSE;
		for (int j = 0; j < MAXX; j++) //���� ���� ������
		{
			char imageBlankCheck = FALSE; //���� Ŀ�� ��ǥ�� �÷��̾� �̹��� ���� -1(����) ���� Ȯ����
			gotoxy(j * 2 + SCREENSTARTX, i + SCREENSTARTY); //Ŀ�� �̵�
			if ((MAXY / 2 + 7 <= i && i <= MAXY - 5) && (MAXX / 2 - 8 <= j && j <= MAXX / 2 + 7)) // �ڵ��� ����
			{
				if (playerCar[i - MAXY / 2 + 1][j - MAXX / 2 - 8] == BLANK_IMAGE)
					imageBlankCheck = TRUE; // ���� ��� ��ġ�� �÷��̾� �̹����� �����ִ� ���� ������ ����, ������ ��
			}
			if (!((MAXY / 2 + 7 <= i && i <= MAXY - 5) && (MAXX / 2 - 8 <= j && j <= MAXX / 2 + 7)) || imageBlankCheck == TRUE)
			{
				if (flat[i][j] == 0) //���μ��� �ƴ� ���
				{
					if (isRoad == TRUE) //���� Ŀ���� ���� �� ���
					{
						drawRoadStarted = TRUE; //���� ��ĥ�� �ǰ� ������ �˸�
						color(D_GRAY, D_GRAY); //���� �� (��ο� ȸ��)
						flat[i][j] = 1; //���η� ä������ ����
						if (road[roadAniFrame][i - MAXY / 2][j] == 0) //�̰� ���ο� ȸ���� ���� ���̳� ���°ǵ�,
							puts("  ");								  //�� puts�� �����غ��� ���� ���� ������ �� �� �ִ�.
					}
					else //���ΰ� �ƴϸ�
					{
						color(D_GREEN, D_GREEN); //�ܵ� �� (��ο� �ʷϻ�)
						if (road[roadAniFrame][i - MAXY / 2][j] == 0) //�̰� �ܵ� �ʷϻ��� ���� ���̳� ���°ǵ�,
							puts("  "); 							  //�� puts�� �����غ��� ���� ���� ������ �� �� �ִ�.
					}
				}
				else if (flat[i][j] == 1) //���� Ŀ���� ������ ���̰ų� �����̶��
				{
					if (j < MAXX - 1 && i > MAXY / 2 && flat[i][j + 1] == 1)
					{
						isRoad = FALSE;
					}
					if (j < MAXX - 1 && i > MAXY / 2 //�̰� ������ ������ �����ϰ� �϶�� ���̴�.
						&& flat[i][j + 1] == 0) //���ΰ� �ƴ϶��
					{
						if (drawRoadStarted == TRUE) //Ŀ���� ������ ���ε� ĥ�ϴ� ���߿� �����ٸ� ��ĥ �ߴ�
							isRoad = FALSE;			 //���ΰ� �������� �˸�
						else						 //ĥ���� �ʰ� �־��ٸ� ó�� �������̹Ƿ� ���ζ�°� Ȯ����
							isRoad = TRUE;			 //���ΰ� ���۵��� �˸�
					}
					color(D_GRAY, D_GRAY); //������ �����̰ų� ���̹Ƿ� ���� ���λ�����
					if (road[roadAniFrame][i - MAXY / 2][j] == 0) //�̰� ������ ���Ҵ� ���� �ִϸ��̼��� ���� ���� �ٲٴ°Ŵ�
						puts("  ");								  //�̰� ������ ���� �ִϸ��̼ǿ� �����ϴ°� ���� �����̴�.
				}
				if (road[roadAniFrame][i - MAXY / 2][j] == 1) //���� Ŀ�� ��ǥ�� �ִϸ��̼��� 1�� �ش��Ѵٸ�
				{
					if (flat[i][j] == 1) //���ζ��
						color(GRAY, GRAY); //ȸ��
					if (flat[i][j] == 0) //�ܵ���
						color(GREEN, GREEN); //�ʷϻ�
					puts("  ");//���ٲ��� ���� ���
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
			putColor(SKYBLUE); //putColor�� ���� �ϴû����� ĥ�Ѵ�
							   //putColor�� gotoxy�� color�� ���ÿ� ���� ������ �Լ���.
							   //�ֱٿ� ���� �Լ��� ��� �ڵ忡 �̰� ����Ǿ����� ���� �� �ִ�.
		}
	}
	for (int i = 0; i < MAXY / 2; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			gotoxy(j * 2 + SCREENSTARTX, i + SCREENSTARTY);
			if ((SCREENSTARTY <= i && i <= 15) && (25 <= j && j < 35)) //���� ��ǥ
			{
				putColor(cloud[i - SCREENSTARTY][j - 25]); //�ش� ���� �̹����� ���� putColor�� ���� ����Ѵ�.
			}
			else if ((4 <= i && i <= 9) && (7 <= j && j < 17)) //���� ��ǥ
			{
				putColor(cloud[i - 4][j - 7]);
			}
			else if ((6 <= i && i <= 11) && (45 <= j && j < 55)) //���� ��ǥ
			{
				putColor(cloud[i - 6][j - 45]);
			}
		}
	}
	color(BLACK, WHITE);
}

int flatClear() // Ŭ����
{
	int blankFlat[MAXY][MAXX] = { 0, };
	memcpy(flat, blankFlat, sizeof(blankFlat));
}

void MAXorMIN(float x1, float x2, float  y1, float y2) // x1�� x2, y1�� y2�� ��� ���� ����
{
	if (x2 >= x1) { max_x = x2; min_x = x1; }
	else { max_x = x1; min_x = x2; }

	if (y2 >= y1) { max_y = y2; min_y = y1; }
	else { max_y = y1; min_y = y2; }
}

void CursorView(int show) // �Է� Ŀ�� ����
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
	if (kbhit()) //Ű���尡 ������ ����
	{
		if (GetAsyncKeyState(VK_UP)) //GetAsyncKeyState()�� Ȱ���ϸ� �Է��� �� ����Ű ���� �ٷ� �Է��� �����ϱ⿡ ���� �ʼ���
			if (roadAniDelaySpeed != 0) //�ڵ����� ���θ� �޸��� ����� �ܼ��ϰԵ� ���� �ִϸ��̼����� ���� �����δ�.
										//�� �ִϸ��̼� �Լ��� ���� �����ӿ� ���� ���� ���� ���������� �����̴µ�,
										//�׷��� roadAniDelaySpeed ���� �ø��� �ڵ����� �ӷ��� ������ �� ó�� ��������.
										//�翬�� ���� 0�̸� �ִ�ӵ��� �ȴ�.
			{
				roadAniDelaySpeed--;
			}
		if (GetAsyncKeyState(VK_DOWN))
			if (roadAniDelaySpeed != 5)
			{
				roadAniDelaySpeed++;
			}
		if (GetAsyncKeyState(VK_RIGHT)) //�¿� ���������� ���� ȭ�� �¿� �ϴ��� �������� �̵����Ѿ��Ѵ�
		{
			if (y[2] > (MAXY / 2 + 3)) //�ڵ����� �������� �����̸� ���δ� �������� ���������Ѵ�.
									   //���� ���� �ϴ� �������� ȭ������� �����ٸ� �ٽ� ȭ���� �ִ������� �ΰ� y���� �����Ų��.
									   //�ش� if���� �� y���� �ʹ� �ö󰡸� �ȵǹǷ� ������ �� ���̴�.
			{
				if (x[2] > 0) //���� ȭ������� ������ �ʾҴٸ�
				{
					x[2] -= LRspeed; //���� ���� �ϴ� �������� �¿� �������� �� ��ŭ ���ش�.
					x[4] -= LRspeed; //���� ���� �ϴ� ������������ �������� (������������ �Ʒ��� ����)
										//�̰��� �����������ε�, ���� ���θ� ȸ������ ����ϰ� �����ִ� �Լ���
										//���μ��� ������ �� ������ ȸ���� ĥ�ϰ� �ٽ� ������ ��ĥ�� �׸��δ� ����̴�.
										//���� ������ ���� ���� �������� y���� ����Ǿ� �ö󰣴ٸ�
										//�ڵ����� �������� ���� �ƴ� ���ΰ� �������� ��� �÷��� ��ó�� �ȴ�.
										//���� ������������ ���� �ϴܿ� �ΰ� �̰��� ������ѵθ�
										//��ٸ����� 5������ �Ǹ鼭 ������ ���� ������ ĥ������.
										//�̷��� ������ ���� ������������ ������������ ��ǥ�� ������ y���� �״�δ�.
					if (y[3] < MAXY) //���� ���� �ϴ� �������� y�� ���� ����Ǿ����� �� �����Ƿ� �����ִ� �۾��̴�.
					{
						y[3] += LRspeed; //y���� �ö󰡸鼭 ȭ���� �Ʒ������� �̵��ȴ�.
					}
				}
				else if (x[2] <= 0) //�����ϴ� �������� ȭ������� �����ٸ�
				{
					x[2] = 0; //���� ���� �ϴ� �������� 0���� �������ش�
					x[3] -= LRspeed; //���� ���� �ϴ� �������� LRspeed��ŭ �������� �Ű��ش�
					x[5] -= LRspeed; //���� ���� �ϴ� ���������� ���� �Բ� �����δ�.
					y[2] -= LRspeed; //ȭ������� �����Ƿ� ���� ���� �ϴ� �������� y���� �����Ѵ�.
					
					//y4�� ���������� �����̱⿡ y2�� �Բ� �������� �ʰ� �״�� �����ִ´�.
				}
			}
		}
		if (GetAsyncKeyState(VK_LEFT)) //���⵵ ���� ����ϴ�.
		{
			if (y[3] > (MAXY / 2 + 3))
			{
				if (x[3] < MAXX - 1)
				{
					x[3] += LRspeed;
					x[5] += LRspeed;
					if (y[2] < MAXY)
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
		}
		if (GetAsyncKeyState(VK_SPACE)) //�����̽��ٴ� �극��ũ ����� �Ѵ�.
			roadAniDelayCount = -1;		//roadAniDelayCount�� �� �ø��� �ʰ� ���̳ĸ�
										//roadAniDelaySpeed���� ���� ū ��쿡�� ����ǵ��� �Ǿ��ִµ�,
										//-1�̸� speed���� �� ���� ������ �Լ��� ������� �����Ƿ� �ִϸ��̼��� �����ȴ�.
		if (GetAsyncKeyState(VK_ESCAPE)) //esc������ ���� ���¿뵵
		{
			DebugText("GAME OFF...");
			gotoxy(MAXX - 5 + SCREENSTARTX, MAXY + SCREENSTARTY);
			puts("ESC PRESSED");
			gotoxy(MAXX - 4 + SCREENSTARTX, MAXY + 11);
			puts("GAME OVER");
			exit(1);
		}
	}
}

void roadDownAnimation()
{
	char roadMoveScene[4][MAXY / 2] = {
		{0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1}, //1�� ���� �ٲٶ�� �Ŵ�
		{1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1}, //�� ������ �ٲپ� ���� � �������� ������ �� ���� ���̴�.
		{1,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
		{0,1,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0}
	};
	for (short i = 0; i < 4; i++)
	{
		for (short j = 0; j < MAXY / 2; j++)
		{
			if (roadMoveScene[i][j] == 1)
			{
				for (short k = 0; k < MAXX; k++)
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
	//���� ���� ��ġ, SND_ASYNC, SND_LOOP �������� ���� �߿�
	//SND_ASYNC : ����ϸ鼭 �����ڵ� ����
	//SND_LOOP : �ݺ����
	//while (1)
	//{
	//	int a;
	//	printf("����?");
	//	scanf("%d", &a);
	//	if (a == 0)
	//	{
	//		PlaySound(NULL, 0, 0);
	//		Sleep(1000);
	//		printf("�뷡�� �����߽��ϴ�. �Լ��� �����մϴ�.");
	//		return;
	//	}
	//}
}

void car()
{
	for (short i = 0; i < NORMAL_IMAGE_SIZE; i++)
	{
		for (short j = 0; j < NORMAL_IMAGE_SIZE; j++)
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
	if (roadAniDelayCount >= roadAniDelaySpeed) //speed�� 0�� ����� ���� ȣ�� �󵵰� ��������.
	{
		if (roadAniFrame != 3) // �ִϸ��̼� �� ��
			roadAniFrame++;
		else if (roadAniFrame == 3) // ������ �ִϸ��̼� ��
			roadAniFrame = 0;
		roadAniDelayCount = 0;
	}
	else
		roadAniDelayCount++; //�̰� speed�� �ӵ��� ��ġ�� �� ���� �ִϸ��̼��� ����ȴ�.
}

void gameSettingReady()
{
	system("mode con:lines=61  cols=141"); //�ܼ�â ����, ���� ���� (�̸� ����ϸ� ������ �����̵�ٰ� �������.)

	fullScreen(); //��üȭ��
	Sleep(100); //fullScreen()�� ����Ǵµ��� ���� ������. ���� ����Ǵ� �ð��� �ֱ� ���� 100 �и������带 ��ٷ� �ش�.
	roadDownAnimation(); //���� �ִϸ��̼� ����
	background(); //��� �׸��� (�ϴ� �� ����)

	/* ���� */
	{
		/*
		 *	���� ���� ���⸦ ���� �մ� ���� Ȱ���غ��� �;� �̷��� �������...
		 *	�׷��� ������ �߾��� �Ǵ� ��ġ�� ȭ�� �¿� �ϴܿ� ���� �ΰ� ������ ���� �� ���θ� ä��� ����̴�.
		 *	�׷� ���� �Ű����� �κ��� ������ �������� ��ȿ�����̰� �Ǿ���.
		 *	�״���� �̷� ��û�� ���� �ݺ��������ÿ�..
		*/
		//������ ���� ���� ������
		x[0] = (MAXX - 5) / 2; 
		y[0] = (MAXY) / 2;
		//������ ���� ���� ������
		x[1] = (MAXX + 5) / 2; 
		y[1] = (MAXY) / 2;

		//���� �ϴ��� ���� ������
		x[2] = (0);
		y[2] = (MAXY - 1);
		//���� �ϴ��� ���� ������
		x[3] = (MAXX - 1); 
		y[3] = (MAXY - 1);

		//���� �ϴ��� ���� (����) : �̴� ���ΰ� ȭ�� ������ ��� �� ������ �� ������ �ش�.
		x[4] = (0);
		y[4] = (MAXY - 1);
		//���� �ϴ��� ���� (����) : �̴� ���ΰ� ȭ�� ������ ��� �� ������ �� ������ �ش�.
		x[5] = (MAXX - 1); 
		y[5] = (MAXY - 1);
	}

	playSound(runSound); //������� ��� (playSound()�� �ΰ��� ������ Ʈ�°� �ȵǴ���)
	car(); //�ڵ��� ���
	CursorView(0); //Ŀ�� ����
}

void gamePlaying()
{
	while (TRUE) //������ ����Ǵ� ���� ���ѷ���
	{
		color(BLACK, WHITE); //���� ���� ���� ������ �ϴ��� ����ϴµ� �ش� Ŀ�� ��ġ�� �ϴû��� �������� �� �����Ƿ� �⺻������ ����
		moveControl(); //���� ����

		/* �� ���� ��� ������Ʈ */
		{
			//�̰� ������ �ʴ´ٸ� ���ΰ� ��µ��� �ʴ´�.

			/* �� ���� ���γ� �� ���ձ� */
			MAXorMIN(x[0], x[1], y[0], y[1]);
			lining(x[0], x[1], y[0], y[1], max_x, min_x, max_y, min_y);
			//MaxMinLining(x[0], x[1], y[0], y[1], max_x, min_x, max_y, min_y);
			//MaxMinLining�� ���� MaxorMin�� lining�� ���ÿ� ó���Ϸ��� ������... ���־� ��Ʃ����� �������ϴµ� ������ ���� ���� ���Ͽ���.


			/* ������ ���� */
			MAXorMIN(x[2], x[4], y[2], y[4]);
			lining(x[2], x[4], y[2], y[4], max_x, min_x, max_y, min_y);

			MAXorMIN(x[4], x[5], y[4], y[5]);
			lining(x[4], x[5], y[4], y[5], max_x, min_x, max_y, min_y);

			MAXorMIN(x[5], x[3], y[5], y[3]);
			lining(x[5], x[3], y[5], y[3], max_x, min_x, max_y, min_y);


			/*���� �� �� �������� �ϴ� ������ ������ ����*/

			/*����*/
			MAXorMIN(x[0], x[2], y[0], y[2]);
			lining(x[0], x[2], y[0], y[2], max_x, min_x, max_y, min_y);
			/*����*/
			MAXorMIN(x[1], x[3], y[1], y[3]);
			lining(x[1], x[3], y[1], y[3], max_x, min_x, max_y, min_y);
		}

		printMap(); //���� ����ϴ� �뵵�� (FLAT ���)
		roadAnimation();
	}
}

int MaxMinLining(float x1, float x2, float y1, float y2, float max_x, float min_x, float max_y, float min_y)
{
	MAXorMIN(x1, x2, y1, y2);
	lining(x1, x2, y1, y2, max_x, min_x, max_y, min_y);
	//DebugText("TEST");
	//DebugFloat(x1);
}

int DebugText(char word[])
{
	gotoxy(1, 1), puts("                               ");
	gotoxy(1, 1), color(BLACK, WHITE), printf("%s", word);
}

int DebugInt(int num)
{
	gotoxy(1, 1), puts("                               ");
	gotoxy(1, 1), color(BLACK, WHITE), printf("%d", num);
}

int DebugFloat(float num)
{
	gotoxy(1, 1), puts("                               ");
	gotoxy(1, 1), color(BLACK, WHITE), printf("%f", num);
}

int main(void)
{
	/* ���� ������ ���� */
	DebugText("SETTING..."); //�� �۵� �Ǵ��� Ȯ���ϴ� �뵵�� ���� ��ܿ� �ؽ�Ʈ ���
	gameSettingReady(); //������ �����ϱ� ���� �����ؾ��� �����

	/* ���� ���� */
	DebugText("PLAYING..."); //�� �۵� �Ǵ��� Ȯ���ϴ� �뵵�� ���� ��ܿ� �ؽ�Ʈ ���
	gamePlaying(); //������ ���� �� ���� ����Ǿ�� �ϴ� �����
}

//nabilera1@naver.com