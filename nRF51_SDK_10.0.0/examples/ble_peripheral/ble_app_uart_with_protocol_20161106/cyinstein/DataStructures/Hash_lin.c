/*=========================================================

       8.4 ����k
        8.4.2 �ѨM�I������k

          �u�ʱ����k

          hash_init()           �ҩl���禡
          hash_insert()         �w�Ƹ�ƨ禡
          hash()                ������
          hash_srch()           �b�H����s�ƹL����椤�j�M
          is_full()             ���ܰ}�C�Ŷ��O�_�w��
          PrintTable()          �L�X������
          table[m]              ������}�C
          m                     ���j�p

  =========================================================
*/

#include <stdio.h>

#define         m       13
#define         EMPTY   -32768

int table[m];
void hash_init();
void hash_insert();
int  hash_srch();
int  is_full();
void PrintTable();

void main(void)
{
    int in_data,addr;
    int choose;

    /*clrscr();*/
    hash_init();

    while(1)
    {
        printf("������e(e��Ů�):");
        PrintTable(table);
        printf("\n(1)�[�J���\n(2)�j�M���\n(0)����=>");
        scanf("%d",&choose);

        switch(choose)
        {
                case 0:
                        exit(0);                        /*�����{��*/
                case 1:
                        if(is_full())
                                printf("�}�C�w��\n");
                        else
                        {
                                printf("�п�J���[�J�����=>");
                                scanf("%d",&in_data);

                                hash_insert(in_data);
                        }
                        break;
                case 2:
                        printf("�п�J���j�M�����=>");
                        scanf("%d",&in_data);

                        if((addr=hash_srch(in_data)) == -1)
                                printf("�䤣����\n");
                        else
                                printf("��� %d ����� %d �Ӧ�m\n",in_data,addr);
                        break;
        }
    }
}

/*������*/
int hash(int key)
{
    return(key % m);
}


void hash_init(void)
{
    int i;
    for(i=0;i < m;i++)
        table[i]= EMPTY;        /*�N��Ů�*/
}


void hash_insert(int key)
{
    int addr=hash(key);         /*�I�s�����Ʊo��m*/
    while(table[addr] != EMPTY)
        addr=(addr+1) % m;      /*�~��A�qtable[0]�}�l*/
    table[addr]=key;
}

/*�j�M��Ȭ�key,���\�Ǧ^����m,���ѶǦ^-1*/
int hash_srch(int key)
{
    int addr;
    addr=hash(key);
    while(table[addr] != key)
    {
        addr=(addr+1)%m;
        if(table[addr]== EMPTY || addr==hash(key))/*�Y�O�Ů�ΤS�^����I*/
                return(-1);                    /*�h�Ǧ^����*/
    }
    return(addr);
}

int is_full(void)
{
    int i;

    for(i=0;i < m;i++)
        if(table[i] == EMPTY)
                return(0);
    return(1);
}

void PrintTable(int a[])
{
   int i;
   for (i=0; i<m; i++)
     if (a[i]!= EMPTY)
       printf("%d ",a[i]);
     else
       printf("e ");
 }
