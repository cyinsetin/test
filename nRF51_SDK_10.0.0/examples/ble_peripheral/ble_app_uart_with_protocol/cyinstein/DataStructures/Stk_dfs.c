/*=========================================================

       5.3.1 深度優先拜訪(DFS)
	利用堆疊

	  DFS ()
	  visit()	對頂點Vx作拜訪
	  Find_Adj()	找出與Vx相鄰且未被拜訪過的節點
	  PUSH()	推入堆疊
	  is_full()	測試是否已滿
	  POP() 	自堆疊彈出資料
	  is_empty()	測試是否已空

  =========================================================
*/

#include <stdio.h>

#define V	9	/*頂點數目*/
#define MAX_ITEM	50

typedef struct stk
	{
	   int item[MAX_ITEM];	      /*資料欄位*/
	   int top;
	}STACK;
STACK S;

void DFS(),visit();
char Find_Adj();
int PUSH(),is_full(),POP(),is_empty();

int visited[V],Col;
int a[V][V]={{0,1,1,1,0,0,0,0,0},
	     {1,0,0,0,0,0,0,0,0},
	     {1,0,0,0,0,0,1,1,0},
	     {1,0,0,0,1,1,0,0,0},
	     {0,0,0,1,0,0,0,0,0},
	     {0,0,0,1,0,0,1,0,1},
	     {0,0,1,0,0,1,0,1,0},
	     {0,0,1,0,0,0,1,0,0},
	     {0,0,0,0,0,1,0,0,0}};

void main(void)
{
    int  V0;

    /*clrscr();*/
	S.top=-1;
    printf("請輸入起點:");
    scanf("%d",&V0);
    DFS(V0);
}

void DFS(int V0)
{
    int Vx,Vy;
    PUSH (V0);
    while(!is_empty(&S))
    {
	POP (&Vx);
	if(!visited[Vx])
	{
		visit(Vx);
		Vy=Find_Adj(Vx,1);
		while(Vy!=-1)
		{
			PUSH(Vy);
			Vy=Find_Adj(Vx,0);
		}
	}
    }
}

void visit(int Vx)
{
    visited[Vx]=1;
    printf("V%2d\n",Vx);
}

char Find_Adj(int Vx,char first)
{
    if (first)
	Col=0;
    else
	Col++;
    while(Col < V)
    {
	if (a[Vx][Col]==0 || visited[Col])
		Col++;
	else
		return (Col);
    }
    return(-1);
}

int PUSH(int X)
{
    if(is_full(&S))
	return(0);
    S.top=S.top+1;
    S.item[S.top]=X;
	return(1);
}

int is_full(STACK *s)
{
    if (s->top == (MAX_ITEM-1))
	return(1);
    else
	return(0);
}

int POP(int *X)
{
    if(is_empty(&S))
	return (0);
    *X=S.item[S.top];
    S.top--;
	return(1);
}

int is_empty(STACK *s)
{
    if(s->top == -1)
		return(1);
    else
		return(0);
}
