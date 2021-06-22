#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

#define TRUE 1
#define FALSE 0

#define MAXY 40
#define MAXX 60
#define SCREENSTARTX 0 //20
#define SCREENSTARTY 0 //10

#define BLANK_IMAGE -1
#define NORMAL_IMAGE_SIZE 16

//#define runSound "./Sound\\8-Bit-Fantasy-Racing-Music.wav"

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

int cloud[6][10] = {
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, //
	{-1,-1,15,15,15,-1,-1,-1,-1,-1}, //  ■■■     
	{15,15,15,15,15,15,15,15,15,15}, //■■■■■■■■■■
	{15,15,15,15,15,15,15,15,15,15}, //■■■■■■■■■■
	{-1,-1,-1,-1,-1,-1,15,15,15,-1}, //      ■■■ 
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}  //
};

enum colorName
{
	BLACK,  D_BLUE,   D_GREEN,  D_SKYBLUE,	// 0,	1,  2,  3
	D_RED,  D_VIOLET, D_YELLOW, GRAY,	// 4,	5,  6,  7
	D_GRAY, BLUE,     GREEN,    SKYBLUE,	// 8,	9, 10, 11
	RED,    VIOLET,   YELLOW,   WHITE	// 12, 13, 14, 15
};

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}

int color(int bgColor, int textColor)
{
	switch (bgColor)
	{
	case BLACK:	bgColor = 40; break;
	case D_RED:	bgColor = 41; break;
	case D_GREEN:	bgColor = 42; break;
	case D_YELLOW:	bgColor = 43; break;
	case D_BLUE:	bgColor = 44; break;
	case D_VIOLET:	bgColor = 45; break;
	case D_SKYBLUE:	bgColor = 46; break;
	case GRAY:	bgColor = 47; break;
	case D_GRAY:	bgColor = 100;break;
	case RED:	bgColor = 101;break;
	case GREEN:	bgColor = 102;break;
	case YELLOW:	bgColor = 103;break;
	case BLUE:	bgColor = 104;break;
	case VIOLET:	bgColor = 105;break;
	case SKYBLUE:	bgColor = 106;break;
	case WHITE:	bgColor = 107;break;
	}

	//printf("%c[%d;%d;%dm"0x1B,7,49,bgColor); // 특성, 글자색, 바탕색
	printf("%c[%dm", 27, bgColor); // 바탕색
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bgColor * 16 + textColor);
}

int gotoxy(int x, int y)
{
	printf("\033[%d;%df", y, x);
	fflush(stdout);
}

int lining(float x1, float x2, float y1, float y2, float max_x, float min_x, float max_y, float min_y) // x1,y1의 좌표와 x2,y2의 좌표를 선으로 이음
{
	float x, y, a; //이은선 중 x좌표, y좌표 그리고 기울기
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

int printMap() // FLAT 맵 출력 (FLAT을 땅이라 부르기로) //어질어질하다
{
	for (int i = MAXY / 2; i < MAXY - 1; i++) //땅 부분만 출력하면 되므로 지평선의 y값 부터
	{
		char isRoad = FALSE;
		char drawRoadStarted = FALSE;
		for (int j = 0; j < MAXX; j++) //땅의 가로 끝까지
		{
			char imageBlankCheck = FALSE; //현재 커서 좌표의 플레이어 이미지 값이 -1(무색) 인지 확인함
			gotoxy(j * 2 + SCREENSTARTX, i + SCREENSTARTY); //커서 이동
			if ((MAXY / 2 + 7 <= i && i <= MAXY - 5) && (MAXX / 2 - 8 <= j && j <= MAXX / 2 + 7)) // 자동차 범위
			{
				if (playerCar[i - MAXY / 2 + 1][j - MAXX / 2 - 8] == BLANK_IMAGE)
					imageBlankCheck = TRUE; // 현재 출력 위치가 플레이어 이미지의 색상있는 곳에 있으면 거짓, 없으면 참
			}
			if (!((MAXY / 2 + 7 <= i && i <= MAXY - 5) && (MAXX / 2 - 8 <= j && j <= MAXX / 2 + 7)) || imageBlankCheck == TRUE)
			{
				if (flat[i][j] == 0) //도로선이 아닐 경우
				{
					if (isRoad == TRUE) //현재 커서가 도로 위 라면
					{
						drawRoadStarted = TRUE; //도로 색칠이 되고 있음을 알림
						color(D_GRAY, D_GRAY); //도로 색 (어두운 회색)
						flat[i][j] = 1; //도로로 채워지는 과정
						if (road[roadAniFrame][i - MAXY / 2][j] == 0) //이건 도로에 회색을 넣을 것이냐 묻는건데,
							puts("  ");								  //이 puts를 제거해보면 대충 무슨 말인지 알 수 있다.
					}
					else //도로가 아니면
					{
						color(D_GREEN, D_GREEN); //잔디 색 (어두운 초록색)
						if (road[roadAniFrame][i - MAXY / 2][j] == 0) //이건 잔디에 초록색을 넣을 것이냐 묻는건데,
							puts("  "); 							  //이 puts를 제거해보면 대충 무슨 말인지 알 수 있다.
					}
				}
				else if (flat[i][j] == 1) //현재 커서가 도로의 끝이거나 시작이라면
				{
					if (j < MAXX - 1 && i > MAXY / 2 && flat[i][j + 1] == 1)
					{
						isRoad = FALSE;
					}
					if (j < MAXX - 1 && i > MAXY / 2 //이건 지평선의 직선을 제외하고 하라는 말이다.
						&& flat[i][j + 1] == 0) //도로가 아니라면
					{
						if (drawRoadStarted == TRUE) //커서가 도로의 선인데 칠하는 도중에 만났다면 색칠 중단
							isRoad = FALSE;			 //도로가 끝났음을 알림
						else						 //칠하지 않고 있었다면 처음 만났것이므로 도로라는걸 확인함
							isRoad = TRUE;			 //도로가 시작됨을 알림
					}
					color(D_GRAY, D_GRAY); //도로의 시작이거나 끝이므로 색을 도로색으로
					if (road[roadAniFrame][i - MAXY / 2][j] == 0) //이건 위에서 보았다 시피 애니메이션을 통해 색을 바꾸는거다
						puts("  ");								  //이건 도로의 선을 애니메이션에 적용하는가 묻는 구간이다.
				}
				if (road[roadAniFrame][i - MAXY / 2][j] == 1) //현재 커서 좌표가 애니메이션의 1에 해당한다면
				{
					if (flat[i][j] == 1) //도로라면
						color(GRAY, GRAY); //회색
					if (flat[i][j] == 0) //잔디라면
						color(GREEN, GREEN); //초록색
					puts("  ");//색바꿈을 위한 출력
				}
			}
		}
	}
	puts("\n");
	flatClear();
}

int background()
{
	for (int i = 0; i < MAXY / 2; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			gotoxy(j * 2 + SCREENSTARTX, i + SCREENSTARTY);
			putColor(SKYBLUE); //putColor를 통해 하늘색으로 칠한다
							   //putColor는 gotoxy와 color를 동시에 실행 가능한 함수다.
							   //최근에 만든 함수라 몇몇 코드에 이게 적용되어있지 않을 수 있다.
		}
	}
	for (int i = 0; i < MAXY / 2; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			gotoxy(j * 2 + SCREENSTARTX, i + SCREENSTARTY);
			if ((10 <= i && i <= 15) && (25 <= j && j < 35)) //구름 좌표
			{
				putColor(cloud[i - 10][j - 25]); //해당 구름 이미지의 값을 putColor를 통해 출력한다.
			}
			else if ((4 <= i && i <= 9) && (7 <= j && j < 17)) //구름 좌표
			{
				putColor(cloud[i - 4][j - 7]);
			}
			else if ((6 <= i && i <= 11) && (45 <= j && j < 55)) //구름 좌표
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

int MAXorMIN(float x1, float x2, float  y1, float y2) // x1과 x2, y1과 y2의 대소 관계 설정
{
	if (x2 >= x1) { max_x = x2; min_x = x1; }
	else { max_x = x1; min_x = x2; }

	if (y2 >= y1) { max_y = y2; min_y = y1; }
	else { max_y = y1; min_y = y2; }
}

int moveControl()
{
	if (kbhit()) //키보드가 눌렸을 때만
	{
		char command = getchar();
		if (/*GetAsyncKeyState(VK_UP) || */command == UP || command == 'w') //GetAsyncKeyState()를 활용하면 입력할 때 필터키 없이 바로 입력이 가능하기에 거의 필수다
			if (roadAniDelaySpeed != 0) //자동차가 도로를 달리는 방식은 단순하게도 도로 애니메이션으로 인해 움직인다.
										//그 애니메이션 함수가 게임 프레임에 따라 거의 같은 프레임으로 움직이는데,
										//그래서 roadAniDelaySpeed 값을 올리면 자동차의 속력이 낮아진 것 처럼 느껴진다.
										//당연히 값이 0이면 최대속도가 된다.
			{
				roadAniDelaySpeed--;
			}
		if (/*GetAsyncKeyState(VK_DOWN) || */command == DOWN || command == 's')
			if (roadAniDelaySpeed != 5)
			{
				roadAniDelaySpeed++;
			}
		if (/*GetAsyncKeyState(VK_RIGHT) || */command == RIGHT || command == 'd') //좌우 움직임으로 인해 화면 좌우 하단의 꼭짓점을 이동시켜야한다
		{
			if (y[2] > (MAXY / 2 + 3)) //자동차가 우측으로 움직이면 도로는 왼쪽으로 움직여야한다.
									   //따라서 좌측 하단 꼭짓점이 화면밖으로 나갔다면 다시 화면의 최대지점에 두고 y값만 변경시킨다.
									   //해당 if문은 그 y값이 너무 올라가면 안되므로 제한을 둔 것이다.
			{
				if (x[2] > 0) //아직 화면밖으로 나가지 않았다면
				{
					x[2] -= LRspeed; //도로 좌측 하단 꼭짓점을 좌우 움직임의 값 만큼 빼준다.
					x[4] -= LRspeed; //도로 좌측 하단 보조꼭짓점도 마찬가지 (보조꼭짓점은 아래에 설명)
										//이것은 보조꼭짓점인데, 현재 도로를 회색으로 출력하게 도와주는 함수가
										//도로선을 만나면 그 때부터 회색을 칠하고 다시 만나면 색칠을 그만두는 방식이다.
										//만약 보조선 없이 좌측 꼭짓점만 y축이 변경되어 올라간다면
										//자동차가 우측으로 간게 아닌 도로가 왼쪽으로 들어 올려진 것처럼 된다.
										//따라서 보조꼭짓점을 좌측 하단에 두고 이것을 연결시켜두면
										//사다리꼴이 5각형이 되면서 도로의 색이 완전히 칠해진다.
										//이러한 역할을 위해 보조꼭짓점은 기존꼭짓점과 좌표가 같으나 y값은 그대로다.
					if (y[3] < MAXY) //도로 우측 하단 꼭짓점의 y값 또한 변경되어있을 수 있으므로 내려주는 작업이다.
					{
						y[3] += LRspeed; //y값이 올라가면서 화면의 아랫쪽으로 이동된다.
					}
				}
				else if (x[2] <= 0) //좌측하단 꼭짓점이 화면밖으로 나간다면
				{
					x[2] = 0; //도로 좌측 하단 꼭짓점은 0으로 고정해준다
					x[3] -= LRspeed; //도로 우측 하단 꼭짓점은 LRspeed만큼 좌측으로 옮겨준다
					x[5] -= LRspeed; //도로 우측 하단 보조꼭짓점 또한 함께 움직인다.
					y[2] -= LRspeed; //화면밖으로 나가므로 도로 좌측 하단 꼭짓점의 y값을 변경한다.
					
					//y4는 보조꼭짓점 역할이기에 y2와 함께 움직이지 않고 그대로 남아있는다.
				}
			}
		}
		if (/*GetAsyncKeyState(VK_LEFT) || */command == LEFT || command == 'a') //여기도 위와 비슷하다.
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
		if (/*GetAsyncKeyState(VK_SPACE) || */command == ' ') //스페이스바는 브레이크 기능을 한다.
			roadAniDelayCount = -1;		//roadAniDelayCount를 왜 늘리지 않고 줄이냐면
										//roadAniDelaySpeed보다 값이 큰 경우에만 실행되도록 되어있는데,
										//-1이면 speed보다 더 낮기 때문에 함수가 실행되지 않으므로 애니메이션이 고정된다.
		if (/*GetAsyncKeyState(VK_ESCAPE) || */command == 'e') //esc눌러서 게임 끄는용도
		{
			pressedESC();
		}
	}
}

int roadDownAnimation()
{
	char roadMoveScene[4][MAXY / 2] = {
		{0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1}, //1이 색을 바꾸라는 거다
		{1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1}, //이 내용을 바꾸어 보면 어떤 원리인지 가늠할 수 있을 것이다.
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

int car()
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

int roadAnimation()
{
	if (roadAniDelayCount >= roadAniDelaySpeed) //speed가 0에 가까울 수록 호출 빈도가 높아진다.
	{
		if (roadAniFrame != 3) // 애니메이션 장 수
			roadAniFrame++;
		else if (roadAniFrame == 3) // 마지막 애니메이션 장
			roadAniFrame = 0;
		roadAniDelayCount = 0;
	}
	else
		roadAniDelayCount++; //이게 speed와 속도가 일치할 때 마다 애니메이션이 실행된다.
}

int gameSettingReady()
{
	//system("mode con:lines=61  cols=141"); //콘솔창 세로, 가로 조정 (이를 사용하면 우측의 슬라이드바가 사라진다.)

	//fullScreen(); //전체화면
	sleep(100); //fullScreen()은 적용되는데에 조금 느리다. 따라서 적용되는 시간을 주기 위해 100 밀리세컨드를 기다려 준다.
	roadDownAnimation(); //도로 애니메이션 설정
	background(); //배경 그리기 (하늘 및 구름)

	/* 도로 */
	{
		/*
		 *	점과 점을 기울기를 통해 잇는 것을 활용해보고 싶어 이렇게 만들어짐...
		 *	그래서 지평선의 중앙쯤 되는 위치와 화면 좌우 하단에 점을 두고 선으로 이은 후 내부를 채우는 방식이다.
		 *	그로 인해 신경써야할 부분이 굉장히 많아지고 비효율적이게 되었다.
		 *	그대들은 이런 멍청한 짓을 반복하지마시오..
		*/
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

	//playSound(runSound); //배경음악 출력 (playSound()가 두가지 음악을 트는건 안되더라)
	car(); //자동차 출력
	//CursorView(0); //커서 제거
}

int gamePlaying()
{
	while (TRUE) //게임이 실행되는 동안 무한루프
	{
		color(BLACK, WHITE); //유저 조작 가능 이전에 하늘을 출력하는데 해당 커서 위치에 하늘색이 남아있을 수 있으므로 기본값으로 조정
		moveControl(); //유저 조작

		/* 맵 도로 계속 업데이트 */
		{
			//이걸 해주지 않는다면 도로가 출력되지 않는다.

			/* 맵 지평선 도로끝 줄 선잇기 */
			MAXorMIN(x[0], x[1], y[0], y[1]);
			lining(x[0], x[1], y[0], y[1], max_x, min_x, max_y, min_y);
			//MaxMinLining(x[0], x[1], y[0], y[1], max_x, min_x, max_y, min_y);
			//MaxMinLining을 통해 MaxorMin과 lining을 동시에 처리하려고 했으나... 비주얼 스튜디오가 컴파일하는데 문제가 생겨 하지 못하였다.


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

		printMap(); //땅을 출력하는 용도다 (FLAT 출력)
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

int pressedESC()
{
	//DebugText("GAME OFF...");
	//gotoxy(MAXX - 5 + SCREENSTARTX, MAXY + SCREENSTARTY);
	//puts("ESC PRESSED");
	//gotoxy(MAXX - 4 + SCREENSTARTX, MAXY + SCREENSTARTY + 1);
	//puts("GAME OVER");
	exit(1);
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
	/* 게임 시작전 설정 */
	//DebugText("SETTING..."); //잘 작동 되는지 확인하는 용도로 좌측 상단에 텍스트 출력
	gameSettingReady(); //유저가 조작하기 전에 설정해야할 내용들

	/* 게임 동작 */
	//DebugText("PLAYING..."); //잘 작동 되는지 확인하는 용도로 좌측 상단에 텍스트 출력
	gamePlaying(); //유저의 조작 및 같이 실행되어야 하는 내용들
}
