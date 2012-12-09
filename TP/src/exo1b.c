#include <setjmp.h>
#include <stdio.h>
int i = 0;
jmp_buf buf;
int
main()
{
	int j = 0;
	if (setjmp(buf))
	{	
		printf(" zizi? i = %d ,j =%d\n",i,j);
		for (; j<5; j++)
		{
			printf("up ,i = %d ,j =%d\n",i,j);
			i++;
		}
	}
	else {
		for (; j<5; j++)
		{
			printf("down ,i = %d ,j =%d\n",i,j);
			i--;
		}
		
		longjmp(buf,~0);
	}
	printf("%d\n", i );
} 
