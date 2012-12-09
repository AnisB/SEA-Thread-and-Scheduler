#include <setjmp.h>
#include <stdio.h>
int i = 0;
jmp_buf buf;
int
main()
{
	int j;
	if (setjmp(buf))
	{
		for (j=0; j<5; j++)
		{
			printf("Je passe dans la boucle up ,%d\n",i);
			i++;
		}
	}
	else 
	{
		for (j=0; j<5; j++)
		{
			printf("Je passe dans la boucle down ,%d\n",i);
			i--;
		}
		longjmp(buf,~0);
	}
	printf("%d\n", i );
	return 0;
}



