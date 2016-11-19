/*=========================================================

       4.6 �ΰ}�C���c��@��C

	  enqueue()	�N��Ʀ۫�ݥ[�J
	  dequeue()	�N��Ʀ۫e�ݧR��

  =========================================================
*/

#include <stdio.h>

#define MAX_ITEM	7

typedef struct stk
	{
	   int item[MAX_ITEM];        /*������*/
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
	printf("\n��C���e�ѫe���:");
	PrintQueue(q);
	printf("\n(1)�q��C��ݩ���\n(2)�q��C�e�ݨ����\n(0)����=>");
	scanf("%d",&choose);

	switch(choose)
	{
	case 0:
		LoopFlag=0;			/*�����{��*/
		break;
	case 1:
		printf("�п�J����J�����=>");
		scanf("\n%d",&data);
		if(enqueue(data) == 0)
			printf("��C�w��,enqueue����");
		break;
	case 2:
		if(dequeue(&data) == 0)
			printf("��C�w��,dequeue����");
		else
			printf("���X����Ƭ�=>%d",data);
		break;
	default:
		printf("�ﶵ���~");
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