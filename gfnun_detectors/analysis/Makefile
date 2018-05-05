#/****************************************************************/
#/****************************************************************/
#/***** File created by Pico (L.G. Sarmiento) for the TASCA  *****/
#/***** group @ GSI. August 2011.                            *****/
#/***** Contact: lgsarmientop@unal.edu.co                    *****/
#/****************************************************************/
#/****************************************************************/

DESTDIR = $(PWD)
CC      = g++
CFLAGS	= -g
LD	= $(CC)
LDFLAGS = 
RM	= rm

ifdef ROOTSYS
INCLUDES = -I$(ROOTSYS)/include
else
INCLUDES = -I/usr/include/root
endif

LIBDIRS = `root-config --glibs`
LDLIBS  = `root-config --libs`

PROGRAM = PK_read
MAIN    = main.cxx
SRCS    = $(shell find . -name *.C)
OBJS    = ${SRCS:.C=.o}

# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .C .o
# define a suffix rule for .C -> .o
.C.o :
	$(CC) $(INCLUDES) $(LDLIBS) $(LIBDIRS) -c -o $@ $<

# default target by convention is ``all''
all : $(PROGRAM)

$(PROGRAM) : $(OBJS) $(MAIN)
	$(LD) $(INCLUDES) $(LDLIBS) $(LIBDIRS) -o $@ $(OBJS) $(MAIN)

$(OBJS) : $(SRCS) ${SRCS:.C=.h}

.PHONY: clean help

help :
	@ echo "Makefile to compile ROOT/C++ routines instead of using CINT"

clean :
	-$(RM) -f $(PROGRAM) $(OBJS)