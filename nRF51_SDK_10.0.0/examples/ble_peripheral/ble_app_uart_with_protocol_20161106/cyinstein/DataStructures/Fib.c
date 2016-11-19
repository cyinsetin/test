/*=========================================================

      4.4 堆疊的應用
		4.4.3 遞迴
	 例4.1 Fibanacci數列

	  Fib() 	Fibanacci數列函式

   
  =========================================================
*/
#include<stdio.h>

void main(void)
{
    int n;
    long Fib();

    /*clrscr();				清除螢幕*/
    printf("Please input a number less than 35 =>");
    scanf("%d",&n);                     /*讀入數字*/

    if(n > 34)			
    	printf("The calculation time will be too long to wait...");
	else if(n < 0)				/*小於零之數不合法*/
    	printf("input error,number must > 0");
	else
		printf("Fib(%d) = %d  ",n,Fib(n));
}

/*Fibanacci數列計算函式*/
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