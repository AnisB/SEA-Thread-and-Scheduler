#include <setjmp.h>
#include <stdio.h>

static jmp_buf buf;
static int i = 0;
static int
cpt()
{
	int j = 0;
	if (setjmp(buf)) 
	{
		printf(" boum j=%d\n",j);
		for (j=0; j<5; j++)
		{
			printf(" up %d,%d\n",i,j);

			i++;
		}
		
	} 
	else 
	{
		for (j=0; j<5; j++)
		{
			printf(" down %d,%d\n",i,j);
			i--;
		}
	}
}
int
main()
{
	int np = 0 ;
	cpt();
	if (! np++)
	longjmp(buf,~0);
	printf("i = %d\n", i );
}