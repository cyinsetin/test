/*=========================================================

       6.4 二元樹的建立
       6.5 二元樹的走訪

          *crt_bt()             二元樹建立函式
          preorder()            前序走訪函式
          inorder()             中序走訪函式
          postorder()           後序走訪函式
          visit()               拜訪節點
          visit2()              拜訪節點順便將記億體釋回
                                本例利用後序拜訪時一一將
                                子樹所占之記億體釋回,以免
                                當機

       備註:執行時一一輸入節點結構資料,不用按<ENTER>
            也不能更正資料!!!

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
    printf("輸入節點資料,不用按<ENTER>\n");
    tree=crt_bt();

    printf("\n前序走訪結果=> ");
    preorder(tree);

    printf("\n中序走訪結果=> ");
    inorder(tree);

    printf("\n後序走訪結果=> ");
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

/*後序走訪時*/
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