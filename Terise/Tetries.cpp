#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>


#define WIDE  800
#define HIGH  800

#define CELL	30
char szBuffer[30]={0};
int A_shape[23][13];
int x=0,y=0,ran;
int X=5,Y=0;
int yOffset=1,xOffset =0;
int nShape =0;
int xStart =200;
int yStart = 50;
int shape[8]={15,8738,785,63488,34956,143,561,51};
int Score = 0;

int Kuangjia()
{
	sprintf(szBuffer,"Score:%d",Score++);
	outtextxy(600,200,szBuffer);
	rectangle(xStart+CELL-1,yStart+CELL-1,xStart +11*CELL+1,yStart +21*CELL+1);
	return 0;
}

void initrect()
{
	for(int i=0;i<22;i++)
			for(int j=0; j<12; j++)
			{
				if(i==21)
				A_shape[i][j]=1;
				else
				{
				if(j==0 || j==11)
				 A_shape[i][j] = 1;
				else
					 A_shape[i][j]=0;
				}
			}
}
int A_init(int color, int bFlag)
{
	int m, n;
	for(m=1;m<21;m++)
		for(n=1;n<11;n++)
		{
		if(A_shape[m][n])			
			{	setfillstyle(color);
			bar(xStart+n*CELL,yStart+m*CELL,xStart+n*CELL+CELL,m*CELL+yStart +CELL);
			}
		else
			{	setfillstyle(BLACK);
				bar(xStart+n*CELL,yStart+m*CELL,xStart+n*CELL+CELL,m*CELL+yStart +CELL);
			}		
		}
		return 0;
}
void line()
{
	int x=0;

	int m,n,i,k;
	for(m=1;m<21;m++,x=0)
		for(n=1;n<11;n++)
		{
			x+=A_shape[m][n];
			if( x==10)
			{
				for(i =m;i>0;i--)
					for(k =1;k<11;k++)
					{
						A_shape[i][k] = A_shape[i-1][k];
					}
					m--;
					Score+=100;
					sprintf(szBuffer,"Score:%d",Score);
					outtextxy(600,200,szBuffer);;
			}
		}
}
int haha(int x, int y, int nData,int bFlag)
{
	
	for(int i=0; i<4; i++)
		for(int k=0; k<4; k++,nData>>=1)
		{
			switch(bFlag)
			{
			case 1: A_shape[i+y][k+x] ^= (nData%2);break;
			case 2: if( A_shape[i+y+yOffset][k+x+xOffset]  && (nData%2) )  return 1;break;
			case 3: A_shape[i+y][k+x] |= (nData%2); 	break;
			default:  break;
			}
		}
		return 0;
}
int Control()
{
	
	int Key;
	while(kbhit())
		{
			Key=getch();
			switch(Key)
			{
			case 'D':
				xOffset =1;
				if( !haha(X,Y,shape[nShape],2))  X++;
				xOffset = 0;
				break;
			case 'A':
				xOffset =-1;
				if( !haha(X,Y,shape[nShape],2))  X--;
				xOffset = 0;
				break;
			case 'W': 
				nShape = ++nShape>7?0:nShape;break;
			default :break;
			}
	}
	return 0;
}
lyh()
{
	randomize();
	nShape = random(7);
//	rectangle(600,100,750,300);

	
}
	int main()
	{	
		int i,j;
		
		setinitmode(0,1,1);
		initgraph(WIDE,HIGH);
		initrect();
		setfillstyle(RED);
		Kuangjia();

		while(1)
		{

			A_init(RED,1);
			Control();
			haha(X,Y,shape[nShape],3);
			A_init(RED,1);
			delay(100);
			haha(X,Y,shape[nShape],1);
			
			if( haha(X,Y,shape[nShape],2) )
			{
				haha(X,Y,shape[nShape],3);
				line();
				lyh();
				Y=0;
				X=5;
			}
			Y++;
		}
		closegraph();
	return 0;
}
