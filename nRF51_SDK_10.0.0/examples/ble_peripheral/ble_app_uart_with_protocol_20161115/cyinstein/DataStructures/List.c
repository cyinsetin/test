/*=========================================================

    第三章:
     3.3 動態配置節點實作鏈結串列
		3.3.1 插入節點
		3.3.2 刪除節點
		3.3.3 鏈結串列的建立

	  InsertAfter() 插入節點
	  DeleteNode()	刪除節點
	  InsertTail()	於串列尾端增一新節點
	  GetPreNode()	找尋指定節點之前一節點
	  PrintList()	印出節點內容
	  FreeAllNode() 釋放所有節點之記憶體空間

  =========================================================
*/

#include	<stdio.h>
#include	<stdlib.h>

typedef struct listnode
	{
	   int data;			/*資料欄位*/
	   struct listnode *next;
	}Node;
Node *listA;

int InsertAfter(),DeleteNode(),InsertTail();
Node *GetPreNode();
void FreeAllNode(),PrintList();
void main(void)
{
   int choose,i,insertdata,num;
   Node *p;
   FILE *fin;
   /*clrscr();*/
   if((fin=fopen("list.in","r")) == NULL)
   {
	printf("File can not be opened, program terminate.");
	exit(1);
   }

   listA=(Node*)malloc(sizeof(Node));
   listA->next=NULL;

   fscanf(fin,"%d",&insertdata);
   while(!feof(fin))
   {
	InsertTail(listA,insertdata);
	fscanf(fin,"%d",&insertdata);
   }
   fclose(fin);
   while(1)
   {
	printf("\n串列內容( Content of list )=>");
	PrintList(listA);

	printf("\n(1)附加節點(Append new node)\n(2)插入節點(Insert new node)\n(3)刪除節點(Delete node)\n(0)結束(exit)=>");
	scanf("%d",&choose);

	switch(choose)
	{
		case 0:
			FreeAllNode(listA);		/*釋放所有節點*/
			exit(0);			/*結束程式*/
		case 1:
			printf("請輸入欲附加之資料(Input new data )=>");
			scanf("%d",&insertdata);
			InsertTail(listA,insertdata);
			break;
		case 2:
			printf("請輸入欲插入之資料(Input new data)=>");
			scanf("%d",&insertdata);

			printf("請輸入欲插入之位置(New Position)=>");
			scanf("%d",&num);

			for(i=1,p=listA;i!=num && p!=NULL;p=p->next,i++);
			if(p == NULL)
				printf("插入失敗 ( Insert Failed )");
			else if(InsertAfter(p,insertdata) == 0)
				printf("插入失敗 ( Insert Failed )");
			break;
		case 3:
			printf("請輸入欲刪除之資料 ( data to be deleted )=>");
			scanf("%d",&num);

			for(i=0,p=listA;p!=NULL&&p->data!=num;p=p->next,i++);
			if(p == NULL)
				printf("此資料不在串列中( the data is not in list )");
			else if(DeleteNode(listA,p) == 0)
				printf("刪除失敗( Delete Failed)");
			break;
		default:
			printf("選項錯誤 ( Wrong option ) !!");
	}
   }
}

int InsertAfter(Node *p,int value)
{
   Node *newnode;
   newnode=(Node*)malloc(sizeof(Node));
   if (newnode==NULL)
	return (0);
   newnode->data=value;
   newnode->next=p->next;
   p->next=newnode;
   return(1);
}

int DeleteNode(Node *head,Node *node)
{
   Node *GetPreNode(),*prenode;
   if (head==node)
	return 0;
   prenode=GetPreNode(head,node);
   if (prenode==NULL)
	return 0;
   prenode->next=node->next;
   free(node);
   return(1);
}

Node *GetPreNode(Node *head,Node *node)
{
   Node *p,*q;
   p=head;
   q=head->next;
   while((q!=NULL) && (q!=node))
   {
	p=q;
	q=q->next;
   }
   if(q != NULL)
	return(p);
   else
	return(NULL);
}

int InsertTail(Node *head,int value)
{
   Node *new_node,*p=head;
   if ((new_node=(Node*)malloc(sizeof(Node))) == NULL)
	return (0);
   new_node->data=value;
   new_node->next=NULL;
   while(p->next != NULL)
	p=p->next;
   p->next=new_node;
   return (1);
}

void PrintList(Node *p)
{
   for(p=p->next;p != NULL;p=p->next)
	printf("%d ",p->data);
}

void FreeAllNode(Node *head)
{
   Node *next_node;

   while(head != NULL)
   {
	next_node=head->next;
	free(head);
	head=next_node;
   }
}
