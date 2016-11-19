/*=========================================================

       4.2 用陣列結構實作堆疊

	  PUSH()	推入堆疊
	  is_full()	測試是否已滿
	  POP() 	自堆疊彈出資料
	  is_empty()	測試是否已空

  =========================================================
*/

#include <stdio.h>

#define MAX_ITEM	9

typedef struct stk
	{
	   int item[MAX_ITEM];        /*資料欄位*/
	   int	top;
	}STACK;
STACK S;
int PUSH(),is_full(),POP(),is_empty();
void PrintStack();
void main(void)
{
   int choose,LoopFlag=1;
   char data;

   S.top=-1;
   /*clrscr();*/
   while(LoopFlag)
   {
	printf("\n 堆疊內容由上到下 : \n");
	PrintStack(&S);
	printf("\n(1)ＰＵＳＨ資料\n(2)ＰＯＰ資料\n(0)結束=>");
	scanf("%d",&choose);

	switch(choose)
	{
	case 0:
		LoopFlag=0;			/*結束程式*/
		break;
	case 1:
		printf("請輸入欲ＰＵＳＨ之資料=>");
		scanf("\n%d",&data);
		if(PUSH(&S,data) == 0)
			printf("堆疊已滿,ＰＵＳＨ失敗");
		break;
	case 2:
		if(POP(&S,&data) == 0)
			printf("堆疊已空,ＰＯＰ失敗");
		else
			printf("POP 出 : %d",data);
		break;
	default:
		printf("選項錯誤");
	}
   }
}

void PrintStack( STACK *s)
{
   int i;
   if  ( is_empty(s) )
      return ;
   for (i=s->top;i >= 0;i--)
      printf("%d ",s->item[i]);
   printf("\n");
}

int PUSH(STACK *S,int X)
{
    if(is_full(S))
	return(0);
    S->top=S->top+1;
    S->item[S->top]=X;
	return(1);
}

int is_full(STACK *S)
{
    if (S->top == (MAX_ITEM-1))
	return(1);
    else
	return(0);
}

int POP(STACK *S,int *X)
{
    if(is_empty(S))
	return (0);
    *X=S->item[S->top];
    S->top--;
	return(1);
}

int is_empty(STACK *S)
{
    if(S->top == -1)
	return(1);
    else
	return(0);
}
