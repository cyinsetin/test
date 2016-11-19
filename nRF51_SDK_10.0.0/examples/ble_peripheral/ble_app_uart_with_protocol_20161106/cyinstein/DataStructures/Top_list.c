/*=========================================================

       5.6 拓樸排序
	  使用鄰接串列

	  topolody_sort()  拓樸排序函式
	  M_to_List()	   將鄰接矩陣作成鄰接串列

  =========================================================
*/

#include <stdio.h>

#define V	7
typedef struct node
	{
		int vertex;
		struct node *next;
	}ANODE;
ANODE AdjList[V];

int M[V][V]={	{0,1,1,1,0,0,0},
		{0,0,0,0,0,1,0},
		{0,0,0,0,1,0,0},
		{0,0,0,0,1,0,0},
		{0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1},
		{0,0,0,0,0,0,0}};

void M_to_List(),topology_sort();

void main(void)
{
    M_to_List();
    /*clrscr();*/
    topology_sort();
}

void topology_sort(void)
{
    int i,n,top=-1,k;
    ANODE *p;
    for(i=0;i < V;i++)
		if(AdjList[i].vertex == 0)
		{
			AdjList[i].vertex=top;
			top=i;
		}
    for(i=0;i < V;i++)
    {
		if(top == -1)
		{
			printf("NetWork has a cycle\n");
			break;
		}
		n=top;
		top=AdjList[top].vertex;
		printf("%d",n);
		p=AdjList[n].next;
		while(p != NULL)
		{
			k=p->vertex;
			(AdjList[k].vertex)--;
			if(AdjList[k].vertex == 0)
			{
				AdjList[k].vertex=top;
				top=k;
			}
			p=p->next;
		}
    }
}

void M_to_List(void)
{
    int i,j;
    ANODE *new,*p;

    for(i=0;i < V;i++)
    {
	AdjList[i].next=NULL;
	p=&AdjList[i];
	for(j=0;j < V;j++)
		if(M[i][j] == 1)
		{
			AdjList[j].vertex++;

			new=malloc(sizeof(ANODE));
			new->vertex=j;
			new->next=NULL;
			p->next=new;
			p=p->next;
		}
    }
}
