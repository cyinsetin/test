/*=========================================================

    第三章:
       3.2:鏈結串列基本運算(以陣列實作)

	  ListInsert()		插入節點
	  ListDelete()		刪除節點
	  GetPreNode()		傳回指定節點之前一個節點
	  PrintList()		顯示節點內容
	  find_node_num()	找尋資料所在節點
	  GetFreeNode()		找尋空節點
	  table[MAX_NODE]	陣列鏈結串列
	  MAX_NODE		串列空間數目(陣列大小)

  =========================================================
*/

#include <stdio.h>
#include <string.h>

#define MAX_NODE	9

typedef struct ListNode
	{
	   char data[8];	/*資料欄位*/
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
	printf("原串列 :\n");
	PrintList(table);
	/*clrscr();*/
	while (loopflag)
	{
	
		printf("(1)插入節點,(2)刪除節點, (其它鍵) 離開 =>");
		scanf("%d",&choice);
		switch(choice)
		{
		case 1:
			printf("\n請輸入欲插入之資料=>");
			scanf("%s",NewData);
			strupr(NewData);
			printf("請輸入欲插入那一資料之後 ( 輸入HEAD代表成為第一個資料 ) =>");
			scanf("%s",OldData);
			strupr(OldData);
			if ( strcmp(OldData,"HEAD")==0)
				i=0;
			else if((i=find_node_num(table,OldData)) == -1)
			{
				printf("找不到資料,插入失敗\n");
				break;
			}

			if( (i>=0) && (ListInsert(table,NewData,i) == 0))
			{
				printf("串列已滿,插入失敗\n");
				break;
			}
			printf("插入 %s 之後的串列 :\n",NewData);
			PrintList(table);
			break;
		case 2:
			printf("\n請輸入欲刪除那一資料=>");
			scanf("%s",OldData);
			strupr(OldData);
			if((i=find_node_num(table,OldData)) == -1)
			{
				printf("找不到資料,刪除失敗\n");
				break;
			}

			ListDelete(table,i);
			printf("刪除 %s 之後的串列 :\n",OldData);
			PrintList(table);
			break;
		default:
			loopflag=0;
		}
	}
}

/*將資料插入串列中*/
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

/*刪除串列中一筆資料*/
void ListDelete(NODE table[],int i)
{
   int Prenode;

   Prenode=GetPreNode(table,i);

   table[Prenode].next=table[i].next;
   table[i].next=-1;
}

/*尋找欲刪除處之上一個節點*/
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

/*顯示串列所有內容*/
void PrintList(NODE table[])
{
   int i;

   for(i = table[0].next;i != 0;i = table[i].next)
	printf(" %s\n",table[i].data);
}

/*找尋資料之所在節點*/
int find_node_num(NODE table[],char *OldData)
{
   int i;

   for(i=table[0].next; i!=0 && (strcmp(table[i].data,OldData))!=0;
       i=table[i].next);
   if ( i==0 )
		return -1;
   return i;
}

/*找空節點*/
unsigned GetFreeNode(NODE table[])
{
   unsigned i=1;

   for(;i <= MAX_NODE ;i ++)
	if(table[i].next == -1)
		return i;

   return 0;
}