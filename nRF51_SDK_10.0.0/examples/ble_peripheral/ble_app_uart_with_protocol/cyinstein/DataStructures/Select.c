/*=========================================================

       7.2:�򥻱ƧǪk
         7.2.1��ܱƧǪk

          select_sort()         ��ܱƧǪk�禡
          a[ARR_NUM]            ���ƧǤ����
          ARR_NUM               ���ƧǤ���Ƽƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9
void select_sort();
void main(void)
{
   
   int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("�Ƨǫe (before sorting)\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);

   select_sort(a,ARR_NUM);

   printf("\n�Ƨǫ� ( after sorting )\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
}
void select_sort(int a[],int n)
{
   int i,j,min,temp;

   for(i=0;i < n-1;i++)
   {
        min=i;
        for(j=i+1;j < n-1;j++)
                if(a[j] < a[min])
                        min=j;
        temp=a[min];
        a[min]=a[i];
        a[i]=temp;
   }
}
