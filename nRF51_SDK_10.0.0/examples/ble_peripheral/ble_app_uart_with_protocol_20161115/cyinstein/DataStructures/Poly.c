/*=========================================================

    �ĤT��:
      3.7 �쵲��C������
		3.7.1 �h�������

	  InsertAfter() ���J�`�I
	  DeleteNode()	�R���`�I
	  Read_data()	�۫��w�ɮ�Ū�J���
	  ViewP()	�L�X�h����
	  FreeAllNode() ����Ҧ��`�I���O����Ŷ�

  =========================================================
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct plistnode
	{
	   float coef;			/*�Y��*/
	   int	 expo;			/*����*/
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
	printf("�}�ɥ���,�����{��");
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

   fscanf(f,"%f",&coef);        /*��ŪŪ�ݫY�Ƥέ���,�Y���F�ɧ�,�Y*/
   fscanf(f,"%d",&expo);        /*��Ū�J���\��~�إߦ��`�I,�_�h�|�y*/
   while(!feof(f))		/*���̫�@����Ʒ|�إߨ⦸*/
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