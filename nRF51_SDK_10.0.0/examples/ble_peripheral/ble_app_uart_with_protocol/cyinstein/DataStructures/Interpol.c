/*=========================================================

       8.2 在循序結構上的搜尋
         8.2.3 內插搜尋法

          inter_Srch()          內插搜尋函式
          a[ARR_NUM]            欲搜尋之資料
          ARR_NUM               欲搜尋之資料數目(陣列大小)

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
   printf("陣列內容:");
   PrintArray(a,ARR_NUM);
   printf("\n請輸入欲搜尋之資料內容 => ");
   scanf("%d",&data);

   if((locate=InterSearch(a,ARR_NUM,data)) == -1)
        printf("找不到資料!!!");
   else
        printf("找到資料 %d 於第 %d 個位置(由0算起)",data,locate);
}

/*在a[0..n-1]陣列尋找鍵值key的元素,並傳回位置 ,使用binary Search*/
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
                return(m);      /*找到,傳回位置*/
        if(key > a[m])          /*右半部,改便左限*/
                l=m+1;
        else                    /*左半部,改便右限*/
                r=m-1;
    }

    return(-1);         /*失敗傳回*/
}

void  PrintArray(int a[], int n)
{
    int i;
    for (i=0; i<n; i++)
      printf("%d ",a[i]);
 }
