/*=========================================================

       7.2:�򥻱ƧǪk
         7.2.2���J�ƧǪk


          insert_sort()         ���J�ƧǪk�禡
          a[ARR_NUM]            ���ƧǤ����
          ARR_NUM               ���ƧǤ���Ƽƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9
void insrt_sort();
void main(void)
{
   
   int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("�Ƨǫe ( before sorting )\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   insrt_sort(a,ARR_NUM);

   printf("\n�Ƨǫ� ( after sorting )\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
}
void insrt_sort(int a[],int n)
{
   int i,j,up;

   for(i=1;i < n;i++)
   {
        up=a[i];
        j=i;
        while(j>0 && a[j-1]>up)
        {
                a[j]=a[j-1];
                j--;
        }
        a[j]=up;
   }
}
