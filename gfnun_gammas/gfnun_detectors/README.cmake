######################################################################
######################################################################
####                                                              #### 
####    STEPS TO COMPILE AND RUN LuWuSiA using the CMAKE tool    #### 
####                                                              #### 
######################################################################
######################################################################

Make sure you really want to compile LuWuSiA using the CMAKE tool. If
your plan is use the GNUMakefile you are *NOT* using CMAKE but
autoconf tools. Starting from geant4.9.5 the default CMAKE is the
default compiling method and it is highly recomended to start using
it. For now there is a backwards compatibilty tool but in the next
major release such compatibility will be dropped.

To compile and run LuWuSiA using CMAKE this are the steps you need to
follow:

######################################################################
1) create a build director

$ mkdir LuWuSiA-build

######################################################################
2) from inside that directory run the CMAKE preparation command
pointing to your Geant4 installation location and LuWuSiA source code

$ cd LuWuSiA-build
$ cmake -DCMAKE_INSTALL_PREFIX={path_to_geant4}.cmake {path_to_PICOSPec_source/code}

if running it in nigromanta @ gfnun the line is just
$ cmake /home/pico/Lund/LuWuSiA_hg

since the machines use the package geant4 located in standard
locations.

NOTE:

 The ROOT ArchLinux package does not have the CMAKE procedure enabled
by default so the PATH and all ROOT requirements are included in the
CMakeLists.txt file

#####################################################################
3) make it!!!

$ make

it eveything goes as expected, in the running directory there should
be your new LuWuSiA executable.

#####################################################################
4) run it!!!
The macro files *SHOULD* be copied automatically by the
CMAKE process. So most likely you want to run directly the main macro
file THEmacro.mac. There are also other options that can be run by
themselves i.e. not meant to be called from other macro file, so make
sure you also check them out.

$ ./LuWuSiA macros/THEmacro.mac

#####################################################################
5) enjoy the results !!!

The output control can be performed via the main macro file
THEmacro.mac. The default output is to a ROOT Tree but an ASCII option
is also available. The name of the output files can be controlled from
the macro file equally easy.

If you prefer to work with the ROOT output you might want to consider
using Go4 for quick visualizations and easy superimposition of
spectrum for example from different ROOT files.