/*=========================================================
    �ĤG��

      2.3 �G���}�C������
          �}�C��m:

          ArrTrans()            �}�C��m
          A[ROWS][COLS]         �s��}�C��l���e
          B[COLS][ROWS]         �s��}�C��m�᤺�e
          ROWS                  �C��
          COLS                  ���

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

   /*clrscr();                    �M���ù�*/

   ArrTrans(A,B);
   printf("�x�}A =>\n");
   for(i=0;i < ROWS;i++)
   {
        for(j=0;j < COLS;j++)
                printf(" %d",A[i][j]);
        printf("\n");
   }
   printf("�x�}B =>\n");
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