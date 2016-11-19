/*=========================================================

    �ĤT��:
     3.3 �ʺA�t�m�`�I��@�쵲��C
		3.3.1 ���J�`�I
		3.3.2 �R���`�I
		3.3.3 �쵲��C���إ�

	  InsertAfter() ���J�`�I
	  DeleteNode()	�R���`�I
	  InsertTail()	���C���ݼW�@�s�`�I
	  GetPreNode()	��M���w�`�I���e�@�`�I
	  PrintList()	�L�X�`�I���e
	  FreeAllNode() ����Ҧ��`�I���O����Ŷ�

  =========================================================
*/

#include	<stdio.h>
#include	<stdlib.h>

typedef struct listnode
	{
	   int data;			/*������*/
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
	printf("\n��C���e( Content of list )=>");
	PrintList(listA);

	printf("\n(1)���[�`�I(Append new node)\n(2)���J�`�I(Insert new node)\n(3)�R���`�I(Delete node)\n(0)����(exit)=>");
	scanf("%d",&choose);

	switch(choose)
	{
		case 0:
			FreeAllNode(listA);		/*����Ҧ��`�I*/
			exit(0);			/*�����{��*/
		case 1:
			printf("�п�J�����[�����(Input new data )=>");
			scanf("%d",&insertdata);
			InsertTail(listA,insertdata);
			break;
		case 2:
			printf("�п�J�����J�����(Input new data)=>");
			scanf("%d",&insertdata);

			printf("�п�J�����J����m(New Position)=>");
			scanf("%d",&num);

			for(i=1,p=listA;i!=num && p!=NULL;p=p->next,i++);
			if(p == NULL)
				printf("���J���� ( Insert Failed )");
			else if(InsertAfter(p,insertdata) == 0)
				printf("���J���� ( Insert Failed )");
			break;
		case 3:
			printf("�п�J���R������� ( data to be deleted )=>");
			scanf("%d",&num);

			for(i=0,p=listA;p!=NULL&&p->data!=num;p=p->next,i++);
			if(p == NULL)
				printf("����Ƥ��b��C��( the data is not in list )");
			else if(DeleteNode(listA,p) == 0)
				printf("�R������( Delete Failed)");
			break;
		default:
			printf("�ﶵ���~ ( Wrong option ) !!");
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