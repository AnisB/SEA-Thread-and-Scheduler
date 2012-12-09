#include <stdio.h>
#include <stdlib.h>


typedef void (func_t)(void *);
struct ctx_s
{
	unsigned int ebp_b;
	unsigned int esp_b;
	func_t* func;
	void* parameters;
	void* stack;
	int launched;
};

struct ctx_s pctx;


int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *args)
{
  ctx->stack = malloc(stack_size) + stack_size;
  ctx->ebp_b = ctx->stack;
  ctx->esp_b = ctx->stack;
  printf("contexte initialisé, ebp: %04x, esp: %04x\n", ctx->ebp_b, ctx->esp_b);
  ctx->launched = 0;
  ctx->func = f;
  ctx->parameters = args;
  return 0;
}


typedef struct Type_t
{
	int a;
	int b;
	char c;
} T;


void f(void * paramC)
{
	T * param = (T*)paramC;
	param->a++;
	param->b++;
	printf(" Hi i m a %d, and i m b %d, and i m z %c0\n",param->a,param->b,param->c  );
}


int main()
{
	T  itsMeMario;
	itsMeMario.a=0;
	itsMeMario.b=1;
	itsMeMario.c='q';

	init_ctx(&pctx, 42,f,&itsMeMario);

	return 0;
}
