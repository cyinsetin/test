/*=========================================================

       7.2:�򥻱ƧǪk
         7.2.3��w�ƧǪk

          bubble_sort()         ��w�ƧǪk�禡
          a[ARR_NUM]            ���ƧǤ����
          ARR_NUM               ���ƧǤ���Ƽƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9
void bubble_sort();
void main(void)
{
   
   int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("�Ƨǫe ( before sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   bubble_sort(a,ARR_NUM);

   printf("\n�Ƨǫ� ( after sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
}
void bubble_sort(int a[],int n)
{
   int i,j,temp;

   for(i=n-1;i >= 0;i--)
        for(j=1;j <= i;j++)
                if(a[j-1] > a[j])
                {
                        temp=a[j-1];
                        a[j-1]=a[j];
                        a[j]=temp;
                }
}
