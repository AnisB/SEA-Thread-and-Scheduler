#include <stdio.h>
#include <stdlib.h>
#include "asmMacros.h"

typedef void (func_t)(void *);
struct ctx_s
{
  unsigned int ebp_b;
  unsigned int esp_b;
  func_t* func;
  void* parameters;
  void* stack;
  short launched;
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

int switch_to_ctx(struct ctx_s *ctx)
{
  static struct ctx_s *last_ctx = 0;
  if (last_ctx!=0)
  {
	getESP(last_ctx->esp_b);
	getEBP(last_ctx->ebp_b);

  }
  last_ctx=ctx;
  static unsigned int esp=0, ebp=0;
  static short launched =0;
  static func_t * pf;
  static void* params;
  pf = ctx->func;
  params = ctx->parameters;

  if(!ctx->launched)
    {
      ctx->launched=1;
      launched =1;
    }
  setESP(ctx->esp_b);
  setEBP(ctx->ebp_b);
        
  if(launched)
    {
	launched = 0;
        pf(params);
    }
  return 0;
}
struct ctx_s ctx_ping;
struct ctx_s ctx_pong;
void f_ping(void *arg);
void f_pong(void *arg);

int main(int argc, char *argv[])
{
  init_ctx(&ctx_ping, 16384, f_ping, NULL);
  init_ctx(&ctx_pong, 16384, f_pong, NULL);
  switch_to_ctx(&ctx_ping);
  exit(EXIT_SUCCESS);
}

void f_ping(void *args)
{
  while(1)
    {
      printf("A") ;
      switch_to_ctx(&ctx_pong);
      printf("B") ;
      switch_to_ctx(&ctx_pong);
      printf("C") ;
      switch_to_ctx(&ctx_pong);
    }
}
void f_pong(void *args)
{
  while(1) 
    {
      printf("1") ;
      switch_to_ctx(&ctx_ping);
      printf("2") ;
      switch_to_ctx(&ctx_ping);
    } 
}
