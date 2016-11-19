/*=========================================================

      4.4 ���|������
		4.4.3 ���j
	 ��4.3 �e������D

	  hanoi()   �� n �ӽL�l,�q form �W,�g�� by �W,�h�� to �W

  =========================================================
*/
#include<stdio.h>

void main(void)
{
    int n;
    void hanoi();

    /*clrscr();				�M���ù�*/
    printf("Please input number =>");
    scanf("%d",&n);                     /*Ū�J�Ʀr*/

    if(n > 100) 			/*�]�j��24���ƦC�W�X */
    {					/*unsigned int �ү��ܤ��d��*/
	printf("answer is too large ,end");
	exit(1);
    }
    if(n < 0)				/*�p��s���Ƥ��X�k*/
    {
	printf("input error,number must > 0");
	exit(1);
    }
    hanoi(n,'A','B','C');
    printf("����\n");
}

/*�� n �ӽL�l,�q form �W,�g�� by �W,�h�� to �W*/
void hanoi(int n, char from, char by, char to)
{
    if(n > 0)
    {
		hanoi(n-1, from, to, by);
		printf("move no. %d  disk from '%c' to '%c' \n",n, from, to);
		hanoi(n-1, by, from, to);
    }
}
