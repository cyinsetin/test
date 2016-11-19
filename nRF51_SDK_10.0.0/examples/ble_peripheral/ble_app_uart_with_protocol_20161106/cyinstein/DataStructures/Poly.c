/*=========================================================

    第三章:
      3.7 鏈結串列的應用
		3.7.1 多項式表示

	  InsertAfter() 插入節點
	  DeleteNode()	刪除節點
	  Read_data()	自指定檔案讀入資料
	  ViewP()	印出多項式
	  FreeAllNode() 釋放所有節點之記憶體空間

  =========================================================
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct plistnode
	{
	   float coef;			/*係數*/
	   int	 expo;			/*冪次*/
	   struct plistnode *next;
	}Pnode;
FILE *fin1,*fin2;
Pnode *AddTail(Pnode *,float,int);
Pnode	*PolyAdd();
void FreeAllNode(),ViewP(),Read_data();

void main(void)
{
   Pnode	*Pa,*Pb,*Pc;
 
   if((fin1=fopen("PolyA.in","r"))==NULL || (fin2=fopen("PolyB.in","r"))==NULL)
   {
	printf("開檔失敗,結束程式");
	exit(1);
   }

   Pa=(Pnode *)malloc(sizeof(Pnode));
   Pb=(Pnode *)malloc(sizeof(Pnode));

   Read_data(fin1,Pa);
   Read_data(fin2,Pb);

   Pc=PolyAdd(Pa,Pb);

   /*clrscr();*/
   ViewP(Pa);
   printf("\n+\n");
   ViewP(Pb);
   printf("\n=\n");
   ViewP(Pc);

   FreeAllNode(Pa);
   FreeAllNode(Pb);
   FreeAllNode(Pc);
}

Pnode *AddTail(Pnode *tail,float co,int ex)
{
   Pnode *p;
   p=(Pnode *)malloc(sizeof(Pnode));
   p->coef=co;
   p->expo=ex;
   p->next=NULL;
   tail->next=p;
   tail=p;
   return(tail);
}

Pnode *PolyAdd(Pnode *pa,Pnode *pb)
{
   Pnode *c,*ctail;
   c=(Pnode *)malloc(sizeof(Pnode));
   ctail=c;

   pa=pa->next;
   pb=pb->next;
   while(pa && pb)
   {
	if(pa->expo > pb->expo)
	{
		ctail=AddTail(ctail,pa->coef,pa->expo);
		pa=pa->next;
	}
	else if(pa->expo < pb->expo)
	{
		ctail=AddTail(ctail,pb->coef,pb->expo);
		pb=pb->next;
	}
	else
	{
		if((pa->coef + pb->coef) != 0)
			ctail=AddTail(ctail,pa->coef+pb->coef,pa->expo);
		pa=pa->next;
		pb=pb->next;
	}
   }
   while(pa)
   {
	ctail=AddTail(ctail,pa->coef,pa->expo);
	pa=pa->next;
   }
   while(pb)
   {
	ctail=AddTail(ctail,pb->coef,pb->expo);
	pb=pb->next;
   }
   return (c);
}
void Read_data(FILE *f,Pnode *Tail)
{
   int expo;
   float coef;

   fscanf(f,"%f",&coef);        /*先讀讀看係數及乘冪,若未達檔尾,即*/
   fscanf(f,"%d",&expo);        /*都讀入成功後才建立此節點,否則會造*/
   while(!feof(f))		/*成最後一筆資料會建立兩次*/
   {
	Tail=AddTail(Tail,coef,expo);
	fscanf(f,"%f",&coef);
	fscanf(f,"%d",&expo);
   }
}

void ViewP(Pnode *p)
{
	for(;p->next != NULL;p=p->next)
		printf("%2.3fx^%d+",p->next->coef,p->next->expo);
	printf("%2.3fx^%d\n",p->coef,p->expo);
   
}

void FreeAllNode(Pnode *head)
{
   Pnode *next_node;

   while(head->next != NULL)
   {
	next_node=head->next;
	free(head);
	head=next_node;
   }
   free(head);
}
