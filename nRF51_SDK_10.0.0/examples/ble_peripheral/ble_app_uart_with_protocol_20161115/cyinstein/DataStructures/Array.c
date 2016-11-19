/*=========================================================

    �ĤG�� �}�C

       2.2 �}�C���B��

          ArrayInsert()         ���J�`�I
          ArrayDelete()         �R���`�I
          ArrayCopy()           �}�C����
          PrintArray()          �L�X�}�C���e
          list1[ARRAY_SPACE]    �s��}�C��l���e
          list2[ARRAY_SPACE]    �s��}�C�B�⵲�G
          ARRAY_SPACE           �}�C�j�p
  =========================================================
*/
#include <stdio.h>

#define ARRAY_SPACE      8
void ArrayInsert(),ArrayDelete(),ArrayCopy(),PrintArray();
void main(void)
{
   
   int list1[ARRAY_SPACE]={5,25,11,26,39,18,120,50};
   int list2[ARRAY_SPACE],i,value,choose;
   int loopflag=1;

   /*clrscr();�M���ù�*/
   while (loopflag)
   {
     printf("\n (1) Insert,(2) Delete,(3) Copy");
     printf("\n �п�JChoose 1-3, �䥦�����} ( other key to escape) :");
     scanf("%d",&choose);               /*�N��ܵ��G�s�Jchoose*/
     switch(choose)             /*�̩ҿ�ܤ����G����*/
      {
        case 1:                 /*�߰ݨ䴡�J�I�μƭ�*/
                printf("�}�C��m�q0 �}�l( Starting from 0 ).\n");
                printf("��}�C ( The content of array ) :");
                PrintArray(list1,ARRAY_SPACE);
                printf("\n�п�J�����J���`�I��m ( Input Insert Position )=>");
                scanf("%d",&i);
                printf("�п�J�����J���ƭ� ( Input Insert value )=>");
                scanf("%d",&value);
                ArrayInsert(list1,ARRAY_SPACE,i,value);
                printf("���G��X ( Result )=>");
                PrintArray(list1,ARRAY_SPACE);
                break;
        case 2:                 /*�߰ݱ��R���I*/
                printf("�}�C��m�q0 �}�l( Starting from 0 ).\n");
                printf("��}�C( The content of array )  :");
                PrintArray(list1,ARRAY_SPACE);
                printf("\n�п�J���R�����`�I��m ( Input Position )=>");
                scanf("%d",&i);
                ArrayDelete(list1,ARRAY_SPACE,i);
                printf("���G��X ( Result )=>");
                PrintArray(list1,ARRAY_SPACE);
                break;
        case 3:                 /*����ArrayCopy*/
                printf("�}�C��m�q0 �}�l( Starting from 0 ).\n");
                printf("��}�C( The content of array )  :");
                PrintArray(list1,ARRAY_SPACE);
                ArrayCopy(list2,list1,ARRAY_SPACE);
                printf("\n���G��X list2=>");
                PrintArray(list2,ARRAY_SPACE);
                break;
        default:
                printf("Good Bye!\n");
                loopflag=0;             /*�ﶵ���~,�{������*/
                break;
      }
   }
}

/* �blist �}�C(�j�p�� n ����)�W,�N value ���J��m i */

void ArrayInsert(int list[],unsigned int n,unsigned int i,int value)
{
   unsigned int counter = n-1;

   if(i<0 || i>=n)      return;
   for(;counter > i;counter--)
        list[counter] = list[counter-1];
   list[i] = value;
}

/* �b�����ӼƬ� n �� list �}�C,�R���� i �Ӥ��� */

void ArrayDelete(int list[],unsigned int n,unsigned int i)
{
   unsigned int counter = i;

   if(i < 0)    return;
   for(;counter < n-1;counter++)
        list[counter] = list[counter+1];
   list[n-1] = 0;
}

/* �N list2[] �ƻs��list1[],��̤����ӼƬҬ� n */

void ArrayCopy(int list1[],int list2[],unsigned int n)
{
   unsigned int counter;

   for(counter=0;counter < n;counter++)
        list1[counter] = list2[counter];
}

/*�N�}�C���e��ܦb�ù��W*/

void PrintArray(int list[],int n)
{
   int i;

   for(i=0;i < n;i++)
        printf("%d,",list[i]);
}