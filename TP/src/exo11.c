#include <stdio.h>
#include <stdlib.h>
#include "asmMacros.h"
#include "hw.h"


typedef void (func_t)(void *);
struct ctx_s
{
  unsigned int ebp_b;
  unsigned int esp_b;
  func_t* func;
  void* parameters;
  void* stack;
  short launched;
  struct ctx_s * suivant;
};




struct ctx_s * liste;
struct ctx_s * current_ctx;


int create_ctx(int stack_size, func_t f, void *args)
{
  struct ctx_s * ctx = malloc(sizeof(struct ctx_s));
  ctx->stack = malloc(stack_size) + stack_size;
  ctx->ebp_b = ctx->stack;
  ctx->esp_b = ctx->stack;
  printf("contexte initialisé, ebp: %04x, esp: %04x\n", ctx->ebp_b, ctx->esp_b);
  ctx->launched = 0;
  ctx->func = f;
  ctx->parameters = args;
  ctx->suivant=0;

  if(liste!=NULL)
  {
	struct ctx_s  * third = liste->suivant;

	if(third!=0)
	{
		
		ctx->suivant=third;
		liste->suivant=ctx;
	}
	else
	{
		ctx->suivant=liste;
		liste->suivant=ctx;
	}
  }
  else
  {
	liste=ctx;
	ctx->suivant=ctx; 
  }
  return 0;
}


void yield()
{
	switch_to_ctx(liste->suivant);
}
void switch_to_ctx(struct ctx_s * suivant)
{
	if (current_ctx) 
	{
		getEBP(current_ctx->ebp_b);
		getESP(current_ctx->esp_b);
	}
	current_ctx = suivant;
	liste = current_ctx;
	static short launched =0;
	static func_t * pf;
	static void* params;
	pf = current_ctx->func;
	params = current_ctx->parameters;

	if(!current_ctx->launched)
	{
		current_ctx->launched=1;
		launched =1;
	}
  	setESP(current_ctx->esp_b);
  	setEBP(current_ctx->ebp_b);
        irq_enable();
  	if(launched)
    	{
      		launched = 0;
    		pf(params);
	}
  	return 0;
}

void timer_handler()
{
	irq_disable();
	yield();
	irq_enable();
}

void start_scheduler()
{
	setup_irq(TIMER_IRQ, &timer_handler);
	start_hw();
	switch_to_ctx(liste);

}


void f_ping(void *args)
{
	while (1) {
		printf("A");
		printf("B");
		printf("C");
	}
}

void f_pong(void *args)
{
	while (1) {
		printf("1");
		printf("2");
	}
}

void f_pung(void *args)
{
	while (1) {
		printf("3");
		printf("4");
	}
}

int main(int argc, char *argv[])
{
	create_ctx(16384, f_ping, NULL);
	create_ctx(16384, f_pong, NULL);
	create_ctx(16384, f_pung, NULL);	
	start_scheduler();
	exit(EXIT_SUCCESS);
}
