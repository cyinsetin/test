/*=========================================================

      6.7 �G���j�M��
        6.7.1 �G���j�M�𪺫إ�
        6.7.2 �G���j�M�𪺸`�I���J
        6.7.3 �G���j�M�𪺸`�I�j�M
        6.7.4 �G���j�M�𪺸`�I�R��

          *bst_create()         �إߤG���j�M��
          *bst_insert()         ���J�`�I
          *bst_search()         �j�M�`�I
          *bst_delete()         �R���`�I

  =========================================================
*/

#include <stdio.h>
#include <malloc.h>

typedef struct ttNODE
        {
                char left_thread;
                struct ttNODE *left_c;
                int data;
                char right_thread;
                struct ttNODE *right_c;
        }tNODE;
tNODE *listA;

tNODE *bst_create(),*bst_insert(),*bst_search(),*bst_delete();
void  inorder();
void  visit();

void main(void)
{
    tNODE *p;
    int choose,data,LoopFlag=1;

    listA=bst_create();

    while(LoopFlag)
    {
        printf("BST����: ");
        inorder(listA);
        printf("\n(1)���J���\n(2)�j�M���\n(3)�R�����\n(0)����=>");
        scanf("%d",&choose);

        switch(choose)
        {
                case 0:
						LoopFlag=0;
                        break;                        /*�����{��*/
                case 1:
                        printf("�п�J���إߤ����=>");
                        scanf("%d",&data);
                        p=malloc(sizeof(tNODE));
                        p->data=data;
                        listA=bst_insert(listA,p);
                        break;
                case 2:
                        printf("�п�J���j�M�����=>");
                        scanf("%d",&data);
                        if(bst_search(listA,data) == NULL)
                                printf("�䤣����");
                        else
                                printf("���!!!");
                        break;
                case 3:
                        printf("�п�J���R�������=>");
                        scanf("%d",&data);

                        if(bst_delete(listA,data) == NULL)
                                printf("�R����m���~����!!");
                        else
                                printf("�R������!!");
                        break;
                default:
                        printf("�ﶵ���~");
        }
     }
}

void visit(tNODE *p)
{
    printf("%d ",p->data);
}


void inorder(tNODE *p)
{
    if(p != NULL)
    {
        inorder(p->left_c);
        visit(p);
        inorder(p->right_c);
    }
}

tNODE *bst_insert(tNODE *t,tNODE *p)
{
    tNODE *r,*q;
    char direction;

    p->left_c=p->right_c=NULL;
    if(t == NULL)
        t=p;
    else
    {
        q=t;
        while(q != NULL)
        {
                if(p->data < q->data)
                {
                        direction=1;
                        r=q;
                        q=q->left_c;
                }
                else if(p->data > q->data)
                {
                        direction=0;
                        r=q;
                        q=q->right_c;
                }
                else
                        return(t);
        }
        if (direction==1)
                r->left_c=p;
        else
                r->right_c=p;
    }
    return(t);
}


tNODE *bst_create(void)
{
    FILE *filedata;
    int data;
    tNODE *t,*p;

    filedata=fopen("bst.dat","r");

    t=NULL;
    while(!feof(filedata))
    {
        fscanf(filedata,"%d",&data);
        p=malloc(sizeof(tNODE));
        p->data=data;
        t=bst_insert(t,p);
    }
    return(t);
}


tNODE *bst_search(tNODE *t,int key)
{
    while(t != NULL)
    {
        if(key == t->data)
                return(t);
        else if(key < t->data)
                t=t->left_c;
        else
                t=t->right_c;
    }
    return(NULL);
}


tNODE *bst_delete(tNODE *p,int key)
{
    int found=0,direction=0;
    tNODE *r,*q,*s,*t;

    r=q=s=NULL;
    t=p;

    while(p!=NULL && !found)
    {
        if(key == p->data)
                found=1;
        else if(key < p->data)
        {
                direction=1;
                r=p;
                p=p->left_c;
        }
        else
        {
                direction=0;
                r=p;
                p=p->right_c;
        }
    }
    if(p == NULL)
        return(NULL);
    if(r == NULL)
    {
        if(p->right_c == NULL)
                return(p->left_c);
        else if(p->left_c == NULL)
                return(p->right_c);
    }
    if(p->right_c == NULL)
    {
        if(direction == 1)
                r->left_c=p->left_c;
        else
                r->right_c=p->left_c;
    }
    else if(p->left_c == NULL)
    {
        if(direction == 1)
                r->left_c=p->right_c;
        else
                r->right_c=p->right_c;
    }
    else
    {
        s=p;
        q=p->left_c;
        while(q->right_c != NULL)
        {
                s=q;
                q=q->right_c;
        }
        p->data=q->data;
        if(p == s)
                s->left_c=q->left_c;
        else
                s->right_c=q->left_c;
    }
    return(t);
}
