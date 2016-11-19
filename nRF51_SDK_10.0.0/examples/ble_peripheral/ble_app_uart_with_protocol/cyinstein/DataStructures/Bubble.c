/*=========================================================

       7.2:基本排序法
         7.2.3氣泡排序法

          bubble_sort()         氣泡排序法函式
          a[ARR_NUM]            欲排序之資料
          ARR_NUM               欲排序之資料數目(陣列大小)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9
void bubble_sort();
void main(void)
{
   
   int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("排序前 ( before sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   bubble_sort(a,ARR_NUM);

   printf("\n排序後 ( after sorting ):\n");
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