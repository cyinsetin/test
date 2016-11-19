/*=========================================================

      4.4 堆疊的應用
		4.4.3 遞迴
	 利用遞迴實作階乘函數

	  fact()		遞迴式階乘函式

  =========================================================
*/

#include<stdio.h>

void main(void)
{
    int n;
    long fact();

    /*clrscr();				/*清除螢幕*/
    printf("Please input number =>");
    scanf("%d",&n);                     /*讀入數字*/

    if(n > 12)				/*因大於12之階乘超出 */
    {					/*long 所能表示之範圍*/
		printf("answer is too large ,end");
	}
	else if(n < 0)				/*小於零之數不合法*/
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