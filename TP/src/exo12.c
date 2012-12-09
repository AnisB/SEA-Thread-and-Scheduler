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
  short state;
  struct ctx_s * suivant;
  struct ctx_s * attenteSuivante;
};

struct sem_s {
	signed int value;
	struct ctx_s *wait_list;
};


struct ctx_s * liste;
struct ctx_s * current_ctx;


int create_ctx(int stack_size, func_t f, void *args)
{
  struct ctx_s * ctx = malloc(sizeof(struct ctx_s));
  ctx->stack = malloc(stack_size) + stack_size;
  ctx->ebp_b = ctx->stack;
  ctx->esp_b = ctx->stack;
  ctx->state = 0;
  ctx->func = f;
  ctx->parameters = args;
  ctx->suivant=0;
  ctx->attenteSuivante=0;

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



void switch_to_ctx(struct ctx_s * suivant)
{
	if (current_ctx) 
	{
		getEBP(current_ctx->ebp_b);
		getESP(current_ctx->esp_b);
	}
	current_ctx = suivant;
	liste = current_ctx;
	static short state =0;
	static func_t * pf;
	static void* params;
	pf = current_ctx->func;
	params = current_ctx->parameters;

	if(current_ctx->state==0)
	{
		current_ctx->state=1;
		state =1;
	}
  	setESP(current_ctx->esp_b);
  	setEBP(current_ctx->ebp_b);
        irq_enable();
  	if(state==1)
    	{
      		state = 0;
    		pf(params);
	}
  	return 0;
}
void yield()
{
	struct ctx_s * follower =liste->suivant;
	while(follower->state==2)
	{
		follower=follower->suivant;
	}
	switch_to_ctx(follower);
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


void sem_down(struct sem_s * s)
{
	s->value --;

	if (s->value >= 0) 
	{

	}
	else 
	{
		struct ctx_s * parse=s->wait_list;
		if(s->wait_list==0)
		{
			current_ctx->state=2;
			s->wait_list=current_ctx;
		}
		else
		{
			while(parse->attenteSuivante!=0)
			{
				parse=parse->attenteSuivante;
			}
			parse->attenteSuivante=current_ctx;
		}	
		current_ctx->state=2;
		yield();
	}

}

void sem_up(struct sem_s *s)
{
	irq_disable();
	s->value++;
	if (s->wait_list!=0) 
	{
		struct ctx_s * toRelaunch=s->wait_list;
		s->wait_list->state=1;
		if(s->wait_list->attenteSuivante!=0)
		{
			s->wait_list=s->wait_list->attenteSuivante;
		}
		toRelaunch->attenteSuivante=0;
	}
	irq_enable();
}

void sem_init(struct sem_s * sem, unsigned int init_val)
{

	if (sem) {
		sem->value = init_val;
		sem->wait_list = NULL;
	} 
	else 
	{
		//Error
	}
}

#define N 100


typedef enum { HP,MAC,ASUS,ACER,TOSHIBA } Marque;

struct objet_t {
	Marque saMarque;
};

static struct objet_t *buffer_objets[N];
static int nb_objects = 0;

static struct sem_s *mutex, *vide, *plein;

void mettre_objet(struct objet_t *an_object)
{
	buffer_objets[nb_objects] = an_object;
	nb_objects++;
}

void retirer_objet(struct objet_t *an_object)
{
	struct objet_t *the_objet;
	the_objet = buffer_objets[nb_objects - 1];
	nb_objects--;
}

void utiliser_objet(struct objet_t *an_object)
{
	free(an_object);
}

void produire_objet(struct objet_t *an_object)
{
	int rand =clock()%5;
	an_object->saMarque = rand;
}

void producteur(void *arg)
{
	struct objet_t *objet = NULL;
	while (1) {
		objet = malloc(sizeof(struct objet_t));
		produire_objet(objet);	
		sem_down(vide);
		irq_disable();
		sem_down(mutex);	
		mettre_objet(objet);	
		sem_up(mutex);
		irq_enable();
		sem_up(plein);	
	}
}

void consommateur(void *arg)
{
	struct objet_t *objet = NULL;
	while (1) {

		sem_down(plein);	
		irq_disable();
		sem_down(mutex);	
		retirer_objet(objet);	
		sem_up(mutex);	
		irq_enable();
		sem_up(vide);	
		utiliser_objet(objet);
	}
}

int main(int argc, char *argv[])
{
	srand(clock());


	mutex = malloc(sizeof(struct sem_s));
	vide  = malloc(sizeof(struct sem_s));
	plein = malloc(sizeof(struct sem_s));
	
	sem_init(mutex, 1);	
	sem_init(vide, N);	
	sem_init(plein, 0);
	
	create_ctx(16384, producteur, NULL);
	create_ctx(16384, consommateur, NULL);


	start_scheduler();


	exit(EXIT_SUCCESS);
}
