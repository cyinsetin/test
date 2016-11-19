/*=========================================================

       5.5.1 �q�Y�@�I���l�U�I���̵u���|

	  Dijkstra()	Dijkstra�t��k
	  find_min()	��XDECIDED[]=0�BDIST�̤p��

	  COST[V][V]	�ϧΪ��[�v�F���x�}
	  DIST[V]	�q�_�I��U�I���̵u�Z��
	  PRIOR[V]	�U�I�̵u���|�����e�@�ӳ��I
	  DECIDED[V]	�̵u���w�O�_�w�M�w

  =========================================================
*/

#include <stdio.h>

#define V	7

int COST[V][V]={{0,1,4,5,15000,15000,15000},
		{1,0,15000,2,15000,15000,15000},
		{4,15000,0,4,15000,3,15000},
		{5,2,4,0,5,2,15000},
		{15000,15000,15000,5,0,15000,6},
		{15000,15000,3,2,15000,0,4},
		{15000,15000,15000,15000,6,4,0}};

int DIST[V],PRIOR[V],DECIDED[V];

void Dijstra(),find_min();
void PrintPath();

void main(void)
{
    int i,V0;
    /*clrscr();*/

    printf("�п�J�_�I: ");
    scanf("%d",&V0);
    Dijstra(V0);

    for(i=0;i < V;i++)
    {
	if(i == V0)
		continue;
	printf("\nV%2d --> V%2d  COST == %d\n",V0,i,DIST[i]);
	PrintPath(V0,i);
    }
}

void Dijstra(int V0)
{
    int Vx;
    int i,w;
    for(i=0;i<V;i++)
    {
	DIST[i]=COST[V0][i];
	PRIOR[i]=0;
	DECIDED[i]=0;
    }
    DECIDED[0]=1;

    for(i=1;i<V;i++)
    {
	  find_min(&Vx);
	  DECIDED[Vx]=1;
	  for(w=0;w<V;w++)
	  {
		if(!DECIDED[w] && (DIST[w] > (DIST[Vx]+COST[Vx][w])))
		{
			DIST[w]=DIST[Vx]+COST[Vx][w];
			PRIOR[w]=Vx;
		}
	  }
    }
}

void find_min(int *Vx)
{
    int i;
    int l=0,lowest=32767;
    for(i=0;i<V;i++)
	if (!DECIDED[i] && DIST[i]<lowest)
	{
		lowest=DIST[i];
		l=i;
	}
    *Vx=l;
}

void PrintPath(int V0,int Vx)
{
  int i;

  printf("V%d �e�@���O ",Vx);
  for (i=PRIOR[Vx]; i!= V0 ; i=PRIOR[i])
       printf("V%d, V%d �e�@���O ",i,i);
  printf("%d\n ",V0);
}
