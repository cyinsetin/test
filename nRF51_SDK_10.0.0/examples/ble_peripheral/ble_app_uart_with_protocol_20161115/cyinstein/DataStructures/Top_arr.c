/*=========================================================

       5.6 拓樸排序
	  使用鄰接矩陣

	  topolody_sort()  拓樸排序函式
	  choose_next()    找到SELECTED為0且入分
			   支度為0的頂點


	  M[V][V]	圖形的鄰接矩陣
	  INDEGREE[V]	各頂點的入分支度,初設為0
	  SELECTED[V]	標名各頂點是否已被輸出,初設為0

  =========================================================
*/

#include <stdio.h>

#define V	9

int M[V][V]={	{0,1,1,0,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0},
		{0,0,0,0,1,0,0,1,0},
		{0,0,0,0,0,1,1,1,0},
		{0,0,0,0,0,0,0,0,1},
		{0,0,0,0,0,0,0,0,1},
		{0,0,0,0,0,0,0,0,1},
		{0,0,0,0,0,0,0,0,0}};

int INDEGREE[V];
char SELECTED[V];

void topolody_sort();
int choose_next();

void main(void)
{
    /*clrscr();*/
    topolody_sort();
}

void topolody_sort(void)
{
    int i,j;
    int Vx;
    for (i=0;i < V;i++)
		INDEGREE[i]=SELECTED[i]=0;
    for(i=0;i < V;i++)
		for(j=0;j < V;j++)
			if(M[i][j] == 1)
				INDEGREE[j]++;

    for(i=0;i < V;i++)
    {
		Vx=choose_next();
		printf("%d",Vx);
		for(j=0;j < V;j++)
			if (M[Vx][j] == 1)
			{
				M[Vx][j]=0;
				INDEGREE[j]--;
			}
	}	
}

int choose_next(void)
{
    int i;
    for(i=0;i < V;i++)
	if (!SELECTED[i] && !INDEGREE[i])
	{
		SELECTED[i]=1;
		return(i);
	}
}
