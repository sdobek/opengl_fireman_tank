#-----------------------------------------------------------------------
#  Look for "--->" for things that you need to change for your
#  particular configuration.
#-----------------------------------------------------------------------
#  This simple Makefile which compiles a single C/C++ program, in the
#  source file $TARGET.cpp or $TARGET.c (where the variable TARGET is
#  set below) assuming a Unix system for OpenGL, GLUT, and X-windows.
#  The directories for OpenGL and GLUT will need to be changed depending
#  on your system.
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# ---> Comment out all but the program you wish to compile here.
#-----------------------------------------------------------------------

TARGET = ZombieFireBalls
# TARGET = sample2cpp
# TARGET = sample1c
# TARGET = sample2c

#-----------------------------------------------------------------------
# ---> System/Platform-dependent settings (which you will need to set):
#	OGL_DIR		Directory where OpenGL branches off (OGL_DIR/GL)
#	OGL_LIBS	OpenGL library names
#	X_DIR		X11 lib directory (where libX11, libXext live)
# ---> USE THE DEFINITIONS BELOW FOR CSIC Linux Cluster Machines
#-----------------------------------------------------------------------
OGL_DIR = /usr
OGL_LIBS =  -lGLU -lGL -lglut
X_DIR = /usr/lib

#-----------------------------------------------------------------------
# Setting for C++ compilation
# ---> Set C++ or CC to your favorite C++ or C compiler
#	CC = g++ for C++ compilation
#	CC = gcc for C compilation
#-----------------------------------------------------------------------
C++ = g++
CC = gcc

#-----------------------------------------------------------------------
# ---> Set CFLAGS according your desired option (debug/performance)
#	-Wall		List all warnings (highly recommended)
#	-O3		Optimize
#	-g		Allow debugging
#	-lm		Math library
#-----------------------------------------------------------------------
# CFLAGS = -g -Wall				(for debugging)
# CFLAGS = -O3 -Wall				(for best performance)
CFLAGS = -O3 -Wall

#-----------------------------------------------------------------------
# --->	MISC_LIBS	Miscellaneous libraries that your program needs.
#			-lm for standard math library
#-----------------------------------------------------------------------
MISC_LIBS =  -lm

#-----------------------------------------------------------------------
#  System/Platform-dependent settings (should work for Linux systems):
#	OGL_LIB_DIR	OpenGL/Glut library directory (where libGL.so lives)
#	OGL_INC_DIR	OpenGL linclude directory (where GL/gl.h lives)
#	OGL_LFLAGS	Loader flags for OpenGL stuff
#	X_LFLAGS	Loader flags for X11 stuff
#	LD_FLAGS	All loader flags
#-----------------------------------------------------------------------
OGL_LIB_DIR  = $(OGL_DIR)/lib
OGL_INC_DIR  = $(OGL_DIR)/include

OGL_LFLAGS   = -L$(OGL_DIR)/lib -Wl,-rpath=/usr/local/freeglut/lib $(OGL_LIBS)
INC_FLAGS = -I$(OGL_INC_DIR)
LD_FLAGS = $(OGL_LFLAGS) $(X_LFLAGS) $(MISC_LIBS)
X_LFLAGS = -L$(X_DIR) -lX11 -lXext -lXmu -lXi 

#-----------------------------------------------------------------------
# Compilation
#	This is the simplest form assuming a single source file.
#	If the file name is foo.cpp, set TARGET = foo, and the
#	object file will be put in foo.o, and the executable will
#	be put in foo.
#-----------------------------------------------------------------------

default: $(TARGET)

#-----------------------------------------------------------------------
# Use this for C++ files 
#-----------------------------------------------------------------------
ZombieFireBalls.o: ZombieFireBalls.cpp
	$(C++) -c $(CFLAGS) $(INC_FLAGS) ZombieFireBalls.cpp

ZombieFireBalls: ZombieFireBalls.o
	$(C++) $(CFLAGS) ZombieFireBalls.o -o ZombieFireBalls $(LD_FLAGS)

# ---> ADD THE FOLLOWING IF YOU HAVE ADDITIONAL SOURCE FILES, xxx.cpp.
# xxx.o: xxx.cpp
# 	$(C++) -c $(CFLAGS) $(INC_FLAGS) xxx.cpp

#-----------------------------------------------------------------------
#  Clean-up
#	make clean		remove .o and core files
#	make realclean		also removes executable
#-----------------------------------------------------------------------
clean:
	rm -f *.o core

realclean: clean
	rm -f $(TARGET)
