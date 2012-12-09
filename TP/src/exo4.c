#include <stdio.h>
#include "asmMacros.h"

struct ctx_s
{
	unsigned int ebp_b;
	unsigned int esp_b;
};
typedef int (func_t)(int);

struct ctx_s pctxS;
int returnCode;


int try(struct ctx_s *pctx, func_t *fs, int arg)
{

	unsigned int esp = 0, ebp = 0;
	getESP(esp);
	getEBP(ebp);
	pctx->ebp_b=ebp;
	pctx->esp_b=esp;
	
	return fs(arg);
}

int throw(struct ctx_s *pctx, int r)
{
	returnCode = r;
	setESP(pctx->esp_b);
	setEBP(pctx->ebp_b);
	LEAVERET(returnCode);

	// dead code
	return returnCode;
}


int f(int k)
{
	printf("C\n");
	k++;
	if(k==1)
		throw(&pctxS,3);
	return k;
}




int main()
{
	printf("A\n");
	printf("retour try:%u\n",try(&pctxS, f, 0));

}

