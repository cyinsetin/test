/*=========================================================

    �ĤG��:

      2.3 �G���}�C������
          �}�C�ۥ[:

          ArrayAdd()            �}�C�ۥ[
          Print_Arr()           �L�X�}�C���e
          A[ROWS][COLS]         �}�C��l���e
          ROWS                  �C��
          COLS                  ���

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

   /*clrscr();                    �M���ù�*/
   ArrAdd(A,B,C);
   printf("�x�}A =>\n");
   Print_Arr(A);                /*�L�X�x�} A*/
   printf("�[�x�}B =>\n");
   Print_Arr(B);                /*�L�X�x�} B*/
   printf("����x�}C =>\n");
   Print_Arr(C);                /*�L�X�x�} C*/
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
