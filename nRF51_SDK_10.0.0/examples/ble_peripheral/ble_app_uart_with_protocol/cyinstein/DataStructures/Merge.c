/*=========================================================

       7.4:合併排序法

          merge_sort()          合併排序法函式
          a[ARR_NUM]            欲排序之資料
          b[ARR_NUM]            排序時暫存資料用
          ARR_NUM               欲排序之資料數目(陣列大小)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9


void merge_sort();
void main(void)
{
   
   int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("排序前 ( before sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   merge_sort(a,0,ARR_NUM-1);

   printf("\n排序後 ( after sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   printf("\n");
}
void merge_sort(int a[],int l,int r)
{
	int i,j,k,m;
	int b[ARR_NUM];
	if(l < r)
	{
        m=(l+r)/2;
        merge_sort(a,l,m);
        merge_sort(a,m+1,r);

        for(i=l;i <= m;i++)
                b[i]=a[i];
        for(j=m+1;j <= r;j++)
                b[r+(m-j+1)]=a[j];
        for(i=l,j=r,k=l;k <= r;k++)
                a[k]=(b[i] < b[j]) ? b[i++]:b[j--];
	}
}
