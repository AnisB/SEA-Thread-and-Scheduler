ROOTDIR=../libhw
LIBNAME=hw
LIBFILE=$(ROOTDIR)/lib/lib$(LIBNAME).a

CC      = gcc
CFLAGS  = -Wall  -g -m32
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -l$(LIBNAME)

###------------------------------
### Main targets 
###------------------------------------------------------------
BINARIES= main exo1 exo1b exo2 exol exo3 exo4 exo5 exo7 exo8 exo10 exo11 exo12

all: $(BINARIES)

###------------------------------
### Main rules 
###------------------------------------------------------------

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCDIR)

%: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)


Makefile.depends:
	$(CC) -MM *.c $(INCDIR) > Makefile.depends


include Makefile.depends

main: main.o $(LIBFILE)

exo1: exo1.o $(LIBFILE)

exo1b: exo1b.o $(LIBFILE)

exol: exol.o $(LIBFILE)

exo3: exo3.o $(LIBFILE)

exo4: exo4.o $(LIBFILE)

exo5: exo5.o $(LIBFILE)

exo2: exo2.o $(LIBFILE)

exo8: exo8.o $(LIBFILE)


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean
clean:
	$(RM) *.o $(BINARIES) Makefile.depends
o:
	mv $(BINARIES) ../bin
	rm *.o

