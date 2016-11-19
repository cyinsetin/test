/*=========================================================

    第二章:

      2.3 二維陣列的應用
          陣列相加:

          ArrayAdd()            陣列相加
          Print_Arr()           印出陣列內容
          A[ROWS][COLS]         陣列初始內容
          ROWS                  列數
          COLS                  行數

  =========================================================
*/
#include <stdio.h>

#define ROWS    4
#define COLS    3
void ArrAdd();
void Print_Arr();

void main(void)
{
   int A[ROWS][COLS]={  {1,2,3},
                        {4,5,6},
                        {7,8,9},
                        {10,11,12}};
   int B[ROWS][COLS]={  {0,1,2},
                        {3,4,5},
                        {6,7,8},
                        {9,10,11}};
   int C[ROWS][COLS];

   /*clrscr();                    清除螢幕*/
   ArrAdd(A,B,C);
   printf("矩陣A =>\n");
   Print_Arr(A);                /*印出矩陣 A*/
   printf("加矩陣B =>\n");
   Print_Arr(B);                /*印出矩陣 B*/
   printf("等於矩陣C =>\n");
   Print_Arr(C);                /*印出矩陣 C*/
}

void ArrAdd(int A[ROWS][COLS],int B[ROWS][COLS],int C[ROWS][COLS])
{
   int i,j;

   for(i=0;i < ROWS;i++)
        for(j=0;j < COLS;j++)
                C[i][j]=A[i][j]+B[i][j];
}

void Print_Arr(int A[ROWS][COLS])
{
   int i,j;

   for(i=0;i < ROWS;i++)
   {
        for(j=0;j < COLS;j++)
                printf(" %d",A[i][j]);
        printf("\n");
   }
}
