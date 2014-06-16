#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>

#define CELL 20
#define xStart 40
#define yStart 100
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
int nNumber = 1;
int bflag = 0;
int Score =0;
int xRand =0;
int yRand =0;
static int Snake[100]={0x00050007,0x00050007};
int A_shape[20][30];
int direct =LEFT;
void initrect()
{
	for(int i=0; i<20; i++)
		for(int k=0; k<30; k++)
		{
			if(i==0 || i==19)
				A_shape[i][k] = 1;
			else
			{
				if(k==0 || k==29)
					A_shape[i][k] = 1;
				else
					A_shape[i][k] = 0;
			}
		}
}

int Control()
{
	int Key;
	while(kbhit())
	{
		Key=getch();
		switch(Key)
		{
		case 'd':
		case 'D':direct = RIGHT;break;
		case 'a':
		case 'A':direct = LEFT;break;
		case 'w':
		case 'W':direct = UP;break;
		case 's':
		case 'S':direct = DOWN;break;
		default :break;
		}
	}
	return 0;
}

void A_init(int color)
{
	for(int i=0; i<20; i++)
		for(int k=0; k<30; k++)
		{
			if(A_shape[i][k])			
			{	setfillstyle(color);
			bar(xStart+k*CELL,yStart+i*CELL,xStart+k*CELL+CELL,i*CELL+yStart +CELL);
			}
			else
			{	setfillstyle(BLACK);
			bar(xStart+k*CELL,yStart+i*CELL,xStart+k*CELL+CELL,i*CELL+yStart +CELL);
			}		
		}
}

int direction(int x,int y)
{
	switch(direct)
	{
	case UP:    return ( (x-1)<<16 ) | y;
	case DOWN:  return ( (x+1)<<16 ) | y;
	case RIGHT: return ( x<<16 ) | (y+1);
	case LEFT:  return ( x<<16 ) | (y-1);
	default :break;
	}
}
void move()
{
	
	for(int i=nNumber; i>=0; i--)
	{
		Snake[i+1] = Snake[i];
		A_shape[Snake[i]>>16][Snake[i]<<16>>16] = 1;
	}
	A_shape[Snake[nNumber+1]>>16][Snake[nNumber+1]<<16>>16] = 0;
	Snake[0]= direction(Snake[0]>>16,(Snake[0]<<16)>>16);
}
void food()
{
	int x,y;
	randomize();
	xRand = random(18)+1;
	yRand = random(28)+1;
	A_shape[xRand][yRand] = 2;
}
void feed()
{
	char szBuffer[20]={0};
	if(A_shape[Snake[0]>>16][Snake[0]<<16>>16] ==2)
	{
		nNumber++;
		Score+=100;
		sprintf(szBuffer,"Score:%d",Score);
		outtextxy(99,10,szBuffer);
	}
}
int main()
{
	setinitmode(0,1,1);
	initgraph(700,600);
	initrect();
	while(A_shape[Snake[0]>>16][Snake[0]<<16>>16] !=1)
	{
		Control();
		move();
		feed();
		if(A_shape[xRand][yRand] != 2)
		food();
		Control();
		A_init(RED);
		delay(400);
	};
	return 0;
}