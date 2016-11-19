/*=========================================================

       6.4 �G���𪺫إ�
       6.5 �G���𪺨��X

          *crt_bt()             �G����إߨ禡
          preorder()            �e�Ǩ��X�禡
          inorder()             ���Ǩ��X�禡
          postorder()           ��Ǩ��X�禡
          visit()               ���X�`�I
          visit2()              ���X�`�I���K�N�O�������^
                                ���ҧQ�Ϋ�ǫ��X�ɤ@�@�N
                                �l��ҥe���O�������^,�H�K
                                ���

       �Ƶ�:����ɤ@�@��J�`�I���c���,���Ϋ�<ENTER>
            �]����󥿸��!!!

  =========================================================
*/

#include <stdio.h>

typedef struct Tnode
        {
                struct Tnode *left_c;
                char    data;
                struct Tnode *right_c;
        }NODE;
void main(void)
{
    NODE *crt_bt(),*tree;
    void preorder(),inorder(),postorder();

    /*clrscr();*/
    printf("��J�`�I���,���Ϋ�<ENTER>\n");
    tree=crt_bt();

    printf("\n�e�Ǩ��X���G=> ");
    preorder(tree);

    printf("\n���Ǩ��X���G=> ");
    inorder(tree);

    printf("\n��Ǩ��X���G=> ");
    postorder(tree);
}

NODE *crt_bt()
{
    NODE *ptr;
    char data;
    data=getche();
    if(data == '0')
        return (NULL);
    ptr=malloc(sizeof(NODE));
    ptr->data=data;
    ptr->left_c=crt_bt();
    ptr->right_c=crt_bt();
    return(ptr);
}

void visit(NODE *p)
{
    printf("%c",p->data);
}

/*��Ǩ��X��*/
void visit2(NODE *p)
{
    printf("%c",p->data);
    free(p);
}

void preorder(NODE *p)
{
    if(p != NULL)
    {
        visit(p);
        preorder(p->left_c);
        preorder(p->right_c);
    }
}

void inorder(NODE *p)
{
    if(p != NULL)
    {
        inorder(p->left_c);
        visit(p);
        inorder(p->right_c);
    }
}

void postorder(NODE *p)
{
   if(p != NULL)
   {
        postorder(p->left_c);
        postorder(p->right_c);
        visit2(p);
   }
}
