# include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
//kbhit();
static int i=0;
struct food
{
	int x,y;
	char val;
}f;
struct snake
{
	int x,y;
	char val;
	struct snake *ptr;
}s[42];
void create_food()
{
f.x=5;
f.y=5;
f.val='F';
}
void create_snake()
{
//struct	snake* s=malloc(sizeof(struct snake),1);
	s[i].x=10,s[i].y=10;
	s[i].val='<';
	i++;
		s[i].x=10,s[i].y=11;
	s[i].val='|';
	i++;
			s[i].x=10,s[i].y=12;
	s[i].val='|';
	i++;
			s[i].x=10,s[i].y=13;
	s[i].val='|';
	i++;
//	s->ptr=malloc(sizeof(struct snake),1);
/*	s->ptr->x=10;
		s->ptr->y=11;
		s->ptr->val='#';
			s->ptr->ptr=malloc(sizeof(struct snake),1);*/
			
	
}
void snake_increment()
{
	
s[i].x=s[i-1].x;
s[i].y=s[i-1].y+1;
s[i].val='|';
i++;

}

void food_change()
{
	static int j=2,l=4;
	f.x=j;
	f.y=l;
	if(f.x==s[0].x||f.y==s[0].y)
	{
	f.x=j-1;
	f.y=l-1;	
	}
	if(f.x==41)
{
f.x=1;
j=1;	
}
if(l==41)
{
	l=1;
	f.y=1;
}
	j++;
	l=l+2;

}
void snake_position(char ch)
{//static char c='a';

	int k;
	
	if(ch=='l')
	{
	if(s[0].y-1==s[1].y)
	return;
		
	else	if(s[0].y==1)
		s[0].y=49;
			for(k=i-1;k>0;k--)
		{
			s[k].y=s[k-1].y;
			s[k].x=s[k-1].x;
		}
		s[0].y=s[0].y-1;
	}
else	 if(ch=='r')
	{
		if(s[0].y+1==s[1].y)
		{
		return;
		}
		if(s[0].y==48)
		s[0].y=0;
		for(k=i-1;k>0;k--)
		{
			s[k].y=s[k-1].y;
			s[k].x=s[k-1].x;
		}
		s[0].y=s[0].y+1;
	}
else if(ch=='u')
	{
		if(s[0].x-1==s[1].x)
		{
		return;
		}
		if(s[0].x==1)
		s[0].x=41;
		
		for(k=i-1;k>0;k--)
		{
			s[k].y=s[k-1].y;
			s[k].x=s[k-1].x;
		}
		s[0].x=s[0].x-1;
	
	}
else	if(ch=='d')
	{
		if(s[0].x+1==s[1].x)
		{
	return;
		}
		if(s[0].x==40)
		s[0].x=0;
			for(k=i-1;k>0;k--)
		{
			s[k].y=s[k-1].y;
			s[k].x=s[k-1].x;
		}
		s[0].x=s[0].x+1;
	}




}
void display(char board[42][50])
{
	//clrscr();
	int k,j;
	printf("\n");
	for(k=0;k<42;k++)
{
for(j=0;j<50;j++)
{
	if(k==0||j==0||k==41||j==49)
	board[k][j]='*';
printf("%c",board[k][j]);
}
printf("\n");
}
}

void Delay()
{
    
    long double i;
    for(i=0;i<=(50000000);i++);
}
int main()
{
	printf("snake game \n");
static char board[42][50];
int k,j,d=0,score=0;
	for(k=0;k<42;k++)
{
for(j=0;j<50;j++)
board[k][j]='-';
}
create_food();
create_snake();
board[f.x][f.y]=f.val;

for(k=0;k<i;k++)
board[s[k].x][s[k].y]=s[k].val;




display(board);

char ch;
 printf("enter\n");
	ch=getch();
while(1)
{
//system("cls");
printf("score=%d\n",score);
	 Delay();
	 if(ch=='q')
	  {
	  	printf("you pressed quit button");
	  	return 0;
	  }

		if(kbhit())
		{
		printf("enter\n");
ch=	getch();
		snake_position(ch);
	}
	else
		snake_position(ch);
		
		// game over condition
			  for(j=1;j<i;j++)
{
	if(s[0].x==s[j].x&&s[j].y==s[0].y)
	{
		printf("GAME OVER\n");
		return 0;
	}
}




	
		for(k=0;k<42;k++)
	for(j=0;j<50;j++)
board[k][j]='-';
board[f.x][f.y]=f.val;
	for(k=0;k<i
	;k++)
board[s[k].x][s[k].y]=s[k].val;
display(board);
if(s[0].x==f.x&&s[0].y==f.y)
{
	score++;
	snake_increment();
	food_change();
		for(k=0;k<42;k++)
	for(j=0;j<50;j++)
board[k][j]='-';
board[f.x][f.y]=f.val;
	for(k=0;k<i
	;k++)
board[s[k].x][s[k].y]=s[k].val;
	display(board);
}


}
	



	return 0;
}
