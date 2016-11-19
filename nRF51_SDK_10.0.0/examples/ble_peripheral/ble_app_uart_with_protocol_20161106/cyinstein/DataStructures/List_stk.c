/*=========================================================

       4.3 用鏈結串列實作堆疊

	  PUSH()	推入堆疊
	  pop() 	自堆疊彈出資料

  =========================================================
*/

#include <stdio.h>

typedef struct listnode
	{
	   char data;			/*資料欄位*/
	   struct listnode *next;
	}NODE;
NODE TopOfS;
int PUSH(),pop();
void PrintList();
void main(void)
{
   int choose;
   char data;

   /*clrscr();*/
   while(1)
   {
	printf("\n堆疊內容由上到下 : \n");
	PrintList(&TopOfS);
	printf("\n(1)ＰＵＳＨ資料\n(2)ＰＯＰ資料\n(0)結束=>");
	scanf("%d",&choose);

	switch(choose)
	{
	case 0:
		exit(0);			/*結束程式*/
	case 1:
		printf("請輸入欲ＰＵＳＨ之資料=>");
		scanf("\n%c",&data);
		if(PUSH(&TopOfS,data) == 0)
			printf("記憶體配置不成功,ＰＵＳＨ失敗");
		break;
	case 2:
		if(pop(&TopOfS,&data) == 0)
			printf("堆疊已空,ＰＯＰ失敗");
		else
			printf("POP 出 : %c",data);
		break;
	default:
		printf("選項錯誤");
	}
   }
}

void PrintList(NODE *S)
{
    NODE  *p;
    for (p=S->next; p!= NULL; p=p->next)
	printf("%3c",p->data);
}

int PUSH(NODE *S,int X)
{
    NODE *p;
    if((p=(NODE*)malloc(sizeof(NODE)))==NULL)
	return(0);
    p->data=X;
    p->next=S->next;
    S->next=p;
    return(1);
}
int pop(NODE *S,int *X)
{
    NODE *p=S->next;
    if(p == NULL)
	return(0);
    *X=p->data;
    S->next=p->next;
    free(p);
    return(1);
}
