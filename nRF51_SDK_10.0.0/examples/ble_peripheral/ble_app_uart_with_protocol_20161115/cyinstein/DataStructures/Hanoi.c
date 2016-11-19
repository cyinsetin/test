/*=========================================================

      4.4 堆疊的應用
		4.4.3 遞迴
	 例4.3 河內塔問題

	  hanoi()   把 n 個盤子,從 form 柱,經由 by 柱,搬往 to 柱

  =========================================================
*/
#include<stdio.h>

void main(void)
{
    int n;
    void hanoi();

    /*clrscr();				清除螢幕*/
    printf("Please input number =>");
    scanf("%d",&n);                     /*讀入數字*/

    if(n > 100) 			/*因大於24之數列超出 */
    {					/*unsigned int 所能表示之範圍*/
	printf("answer is too large ,end");
	exit(1);
    }
    if(n < 0)				/*小於零之數不合法*/
    {
	printf("input error,number must > 0");
	exit(1);
    }
    hanoi(n,'A','B','C');
    printf("結束\n");
}

/*把 n 個盤子,從 form 柱,經由 by 柱,搬往 to 柱*/
void hanoi(int n, char from, char by, char to)
{
    if(n > 0)
    {
		hanoi(n-1, from, to, by);
		printf("move no. %d  disk from '%c' to '%c' \n",n, from, to);
		hanoi(n-1, by, from, to);
    }
}
