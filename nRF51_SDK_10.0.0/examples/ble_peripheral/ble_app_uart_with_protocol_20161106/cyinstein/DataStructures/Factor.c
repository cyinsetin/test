/*=========================================================

     第一章
        例1.5

        factor()       求 N 階乘
  =========================================================
*/

#include <stdio.h>
long factor();

void main(void)
{
    int n;
    long answer;

    /*clrscr();   */                        /*清除螢幕*/
    printf("Please input a number less than 12 =>");
    scanf("%d",&n);                     /*讀入數字*/

    if(n > 12)                          /*因大於12之階乘超出 */
                                       /*long 所能表示之範圍*/
        printf("The answer will be too large for a long integer.\n");
        
    else if(n < 0)                           /*小於零之數不合法*/
        printf("Input error,number must > 0");
    else
	{
		answer=factor(n);
		printf("%d! = %ld\n ",n,answer);
	}
}

        /*階乘計算函式*/
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
