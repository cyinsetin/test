/*=========================================================
    第二章

      2.3 二維陣列的應用
          陣列轉置:

          ArrTrans()            陣列轉置
          A[ROWS][COLS]         存放陣列初始內容
          B[COLS][ROWS]         存放陣列轉置後內容
          ROWS                  列數
          COLS                  行數

  =========================================================
*/

#include <stdio.h>

#define ROWS    4
#define COLS    3
void ArrTrans();

void main(void)
{
   int A[ROWS][COLS]={  {1,2,3},
                        {4,5,6},
                        {7,8,9},
                        {10,11,12}};
   int B[COLS][ROWS],i,j;

   /*clrscr();                    清除螢幕*/

   ArrTrans(A,B);
   printf("矩陣A =>\n");
   for(i=0;i < ROWS;i++)
   {
        for(j=0;j < COLS;j++)
                printf(" %d",A[i][j]);
        printf("\n");
   }
   printf("矩陣B =>\n");
   for(i=0;i < COLS;i++)
   {
        for(j=0;j < ROWS;j++)
                printf(" %d",B[i][j]);
        printf("\n");
   }
}

void ArrTrans(int A[][COLS],int B[][ROWS])
{
   int i,j;

   for(i=0;i < ROWS;i++)
        for(j=0;j < COLS;j++)
                B[j][i]=A[i][j];
}