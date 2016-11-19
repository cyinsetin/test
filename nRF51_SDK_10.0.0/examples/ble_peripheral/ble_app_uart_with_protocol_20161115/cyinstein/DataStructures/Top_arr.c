/*=========================================================

       5.6 �ݾ�Ƨ�
	  �ϥξF���x�}

	  topolody_sort()  �ݾ�ƧǨ禡
	  choose_next()    ���SELECTED��0�B�J��
			   ��׬�0�����I


	  M[V][V]	�ϧΪ��F���x�}
	  INDEGREE[V]	�U���I���J�����,��]��0
	  SELECTED[V]	�ЦW�U���I�O�_�w�Q��X,��]��0

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