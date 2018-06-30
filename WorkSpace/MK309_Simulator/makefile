#
#	Makefile for MK309_Simulator
#

# Set compiler to be gcc 
CC = gcc

# compile flags for gcc
CFLAGS = -g `pkg-config gtk+-3.0 --cflags` `pkg-config epoxy --cflags`

LIBS = `pkg-config gtk+-3.0 --libs` `pkg-config epoxy --libs`

# flags that must be set for the linker
LDFLAGS = -lm -lglut -lGLU -lGL -lGLEW -rdynamic 

# list of files that are apart of my project
#
#	FIX
SOURCES = MK309_Simulator.c MK309_GUI.c OwnshipCalc.c TacticalGraphics.c TacticalSymbols.c TargetCalc.c TMA_Claculations.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = MK309

# build the target first
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^  $(LDFLAGS)

# The phony target "clean" is used to remove all compiled object files.
.PHONY: clean

clean: rm -rf $(TARGET) $(OBJECTS) core
#SRC = $(wldcard *.c)
#TAR = $(SRC:.c=.markdown)

#.PHONY: all clean
#all: $(TAR)
#	%.markdown:%.c
#		pandoc -o $< $@



#LIBS = \
	$(shell pkg-config gtk+-3.0 --libs) \
	$(shell pkg-config epoxy --libs)
