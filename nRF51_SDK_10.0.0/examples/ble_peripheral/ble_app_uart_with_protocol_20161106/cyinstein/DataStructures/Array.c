/*=========================================================

    第二章 陣列

       2.2 陣列的運算

          ArrayInsert()         插入節點
          ArrayDelete()         刪除節點
          ArrayCopy()           陣列拷貝
          PrintArray()          印出陣列內容
          list1[ARRAY_SPACE]    存放陣列初始內容
          list2[ARRAY_SPACE]    存放陣列運算結果
          ARRAY_SPACE           陣列大小
  =========================================================
*/
#include <stdio.h>

#define ARRAY_SPACE      8
void ArrayInsert(),ArrayDelete(),ArrayCopy(),PrintArray();
void main(void)
{
   
   int list1[ARRAY_SPACE]={5,25,11,26,39,18,120,50};
   int list2[ARRAY_SPACE],i,value,choose;
   int loopflag=1;

   /*clrscr();清除螢幕*/
   while (loopflag)
   {
     printf("\n (1) Insert,(2) Delete,(3) Copy");
     printf("\n 請輸入Choose 1-3, 其它鍵離開 ( other key to escape) :");
     scanf("%d",&choose);               /*將選擇結果存入choose*/
     switch(choose)             /*依所選擇之結果執行*/
      {
        case 1:                 /*詢問其插入點及數值*/
                printf("陣列位置從0 開始( Starting from 0 ).\n");
                printf("原陣列 ( The content of array ) :");
                PrintArray(list1,ARRAY_SPACE);
                printf("\n請輸入欲插入之節點位置 ( Input Insert Position )=>");
                scanf("%d",&i);
                printf("請輸入欲插入之數值 ( Input Insert value )=>");
                scanf("%d",&value);
                ArrayInsert(list1,ARRAY_SPACE,i,value);
                printf("結果輸出 ( Result )=>");
                PrintArray(list1,ARRAY_SPACE);
                break;
        case 2:                 /*詢問欲刪除點*/
                printf("陣列位置從0 開始( Starting from 0 ).\n");
                printf("原陣列( The content of array )  :");
                PrintArray(list1,ARRAY_SPACE);
                printf("\n請輸入欲刪除之節點位置 ( Input Position )=>");
                scanf("%d",&i);
                ArrayDelete(list1,ARRAY_SPACE,i);
                printf("結果輸出 ( Result )=>");
                PrintArray(list1,ARRAY_SPACE);
                break;
        case 3:                 /*執行ArrayCopy*/
                printf("陣列位置從0 開始( Starting from 0 ).\n");
                printf("原陣列( The content of array )  :");
                PrintArray(list1,ARRAY_SPACE);
                ArrayCopy(list2,list1,ARRAY_SPACE);
                printf("\n結果輸出 list2=>");
                PrintArray(list2,ARRAY_SPACE);
                break;
        default:
                printf("Good Bye!\n");
                loopflag=0;             /*選項錯誤,程式結束*/
                break;
      }
   }
}

/* 在list 陣列(大小為 n 元素)上,將 value 插入位置 i */

void ArrayInsert(int list[],unsigned int n,unsigned int i,int value)
{
   unsigned int counter = n-1;

   if(i<0 || i>=n)      return;
   for(;counter > i;counter--)
        list[counter] = list[counter-1];
   list[i] = value;
}

/* 在元素個數為 n 的 list 陣列,刪除第 i 個元素 */

void ArrayDelete(int list[],unsigned int n,unsigned int i)
{
   unsigned int counter = i;

   if(i < 0)    return;
   for(;counter < n-1;counter++)
        list[counter] = list[counter+1];
   list[n-1] = 0;
}

/* 將 list2[] 複製給list1[],兩者元素個數皆為 n */

void ArrayCopy(int list1[],int list2[],unsigned int n)
{
   unsigned int counter;

   for(counter=0;counter < n;counter++)
        list1[counter] = list2[counter];
}

/*將陣列內容顯示在螢幕上*/

void PrintArray(int list[],int n)
{
   int i;

   for(i=0;i < n;i++)
        printf("%d,",list[i]);
}