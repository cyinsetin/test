/*=========================================================

     �Ĥ@��
        ��1.5

        factor()       �D N ����
  =========================================================
*/

#include <stdio.h>
long factor();

void main(void)
{
    int n;
    long answer;

    /*clrscr();   */                        /*�M���ù�*/
    printf("Please input a number less than 12 =>");
    scanf("%d",&n);                     /*Ū�J�Ʀr*/

    if(n > 12)                          /*�]�j��12�������W�X */
                                       /*long �ү��ܤ��d��*/
        printf("The answer will be too large for a long integer.\n");
        
    else if(n < 0)                           /*�p��s���Ƥ��X�k*/
        printf("Input error,number must > 0");
    else
	{
		answer=factor(n);
		printf("%d! = %ld\n ",n,answer);
	}
}

        /*�����p��禡*/
long factor(int n)
{
    int i;
    long result;

    result=1;
    i=1;
    while(i <= n)
    {
        result=result*i;
        i++;
    }
    return result;
}
