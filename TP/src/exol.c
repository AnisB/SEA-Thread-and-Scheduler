
#include <setjmp.h>
#include <stdio.h>

static jmp_buf buf;
static int
a()
{
	int j = 0;
	if (setjmp(buf)) 
	{
		printf("Première fonction qui passe la deuxième  fois ,j = %d\n", j );
		j=5;	
	
	} 
	else 
	{
		printf("Première fonction qui passe la première fois ,j = %d\n", j );
	}
	return 0;
}

void b()
{
	long j = 99;
	long h = 99;
	long c = 99;
	long s = 99;
	printf("Deuxieme fonction qui écrase , j = %d%d%d%d\n", j,h,c,s );

}
int
main()
{
	a();
	b();
	longjmp(buf,~0);
	
}