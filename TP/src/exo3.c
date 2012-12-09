#include <setjmp.h>
#include <stdio.h>
#include "asmMacros.h"

static int mul(int depth)
{
	//_SHOWSTACK();
	jmp_buf buf;
	if(!setjmp(buf))
	{
		int i;
		switch (scanf("%d", &i)) 
		{
			case EOF :
				return 1; /* neutral element */
			case 0 :

				return mul(depth+1); /* erroneous read */
			case 1 :
				if (i)
					return i * mul(depth+1);
				else
					longjmp(buf, 1);
					break;
			default:
				return 1;
	
			
		}
	}
	else
	{
		printf("A zero was entered and we returned to the old context\n");
		return 0;
	}
}

int main()
{

	int product;
	printf("A list of int, please\n");
	product = mul(0);
	printf("product = %d\n", product);
}
