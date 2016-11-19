/*=========================================================

       8.2 �b�`�ǵ��c�W���j�M
         8.2.3 �����j�M�k

          inter_Srch()          �����j�M�禡
          a[ARR_NUM]            ���j�M�����
          ARR_NUM               ���j�M����Ƽƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>

#define ARR_NUM 10
int InterSearch();
void PrintArray();

void main(void)
{
   int a[ARR_NUM]={6,33,39,41,52,55,69,77,78,81};
   int data,locate;

   /*clrscr();*/
   printf("�}�C���e:");
   PrintArray(a,ARR_NUM);
   printf("\n�п�J���j�M����Ƥ��e => ");
   scanf("%d",&data);

   if((locate=InterSearch(a,ARR_NUM,data)) == -1)
        printf("�䤣����!!!");
   else
        printf("����� %d ��� %d �Ӧ�m(��0��_)",data,locate);
}

/*�ba[0..n-1]�}�C�M�����key������,�öǦ^��m ,�ϥ�binary Search*/
int InterSearch(int a[],int n,int key)
{
    int l=0,r=n-1,m;
    float x;

    while(l <= r)
    {
        if(a[r]-a[l] != 0)
                x=(float)(key-a[l])/(a[r]-a[l]);
        else
                x=1/2;
        m=l+((int)x*(r-1));
        if(key == a[m])
                return(m);      /*���,�Ǧ^��m*/
        if(key > a[m])          /*�k�b��,��K����*/
                l=m+1;
        else                    /*���b��,��K�k��*/
                r=m-1;
    }

    return(-1);         /*���ѶǦ^*/
}

void  PrintArray(int a[], int n)
{
    int i;
    for (i=0; i<n; i++)
      printf("%d ",a[i]);
 }
