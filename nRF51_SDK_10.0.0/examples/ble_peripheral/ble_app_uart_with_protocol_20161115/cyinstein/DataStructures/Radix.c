/*=========================================================

       7.6:��ƱƧǪk

          radix_sort()          ��ƱƧǪk�禡
          get_key()             ���o�S�w��ƭ�(�p�Ӧ�,�Q��)
          a[ARR_NUM]            ���ƧǤ����
          ARR_NUM               ���ƧǤ���Ƽƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>
#include <math.h>

#define ARR_NUM 9

int count[10];
void radix_sort();
int get_key();
void main(void)
{
   
   int a[ARR_NUM]={137,219,532,655,422,164,98,422,334},b[ARR_NUM],i;

   /*clrscr();*/
   printf("�Ƨǫe( before sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   radix_sort(a,b,ARR_NUM,10,3);

   printf("\n�Ƨǫ� ( after sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
}

/*�Ƨ� a[0]..a[n-1],��ȥu��0..m-1 �@m�إi�� �C����Ȧ�k���*/
void radix_sort(int a[],int b[],int n,int m,int k)
{
   int i,j,d,pass;

   for(pass=0;pass < k;pass++)
   {
        for(j=0;j < m;j++)
                count[j]=0;
        for(i=0;i < n;i++)              /*�p��U��ȥX�{����*/
        {
                d=get_key(a[i],pass,m);
                count[d]++;
        }
        for(j=1;j < m;j++)
                count[j]=count[j]+count[j-1];

        for(i=n-1;i >= 0;i--)           /*���t*/
        {
                d=get_key(a[i],pass,m);
                b[count[d]-1]=a[i];
                count[d]--;
        }
   }

   for(i=0;i < n;i++)           /*�����^a[]*/
        a[i]=b[i];
}

int get_key(int a,int pass,int m)
{
   a=a/pow(m,pass);
   a=a%m;
   return(a);
}
