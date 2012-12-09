#include <stdio.h>



// Displaying the stack  informations
#define _SHOWSTACK( ) \
	unsigned int espVar = 0; \
	unsigned int ebpVar=0;		\
	asm(		\
		"movl %%esp,%%eax;" 	\
		"movl %%ebp,%%ebx;"	\
		:"=a"(espVar),"=b"(ebpVar)	\
		:"a"(espVar),"b"(ebpVar)	\
	);\
	printf("esp=Ox4\n", espVar);	\
	printf("ebp=0x4\n", ebpVar);	\
	printf("Size of stack : 0x%x\n", ebpVar - espVar);


// Getting the value of the esp register
#define getESP(espVar)			\
	asm(		\
		"movl %%esp,%%eax;"	\
		:"=a" (espVar)		\
		:"a" (espVar)		\
	);				


// Getting the value of the ebp register
#define getEBP(ebpVar)			\
	asm(	\
		"movl %%ebp,%%eax;"	\
		:"=a" (ebpVar)		\
		:"a" (ebpVar)		\
	);				

// Setting the value of the esp register
#define setESP(espVar)			\
	asm(	\
		"movl %[input],%%esp;"	\
		: \
		:[input]"a" (espVar)		\
	);				

// Setting the value of the ebp register
#define setEBP(ebpVar)			\
	asm(	\
		"movl %[input],%%ebp;"	\
		: \
		:[input]"a" (ebpVar)		\
	);				

// Leaving a function
#define LEAVERET(returncode)			\
	asm("movl %0, %%eax" "\n" "leave" "\n" "ret"\
	:\
	: "r"(returnCode) \
	);
			





