/*=========================================================

       7.2:基本排序法
         7.2.1選擇排序法

          select_sort()         選擇排序法函式
          a[ARR_NUM]            欲排序之資料
          ARR_NUM               欲排序之資料數目(陣列大小)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9
void select_sort();
void main(void)
{
   
   int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("排序前 (before sorting)\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);

   select_sort(a,ARR_NUM);

   printf("\n排序後 ( after sorting )\n");
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