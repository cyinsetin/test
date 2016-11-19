/*=========================================================

       7.5:�ֳt�ƧǪk

          quick_sort()          �ֳt�ƧǪk�禡
          partition()           ���ΰ}�C�禡
          a[ARR_NUM]            ���ƧǤ����
          ARR_NUM               ���ƧǤ���Ƽƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9
void quick_sort();
int partition();
void main(void)
{
    int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("�Ƨǫe ( before sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   quick_sort(a,0,ARR_NUM-1);

   printf("\n�Ƨǫ� ( after sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   printf("\n");
}

/*�ֳt�ƧǪk*/
void quick_sort(int a[],int l,int r)
{
   int partition_point;
   if(l < r)
   {
        partition_point=partition(a,l,r);       /*����*/

        quick_sort(a,l,partition_point-1);      /*���B�k�b��*/

        quick_sort(a,partition_point+1,r);      /*���B���b��*/
   }
}

/*�� a[l]..a[r] �i�����,�Ǧ^�����I*/
int partition(int a[],int l,int r)
{
   int i,j,temp;

   for(i=l+1,j=r;;)
   {
        while(a[i]<a[l] && i<=r)
                i++;
        while(a[j] > a[l])
                j--;

        if(i >= j)
                break;
        temp=a[i];
        a[i]=a[j];
        a[j]=temp;
   }
   temp=a[l];
   a[l]=a[j];
   a[j]=temp;

   return(j);
}
