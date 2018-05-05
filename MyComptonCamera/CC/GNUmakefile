# -------------------------------------------------------
# ComptonCameraFCF GNUmakefile
# -------------------------------------------------------
name := ComptonCameraFCF
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk
include $(G4INSTALL)/config/binmake.gmk

# ROOT support 
#ifdef G4ANALYSIS_USE_ROOT   # If we have ROOT
#CPPFLAGS += -DG4ANALYSIS_USE_ROOT
CPPFLAGS += -I$(shell root-config --incdir) 
#CPPFLAGS += $(shell root-config --cflags)
EXTRALIBS += $(shell root-config --glibs) #-lReflex -lCintex

#CXX = $(shell root-config --cxx)
#LD = $(shell root-config --ld)
#CPPFLAGS += -I$(shell root-config --incdir) 
#LDFLAGS  += $(shell root-config --libs)
#CPPFLAGS += "-g"
#endif

#visclean:#
#	rm -f g4*.prim g4*.eps g4*.wrl
#	rm -f .DAWN_*

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
