/*=========================================================

      4.4 ���|������
		4.4.3 ���j
	 ��4.1 Fibanacci�ƦC

	  Fib() 	Fibanacci�ƦC�禡

   
  =========================================================
*/
#include<stdio.h>

void main(void)
{
    int n;
    long Fib();

    /*clrscr();				�M���ù�*/
    printf("Please input a number less than 35 =>");
    scanf("%d",&n);                     /*Ū�J�Ʀr*/

    if(n > 34)			
    	printf("The calculation time will be too long to wait...");
	else if(n < 0)				/*�p��s���Ƥ��X�k*/
    	printf("input error,number must > 0");
	else
		printf("Fib(%d) = %d  ",n,Fib(n));
}

/*Fibanacci�ƦC�p��禡*/
long Fib (int n)
{
   long i,j;
   if(n == 0)
	return(0);
   if(n == 1)
	return(1);
   i=Fib(n-1);
   j=Fib(n-2);
   return(i+j);
}
