/*=========================================================

      4.4 ���|������
		4.4.3 ���j
	 �Q�λ��j��@�������

	  fact()		���j�������禡

  =========================================================
*/

#include<stdio.h>

void main(void)
{
    int n;
    long fact();

    /*clrscr();				/*�M���ù�*/
    printf("Please input number =>");
    scanf("%d",&n);                     /*Ū�J�Ʀr*/

    if(n > 12)				/*�]�j��12�������W�X */
    {					/*long �ү��ܤ��d��*/
		printf("answer is too large ,end");
	}
	else if(n < 0)				/*�p��s���Ƥ��X�k*/
		printf("input error,number must > 0");
	else
		printf("%d! = %ld  ",n,fact(n));
}

long fact(int n)
{
    long f;
    if(n == 0)
		return(1);
    f=fact(n-1);
    return(n*f);
}
