/*=========================================================

       7.3:Shell�ƧǪk

          Shell_sort()          Shell�ƧǪk�禡
          a[ARR_NUM]            ���ƧǤ����
          ARR_NUM               ���ƧǤ���Ƽƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9
void Shell_sort();
void main(void)
{
   
   int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("�Ƨǫe(before sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   Shell_sort(a,ARR_NUM);

   printf("\n�Ƨǫ� ( after sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
}
void Shell_sort(int a[],int n)
{
   int i,j,h,up;

   for(h=1;h <= n/9;h=3*h+1);

   for(;h > 0;h=h/3)
        for(i=h;i < n;i++)
        {
                up=a[i];
                j=i;
                while(j>=h && a[j-h]>up)
                {
                        a[j]=a[j-h];
                        j=j-h;
                }
                a[j]=up;
        }
}
