/*=========================================================

       4.3 ���쵲��C��@���|

	  PUSH()	���J���|
	  pop() 	�۰��|�u�X���

  =========================================================
*/

#include <stdio.h>

typedef struct listnode
	{
	   char data;			/*������*/
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
	printf("\n���|���e�ѤW��U : \n");
	PrintList(&TopOfS);
	printf("\n(1)�ޢ��ָ��\n(2)�ޢݢ޸��\n(0)����=>");
	scanf("%d",&choose);

	switch(choose)
	{
	case 0:
		exit(0);			/*�����{��*/
	case 1:
		printf("�п�J���ޢ��֤����=>");
		scanf("\n%c",&data);
		if(PUSH(&TopOfS,data) == 0)
			printf("�O����t�m�����\,�ޢ��֥���");
		break;
	case 2:
		if(pop(&TopOfS,&data) == 0)
			printf("���|�w��,�ޢݢޥ���");
		else
			printf("POP �X : %c",data);
		break;
	default:
		printf("�ﶵ���~");
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