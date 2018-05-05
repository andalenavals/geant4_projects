# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

name := Neutrons
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

#Add ROOT options for compilation
#CPPFLAGS += `root-config --cflags`
#LDFLAGS += `root-config --glibs`
CPPFLAGS += $(shell root-config --cflags)
LDFLAGS  += $(shell root-config --libs)




CPPFLAGS += "-g"
#CPPFLAGS += -g

include $(G4INSTALL)/config/binmake.gmk

#########################################
#TARGET = roottest

#SRC = Neutrons_GFNUN.cc

#OBJ = $(SRC:.cc=.o)

#all : $(TARGET)

##$(TARGET) : $(OBJ)
#	$(LD) $(CPPFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

#%.o : %.cpp
#	$(CXX) $(CPPFLAGS) -o $@ -c $<

#clean :
#	rm -f *.o $(TARGET) *~

### OLD:: Angel

#name := neutronesSand_a4

#G4TARGET := $(name)
#G4EXLIB := true

#ifndef G4INSTALL
#  G4INSTALL = ../../..
#endif

#.PHONY: all
#all: lib bin

#include $(G4INSTALL)/config/binmake.gmk
