TARGET = main

CC = g++
INCPATH=-I./PS2Framework/include \
		-I./Classes/include
CFLAGS = -O3 -Wall $(INCPATH) 
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
		Classes/src/renderer.o \
		Classes/src/raycaster.o \
		Classes/src/vector2.o \
		Classes/src/level.o \
		Classes/src/mob.o \
		Classes/src/mobmanager.o \
		Classes/src/animatedsprite.o \
		Classes/src/gun.o \
		Classes/src/shotgun.o \
		Classes/src/playstate.o \
		Classes/src/startupstate.o \
		Classes/src/menustate.o \
		Classes/src/pausedstate.o \
		Classes/src/helpstate.o \
		Classes/src/gamewinstate.o \
		Classes/src/gamelosestate.o \
		Classes/src/optionsstate.o \
		Classes/src/player.o \
		Classes/src/timer.o \
		Classes/src/audio.o \
		Classes/src/audiomanager.o \
		Classes/src/texturemanager.o \
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