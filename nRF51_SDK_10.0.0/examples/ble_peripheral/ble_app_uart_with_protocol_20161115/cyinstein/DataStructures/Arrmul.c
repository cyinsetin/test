/*=========================================================
    �ĤG��

      2.3 �G���}�C������
          �}�C�ۭ�:

          ArrMul()              �}�C�ۭ�
          A[M][N]               �s��}�C A ��l���e
          B[N][T]               �s��}�C B ��l���e
          C[M][T]               �s��"�}�C A" * "�}�C B"�ᤧ���e
          M                     �}�CA���C��
          N                     �}�CA����Ƥΰ}�CB���C��
          T                     �}�CB�����

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

   c/*lrscr();                    �M���ù�*/
   ArrMul(A,B,C);

   printf("�x�}A =>\n");
   for(i=0;i < M;i++)
   {
        for(j=0;j < N;j++)
                printf(" %d",A[i][j]);
        printf("\n");
   }
   printf("���x�}B =>\n");
   for(i=0;i < N;i++)
   {
        for(j=0;j < T;j++)
                printf(" %d",B[i][j]);
        printf("\n");
   }
   printf("����x�}C =>\n");
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