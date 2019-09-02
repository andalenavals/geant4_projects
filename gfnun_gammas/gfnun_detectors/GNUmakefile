# -------------------------------------------------------
# PICompton GNUmakefile
# -------------------------------------------------------
CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

name := PICompton
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

#Add ROOT options for compilation
#CPPFLAGS += `root-config --cflags`
#LDFLAGS  += `root-config --libs`

# ROOT support # example /exended/analysis/AnaEx02
CPPFLAGS += -I$(shell root-config --incdir) 
LDFLAGS  += $(shell root-config --libs)
#EXTRALIBS = $(shell root-config --glibs)

CPPFLAGS += "-g"

include $(G4INSTALL)/config/binmake.gmk

#visclean:#
#	rm -f g4*.prim g4*.eps g4*.wrl
#	rm -f .DAWN_*
