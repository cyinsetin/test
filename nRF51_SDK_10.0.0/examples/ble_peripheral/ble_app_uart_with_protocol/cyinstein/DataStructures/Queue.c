/*=========================================================

       4.6 用陣列結構實作佇列

	  enqueue()	將資料自後端加入
	  dequeue()	將資料自前端刪除

  =========================================================
*/

#include <stdio.h>

#define MAX_ITEM	7

typedef struct stk
	{
	   int item[MAX_ITEM];        /*資料欄位*/
	   int	front;
	   int	rear;
	}QUEUE;
QUEUE q;

void PrintQueue();
int  enqueue();
int  dequeue();

void main(void)
{
   int choose,LoopFlag=1;
   int data;

   q.front=0;
   q.rear=0;
   /*clrscr();*/
   while(LoopFlag)
   {
	printf("\n佇列內容由前到後:");
	PrintQueue(q);
	printf("\n(1)從佇列後端放資料\n(2)從佇列前端取資料\n(0)結束=>");
	scanf("%d",&choose);

	switch(choose)
	{
	case 0:
		LoopFlag=0;			/*結束程式*/
		break;
	case 1:
		printf("請輸入欲放入之資料=>");
		scanf("\n%d",&data);
		if(enqueue(data) == 0)
			printf("佇列已滿,enqueue失敗");
		break;
	case 2:
		if(dequeue(&data) == 0)
			printf("佇列已空,dequeue失敗");
		else
			printf("取出之資料為=>%d",data);
		break;
	default:
		printf("選項錯誤");
	}
   }


}


void PrintQueue(QUEUE q)
{
   int i;
   if (q.front == q.rear)
       return; /* queue empty */
   for (i=(q.front+1)%MAX_ITEM; i != q.rear ;
		i= (i+1)%MAX_ITEM )
     printf("%d ",q.item[i]);
   printf("%d ",q.item[q.rear]);
}

int enqueue (char x)
{
    if((q.rear+1)%MAX_ITEM == q.front)
	return (0);
    q.rear=(q.rear+1)% MAX_ITEM;
    q.item [q.rear]=x;
    return (1);
}

int dequeue (char *xptr)
{
    if (q.front == q.rear)
	return(0);
    q.front = (q.front+1) % MAX_ITEM;
    *xptr = q.item[q.front];
    return 1;
}
