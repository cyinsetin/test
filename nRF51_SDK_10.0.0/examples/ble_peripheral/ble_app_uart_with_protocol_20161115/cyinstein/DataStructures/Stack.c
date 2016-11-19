/*=========================================================

       4.2 �ΰ}�C���c��@���|

	  PUSH()	���J���|
	  is_full()	���լO�_�w��
	  POP() 	�۰��|�u�X���
	  is_empty()	���լO�_�w��

  =========================================================
*/

#include <stdio.h>

#define MAX_ITEM	9

typedef struct stk
	{
	   int item[MAX_ITEM];        /*������*/
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
	printf("\n ���|���e�ѤW��U : \n");
	PrintStack(&S);
	printf("\n(1)�ޢ��ָ��\n(2)�ޢݢ޸��\n(0)����=>");
	scanf("%d",&choose);

	switch(choose)
	{
	case 0:
		LoopFlag=0;			/*�����{��*/
		break;
	case 1:
		printf("�п�J���ޢ��֤����=>");
		scanf("\n%d",&data);
		if(PUSH(&S,data) == 0)
			printf("���|�w��,�ޢ��֥���");
		break;
	case 2:
		if(POP(&S,&data) == 0)
			printf("���|�w��,�ޢݢޥ���");
		else
			printf("POP �X : %d",data);
		break;
	default:
		printf("�ﶵ���~");
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