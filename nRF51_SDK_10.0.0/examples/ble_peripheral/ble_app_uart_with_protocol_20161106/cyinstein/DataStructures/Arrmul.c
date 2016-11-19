/*=========================================================
    第二章

      2.3 二維陣列的應用
          陣列相乘:

          ArrMul()              陣列相乘
          A[M][N]               存放陣列 A 初始內容
          B[N][T]               存放陣列 B 初始內容
          C[M][T]               存放"陣列 A" * "陣列 B"後之內容
          M                     陣列A之列數
          N                     陣列A之行數及陣列B之列數
          T                     陣列B之行數

  =========================================================
*/

#include <stdio.h>

#define M       3
#define N       2
#define T       4
void ArrMul();

void main(void)
{

   int A[M][N]={{1,2},
                {4,5},
                {3,6}};
   int B[N][T]={{0,1,1,2},
                {4,0,-1,3}};
   int C[M][T],i,j;

   c/*lrscr();                    清除螢幕*/
   ArrMul(A,B,C);

   printf("矩陣A =>\n");
   for(i=0;i < M;i++)
   {
        for(j=0;j < N;j++)
                printf(" %d",A[i][j]);
        printf("\n");
   }
   printf("乘矩陣B =>\n");
   for(i=0;i < N;i++)
   {
        for(j=0;j < T;j++)
                printf(" %d",B[i][j]);
        printf("\n");
   }
   printf("等於矩陣C =>\n");
   for(i=0;i < M;i++)
   {
        for(j=0;j < T;j++)
                printf(" %d",C[i][j]);
        printf("\n");
   }
}

void ArrMul(int A[M][N],int B[N][T],int C[M][T])
{
   int i,j,k;

   for(i=0;i < M;i++)
        for(j=0;j < T;j++)
                C[i][j]=0;      /*clear C Array*/

   for(i=0;i < M;i++)
        for(j=0;j < T;j++)
                for(k=0;k < N;k++)
                        C[i][j]=C[i][j]+A[i][k]*B[k][j];
}