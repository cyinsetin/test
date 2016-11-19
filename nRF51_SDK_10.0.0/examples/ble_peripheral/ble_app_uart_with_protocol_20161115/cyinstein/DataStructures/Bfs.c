/*=========================================================

       5.3.1 �s���u�����X(BFS)

	  BFS ()
	  visit()		�ﳻ�IVx�@���X
	  enqueue()		�N���IVx��J��C
	  dequeue()		�q��C���X�@�ӳ��I��Vx
	  is_empty()		���զ�C�O�_����
	  Find_Adj()		��X�PVx�۾F�B���Q���X
				�L���`�I

  =========================================================
*/

#include <stdio.h>

#define MAX_ITEM 50
#define V	9	/*���I�ƥ�*/

typedef struct que
	{
		int item[MAX_ITEM];
		int front;
		int rear;
	}QUEUE;
QUEUE q;

int Col;
int visited[V];
int a[V][V]={{0,1,1,1,0,0,0,0,0},
	     {1,0,0,0,0,0,0,0,0},
	     {1,0,0,0,0,0,1,1,0},
	     {1,0,0,0,1,1,0,0,0},
	     {0,0,0,1,0,0,0,0,0},
	     {0,0,0,1,0,0,1,0,1},
	     {0,0,1,0,0,1,0,1,0},
	     {0,0,1,0,0,0,1,0,0},
	     {0,0,0,0,0,1,0,0,0}};

void BFS (),visit();
int enqueue(),dequeue(),is_empty();
char Find_Adj();

void main(void)
{
    int V0;
    /*clrscr();*/
    printf("�п�J�_�I: ");
    scanf("%d",&V0);
    BFS(V0);
}

void BFS (int V0)
{
    int Vx,Vy;
    visit(V0);
    enqueue(V0);
    while(!is_empty())
    {
		dequeue(&Vx);
		Vy=Find_Adj(Vx,1);
		while(Vy!=-1)
		{
			visit(Vy);
			enqueue(Vy);
			Vy=Find_Adj(Vx,0);
		}
    }
}

void visit(int Vx)
{
    visited[Vx]=1;
    printf("V%2d\n",Vx);
}

int enqueue(int Vx)
{
    if ((q.rear+1)%MAX_ITEM==q.front)
	return(0);
    q.rear=(q.rear+1)%MAX_ITEM;
    q.item[q.rear]=Vx;
    return(1);
}

int dequeue(int *Vx)
{
    if(is_empty())
	return(0);
    q.front=((q.front+1)%MAX_ITEM);
    *Vx=q.item[q.front];
    return(1);
}

int is_empty()
{
    if(q.front == q.rear)
	 return (1);
    else
	 return (0);
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
    return (-1);
}
