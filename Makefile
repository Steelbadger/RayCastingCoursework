TARGET = main

CC = g++
INCPATH=-I./PS2Framework/include
CFLAGS = -g -Wall $(INCPATH) 
# For release change -g to -O3

.SUFFIXES: .cpp .o

OBJS = 	PS2Framework/src/primitives.o \
		PS2Framework/src/ps2maths.o \
		PS2Framework/src/ps2matrix4x4.o \
		PS2Framework/src/ps2vector4.o \
		PS2Framework/src/pad.o \
		PS2Framework/src/dma.o \
		PS2Framework/src/sps2wrap.o \
		PS2Framework/src/texture.o \
		PS2Framework/src/font.o \
		renderer.o \
		raycaster.o \
		vector2.o \
		level.o \
		mob.o \
		mobmanager.o \
		animatedsprite.o \
		gun.o \
		playstate.o \
		startupstate.o \
		menustate.o \
		texturemanager.o \
		MyPS2Application.o \
		main.o

LIBS = -lm -lsps2util

LIBPATH=-L./

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBPATH) $(LIBS)

.cpp.o:
	$(CC) -c $<  $(CFLAGS) -o $@
	
clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f depend.mk
	
submission: $(TARGET)
	rm -f $(OBJS)
	rm -f depend.mk
	
run: $(TARGET)
	./$(TARGET)


# create the dependancy file	
depend:
	$(CC) $(CFLAGS) -MM *.cpp > depend.mk

	
#include the dependencies into the build
-include depend.mk