/*=========================================================

       8.4 雜湊法
        8.4.2 解決碰撞的方法-- 分別鏈結法


          hash_init()           啟始表格函式
          hash_insert()         安排資料函式
          list_insert_tail()    碰撞時建立鏈結
          hash()                雜湊函數
          hash_srch()           在以雜湊編排過的表格中搜尋
          Free_All_Node()       將所有記憶空間釋回
          table[m]              資料表格陣列
          m                     資料數目(陣列大小)

  =========================================================
*/

#include <stdio.h>

#define         m       13
#define         EMPTY   -32768

typedef struct listnode
        {
                int key;
                struct listnode *next;
        }NODE;

NODE table[m];

void hash_init(),hash_insert(),Free_All_Node();
int  hash_srch();
void PrintList();
void PrintTable();

void main(void)
{
    int in_data,choose,addr;

    hash_init();

    while(1)
    {
        /*clrscr();*/
        printf("雜湊表內容:\n");
        PrintTable();
        printf("\n(1)加入資料\n(2)搜尋資料\n(0)結束=>");
        scanf("%d",&choose);

        switch(choose)
        {
                case 0:
                        Free_All_Node();
                        exit(0);                        /*結束程式*/
                case 1:
                        printf("請輸入欲加入之資料=>");
                        scanf("%d",&in_data);

                        hash_insert(in_data);
                        break;
                case 2:
                        printf("請輸入欲搜尋之資料=>");
                        scanf("%d",&in_data);

                        if(hash_srch(in_data) == -1)
                                printf("找不到資料\n");
                        else
                                printf("找到!!!\n");
                        printf("按任一鍵繼續");
                        getch();
                        break;
                default:
                        printf("選項錯誤");
        }
    }
}

/*雜湊函數*/
int hash(int key)
{
    return(key % m);
}


void hash_init(void)
{
    int i;
    for(i=0;i < m;i++)
        table[i].key=EMPTY;     /*代表空格*/
        table[i].next=NULL;
}


void hash_insert(int key)
{
    int list_insert_tail(),addr;

    addr=hash(key);             /*呼叫雜湊函數得位置*/
    if(table[addr].key != EMPTY)
        list_insert_tail(addr,key);
    else
        table[addr].key=key;
}


int list_insert_tail(int addr,int key)
{
   NODE *p,*new_node;
   p=&table[addr];
   while(p->next != NULL)
        p=p->next;
   if ((new_node=(NODE*)malloc(sizeof(NODE))) == NULL)
        return (0);
   new_node->key=key;
   new_node->next=NULL;
   p->next=new_node;
   return (1);
}

/*搜尋鍵值為key,成功傳回0,失敗傳回-1*/
int hash_srch(int key)
{
    int addr=hash(key);
    NODE *p;

    p=&table[addr];
    while(p->key != key)
        if(p->next == NULL)
                return(-1);
        else
                p=p->next;

    return(0);
}

void PrintList(int n)
{
   NODE  *p;

   p=&table[n];
   printf("table[%d]: ",n);
   if (p->key != EMPTY)
      printf("%d , ",p->key);

   while(p->next != NULL )
    {
      printf("%d , ",p->next->key);
      p=p->next;
    }
   printf("\n");
}

void PrintTable()
{
   int  i;

   for (i=0; i<m; i++)
     PrintList(i);
}

void Free_All_Node(void)
{
    NODE *p,*q;
    int i;

    for(i=0;i < m;i++)
    {
        p=&table[i];
        while(p->next != NULL)
        {
                q=p->next;
                free(p);
                p=q;
        }
    }
}
