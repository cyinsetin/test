/*=========================================================

    �ĤT��:
     3.5 ���V�쵲��C
		3.5.1 ���J�`�I
		3.5.2 �R���`�I

	  InsertAfter() ���J�`�I
	  DeleteNode()	�R���`�I
	  PrintLIst()	 �L�X�`�I���e
	  FreeAllNode() ����Ҧ��`�I���O����Ŷ�

  =========================================================
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct dlist_node
	{
	   struct dlist_node *left;
	   int	  data;
	   struct dlist_node *right;
	}Dnode;

int		InsertAfter(Dnode *,int);
int		DeleteNode(Dnode *);
void	FreeAllNode(Dnode *);
void	PrintList(Dnode *);

void main(void)
{
	int choose,i,insertdata,num;
	Dnode *p;
	Dnode *listA;
	FILE *fin;
	listA=(Dnode*)malloc(sizeof(Dnode));
	listA->left=listA;
	listA->right=listA;
	p=listA;

   if((fin=fopen("List.in","r")) == NULL)
		printf("�}�ɥ���,�����{�� (File can not be opened, program terminate.)");
   else	
   {
	fscanf(fin,"%d",&insertdata);
	while(!feof(fin))
	{
		InsertAfter(p,insertdata);
		p=p->right;
		fscanf(fin,"%d",&insertdata);
	}
	fclose(fin);
	while(1)
	{
		printf("\n�������V�쵲��C���e( Content of circular doubly linked list )=>\n");
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

			for(p=listA;p->right != listA;p=p->right);
			InsertAfter(p,insertdata);
			break;
		case 2:
			printf("�п�J�����J�����(Input new data )=>");
			scanf("%d",&insertdata);

			printf("�п�J�����J����m(New Position)=>");
			scanf("%d",&num);

			for(i=1,p=listA;i!=num && p->right!=listA;p=p->right,i++);
			if(p == NULL)
				printf("���J����( Insert Failed )");
			else if(InsertAfter(p,insertdata) == 0)
				printf("���J����( Insert Failed )");
			break;
		case 3:
			printf("�п�J���R������� ( data to be deleted )=>");
			scanf("%d",&num);

			for(i=0,p=listA->right;p!=listA && p->data!=num ;p=p->right,i++);
			if(p == listA)
				printf("����Ƥ��b��C��( the data is not in list )");
			else if(DeleteNode(p) == 0)
				printf("�R������( Delete Failed)");
			break;
		default:
			printf("�ﶵ���~ ( Wrong option )");
		}
    }
   }
}

int InsertAfter(Dnode *p,int value)
{
   Dnode *newnode;
   newnode=(Dnode*)malloc(sizeof(Dnode));
   if(newnode == NULL)
	return 0;
   newnode->data=value;
   newnode->left=p;
   newnode->right=p->right;
   p->right->left=newnode;
   p->right=newnode;
   return 1;
}

int DeleteNode(Dnode *p)
{
   if(p == NULL)
	return(0);
   p->left->right=p->right;
   p->right->left=p->left;
   free(p);
   return 1;
}

void PrintList(Dnode *head)
{
	Dnode *p=head;
   for(p=p->right;p != head;p=p->right)
	printf("%d ",p->data);
}

void FreeAllNode(Dnode *head)
{
   Dnode *CurrentNode=head->right,*NextNode;

   while(CurrentNode != head)
   {
		NextNode=CurrentNode->right;
		free(CurrentNode);
		CurrentNode=NextNode;
   }
   free(head);
}
