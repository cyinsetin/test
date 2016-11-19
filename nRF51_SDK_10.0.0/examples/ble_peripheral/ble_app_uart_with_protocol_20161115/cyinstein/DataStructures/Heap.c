/*=========================================================

       7.7:��n�ƧǪk

          heap_sort()           ��ƱƧǪk�禡
          construct_by_adjust() �N�}�C�վ㦨��n
          downheap()            �վ��n,�H�ŦX�S��
          a[ARR_NUM]            ���ƧǤ����
          ARR_NUM               ���ƧǤ���Ƽƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 9

void heap_sort();
void construct_by_adjust();
void downheap();

void main(void)
{
   int a[ARR_NUM]={37,41,19,81,41,25,56,61,49},i;

   /*clrscr();*/
   printf("�Ƨǫe ( before sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
   heap_sort(a,ARR_NUM);

   printf("\n�Ƨǫ� ( after sorting ):\n");
   for(i=0;i < ARR_NUM;i++)
        printf(" %d",a[i]);
}

void heap_sort(int a[],int n)
{
   int k,temp;

   construct_by_adjust(a,n);    /*�إ߰�n*/

   while(n > 0)
   {
        temp=a[n-1];            /*��ڸ�̫�@�Ӥ����洫*/
        a[n-1]=a[0];
        a[0]=temp;
        n--;
        downheap(a,n,0);        /*�վ㦨heap*/
   }
}

/*�νվ㪺�覡�إ߰�n a[n]*/
void construct_by_adjust(int a[],int n)
{
   int k;

   for(k=n/2-1;k >= 0;k--)
        downheap(a,n,k);

}

/*�Na[k]���U�վ�,�H�ŦXheap�S��*/
void downheap(int a[],int n,int k)
{
   int j,up;
   up=a[k];
   while(k < n/2)               /*a[k]�������l�`�I*/
   {
        j=2*k+1;                /*a[k]������la[j]*/
        if(j+1<n && a[j]<a[j+1])
                j++;
        /*�Y�k��l���j�B�s�b;a[j]��a[k]���k��l*/
        if(up >= a[j])
                break;  /*�j��j��l*/
        a[k]=a[j];              /*�j��l�W��*/
        k=j;
   }
   a[k]=up;
}
