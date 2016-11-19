/*=========================================================

       4.4 ���|������
	 4.4.2 �B�⦡�ഫ�ΨD��
	    ��������
	    ��Ǧ��D��

	  in_to_post()	��������
	  priority()	�P�_�B�⤸���u������
	  post_evaluate()��Ǧ��D��
	  evaluate()	�@���w�B�⤸�p��
	  PUSH()	���J���|
	  pop() 	�۰��|�u�X���

  =========================================================
*/


#define MAX_OP 5
#define operator(c) ((c=='+')||(c=='-')||(c=='*')||(c=='/'))?1:0
#define operands(c) ((c)>='a' && (c)<='z')?1:0
#define MAX_ITEM 100
typedef struct stk
	{
		int item [MAX_ITEM];
		int top;
	}STACK;
STACK S;

char op[MAX_OP]={'(','+','-','*','/'};
char prio[MAX_OP]={0,1,1,2,2};
char op_value[26]={10,30,6,9,8,11,12,13,7,8,22,56,77,76,
		   55,56,43,40,13,19,18,16,46,52,61,63};

void in_to_post();
void push();
void pop();
int  post_evaluate();

main()
{
    int i;
    char infix[MAX_ITEM];
    char postfix[MAX_ITEM];

    S.top=-1;

    /*clrscr();*/
    printf("�п�J���Ǧ�, �Ҧp (a+b)*c-d/e :");
    scanf("%s",infix);
    in_to_post(infix,postfix);
    printf("���Ǧ�=> %s \n��Ǧ�=> %s",infix,postfix);

    printf("\n�Y =>");
    for(i=0;i <26;i++)
		printf("%c=%d, ",(i+'a'),op_value[i]);
    printf("\n�W�������G= %d",post_evaluate(postfix));
}

void in_to_post(char *infix,char *postfix)
{
    int i,j,priority(),element;
    char token;
    void push(),pop();

    i=j=0;
    while((token=infix[i]) != '\0')
    {
	i=i+1;
	if(operands(token))
		postfix [j++]=token;
	else if(token == '(')
		push(token);
	else if(token == ')')
		while (S.top >= 0)
		{
			pop(&(int)element);
			if(element == '(')
				break;
			postfix [j++]=element;
		}
	else if(operator(token))
	{
		while(S.top >= 0)
		{
			element=S.item[S.top];
			if(priority(token) <= priority (element))
			{
				pop(&(int)element);
				postfix[j++]=element;
			}
			else
				break;
		}
		push(token);
	}
    }
    while(S.top >= 0)
    {
		pop(& element);
		postfix[j++]=element;
    }
    postfix[j]='\0';
}

void push(int x)
{
    if (S.top < MAX_ITEM-1)
    {
		S.top++;
		S.item[S.top]=x;
    }
}
void pop(int *x)
{
    if (S.top >= 0)
    {
		*x=S.item[S.top];
		S.top--;
    }
}

int priority(int c)
{
    int i=0;
	for(;i < MAX_OP;i++)
		if(op[i] == c)
			return(prio [i]);
    return(-1);
}


/*  =========================================================*/


int post_evaluate(char *postfix)
{
    char token;
    int evaluate(),op1,op2,result,i=0;

    while((token=postfix[i]) != '\0')
    {
	i=i+1;
	if(operands(token))
		push(op_value[token-'a']);
	else if(operator(token))
	{
		pop(&op2);
		pop(&op1);
		result=evaluate(token,op1,op2);
		push(result);
	}
    }
    pop (&result);
    return(result);
}
int evaluate(char optr,int op1,int op2)
{
    int result;
    switch(optr)
    {
	case '+':result=op1+op2;
		 break;
	case '-':result=op1-op2;
		 break;
	case '*':result=op1*op2;
		 break;
	case '/':result=op1/op2;
		 break;
    }
    return(result);
}
