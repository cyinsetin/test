/*=========================================================

    �ĤT��:
       3.2:�쵲��C�򥻹B��(�H�}�C��@)

	  ListInsert()		���J�`�I
	  ListDelete()		�R���`�I
	  GetPreNode()		�Ǧ^���w�`�I���e�@�Ӹ`�I
	  PrintList()		��ܸ`�I���e
	  find_node_num()	��M��ƩҦb�`�I
	  GetFreeNode()		��M�Ÿ`�I
	  table[MAX_NODE]	�}�C�쵲��C
	  MAX_NODE		��C�Ŷ��ƥ�(�}�C�j�p)

  =========================================================
*/

#include <stdio.h>
#include <string.h>

#define MAX_NODE	9

typedef struct ListNode
	{
	   char data[8];	/*������*/
	   int	next;
	}NODE;
NODE table[MAX_NODE]={{"",3},{"USA",6},{"",-1},
			{"ROC",1},{"",-1},{"",-1},
		    {"UK",7},{"USSR",0},{"",-1}};
int		ListInsert();
int		GetPreNode();
int		find_node_num();
void	ListDelete();
void	PrintList();
unsigned GetFreeNode();

void main(void)
{
	int choice=0,i,loopflag=1;
	char NewData[255],OldData[255];
	printf("���C :\n");
	PrintList(table);
	/*clrscr();*/
	while (loopflag)
	{
	
		printf("(1)���J�`�I,(2)�R���`�I, (�䥦��) ���} =>");
		scanf("%d",&choice);
		switch(choice)
		{
		case 1:
			printf("\n�п�J�����J�����=>");
			scanf("%s",NewData);
			strupr(NewData);
			printf("�п�J�����J���@��Ƥ��� ( ��JHEAD�N�����Ĥ@�Ӹ�� ) =>");
			scanf("%s",OldData);
			strupr(OldData);
			if ( strcmp(OldData,"HEAD")==0)
				i=0;
			else if((i=find_node_num(table,OldData)) == -1)
			{
				printf("�䤣����,���J����\n");
				break;
			}

			if( (i>=0) && (ListInsert(table,NewData,i) == 0))
			{
				printf("��C�w��,���J����\n");
				break;
			}
			printf("���J %s ���᪺��C :\n",NewData);
			PrintList(table);
			break;
		case 2:
			printf("\n�п�J���R�����@���=>");
			scanf("%s",OldData);
			strupr(OldData);
			if((i=find_node_num(table,OldData)) == -1)
			{
				printf("�䤣����,�R������\n");
				break;
			}

			ListDelete(table,i);
			printf("�R�� %s ���᪺��C :\n",OldData);
			PrintList(table);
			break;
		default:
			loopflag=0;
		}
	}
}

/*�N��ƴ��J��C��*/
int ListInsert(NODE table[],char d[],unsigned i)
{
   unsigned EmptyNode;

   EmptyNode=GetFreeNode(table);
   if (EmptyNode == 0)
	return 0;
   strcpy(table [EmptyNode].data,d);
   table [EmptyNode].next=table[i].next;
   table[i].next=EmptyNode;
   return 1;
}

/*�R����C���@�����*/
void ListDelete(NODE table[],int i)
{
   int Prenode;

   Prenode=GetPreNode(table,i);

   table[Prenode].next=table[i].next;
   table[i].next=-1;
}

/*�M����R���B���W�@�Ӹ`�I*/
int GetPreNode(NODE table[],int i)
{
   int prenode=0,node=table[prenode].next;
   while(node != i && node !=0 )
   {
	prenode=node;
	node=table[node].next;
   }
   return (prenode);
}

/*��ܦ�C�Ҧ����e*/
void PrintList(NODE table[])
{
   int i;

   for(i = table[0].next;i != 0;i = table[i].next)
	printf(" %s\n",table[i].data);
}

/*��M��Ƥ��Ҧb�`�I*/
int find_node_num(NODE table[],char *OldData)
{
   int i;

   for(i=table[0].next; i!=0 && (strcmp(table[i].data,OldData))!=0;
       i=table[i].next);
   if ( i==0 )
		return -1;
   return i;
}

/*��Ÿ`�I*/
unsigned GetFreeNode(NODE table[])
{
   unsigned i=1;

   for(;i <= MAX_NODE ;i ++)
	if(table[i].next == -1)
		return i;

   return 0;
}