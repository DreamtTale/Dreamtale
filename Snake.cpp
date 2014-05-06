#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

int delay;

struct snake
{
	int len;
	int body[50][2];
	int head[2];
	int life;					//0活着
	char dir;
} snake;

struct food
{
	int pos[2];
	int flag;					//0存在
} food;

void gotoxy(int x, int y)
{
	COORD pos = {x, y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

void hidden()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = 0;			//赋值为零，隐藏光标
	SetConsoleCursorInfo(hOut, &cci);
}

void init()
{
	int i;
	snake.head[0] = 1;
	snake.head[1] = 1;
	snake.life = 0;
	snake.len = 1;
	snake.dir = 'd';
	snake.body[0][0] = 1;
	snake.body[0][1] = 1;
	delay = 200;
	for(i = 1;i < 50;i++)
	{
		snake.body[i][0] = 0;
		snake.body[i][1] = 0;
	}
	food.flag = 0;
}

void create_windows()
{
	gotoxy(0,0);
	printf("*******************************************************************************\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *  score: 1    *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *  UP   :w     *\n");
	printf("*                                                              *  DOWN :s     *\n");
	printf("*                                                              *  LEFT :a     *\n");
	printf("*                                                              *  RIGHT:d     *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *  PAUSE:space *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *  EXIT :esc   *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*                                                              *              *\n");
	printf("*******************************************************************************\n");
}

void direction()
{
	char keyhit;
	while(kbhit() != 0)
		keyhit = getch();
	if(((keyhit == 'a') || (keyhit == 'w') || (keyhit == 's') || (keyhit == 'd')) && (abs(keyhit/16 - snake.dir/16) == 1))
	{
		snake.dir = keyhit;
	}
	else if(keyhit == ' ')
	{
		gotoxy(30,25);
		system("pause");
		gotoxy(30,25);
		printf("                                     ");
	}
	else if(keyhit == 27)
		exit(0);
}

void draw_snake()
{
	gotoxy(snake.body[snake.len-1][0],snake.body[snake.len-1][1]);
	printf(" ");
	gotoxy(snake.head[0],snake.head[1]);
	printf("*");
}

void create_food()
{
	time_t t;
	srand(time(&t));
	while(1)
	{
		food.pos[0] = rand()%62+1;
		food.pos[1] = rand()%22+1;
		if(food.pos[0] != snake.head[0] && food.pos[1] != snake.head[1])
			break;
	}
	gotoxy(food.pos[0],food.pos[1]);
	printf("+");
}

void update_score()
{
	gotoxy(72,3);
	printf("%2d",snake.len);
}

void eat()
{
	switch(snake.dir)
	{
	case 'w':snake.head[1]--;break;
	case 's':snake.head[1]++;break;
	case 'a':snake.head[0]--;break;
	case 'd':snake.head[0]++;break;
	}
	if(food.pos[0] == snake.head[0] && food.pos[1] == snake.head[1])
	{
		snake.len++;
		update_score();
		food.flag = 1;
	}
}

void update()
{
	int i;
	if(food.flag == 0)
		for(i = snake.len - 1;i > 0;i--)
		{
			snake.body[i][0] = snake.body[i-1][0];
			snake.body[i][1] = snake.body[i-1][1];
		}
	else
	{
		if(snake.len%3 == 0)
		{
			delay-=30;
		}
		for(i = snake.len - 1;i > 0;i--)
		{
			snake.body[i][0] = snake.body[i-1][0];
			snake.body[i][1] = snake.body[i-1][1];
		}
		create_food();
		food.flag = 0;
	}
	snake.body[0][0] = snake.head[0];
	snake.body[0][1] = snake.head[1];		
	food.flag = 0;
}

void state()
{
	int i;
	if(snake.head[0] < 1 || snake.head[0] > 62 || snake.head[1] < 1 || snake.head[1] > 22)
		snake.life = 1;
	for(i = snake.len -1;i > 3;i--)
	{
		if(snake.body[i][0] == snake.head[0] && snake.body[i][1] == snake.head[1])
		{
			snake.life = 1;
			break;
		}
	}
}

void main()
{
	int t = 0;
	char i;
	while(1)
	{
		t = 1;
		system("cls");
		init();
		hidden();
		create_windows();
		create_food();
		while(t != 0)
		{
			Sleep(delay);
			direction();
			eat();
			draw_snake();
			update();
			state();
			if(snake.life == 1)
			{
				system("cls");
				gotoxy(26,11);
				printf("GAMEOVER");
				gotoxy(26,13);
				printf("Do you want to try again(Y to continue):");
				scanf("%c",&i);
				if(i == 'Y' || i == 'y')
				{
					t = 0;
				}
				else
				{
					exit(0);
				}
			}
		}
	}
}