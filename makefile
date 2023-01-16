TARGET	= bin/dttest

CC    	= gcc
CXX		= g++
INC 	= -I./include -I./extlib

CFILE	= ./extlib/cJSON/*.c ./extlib/hsm/*.c
CPPFILE	= ./src/*.cpp ./tests/*.cpp
OBJ 	= $(addsuffix .o, $(basename $(wildcard $(CPPFILE))))
OBJ		+= $(addsuffix .o, $(basename $(wildcard $(CFILE))))
CFLAGS 	= -Werror $(INC) -g
CFLAGS	+= -DHSM_COLOR_ENABLE
CFLAGS	+= -DHSM_DEBUG_EVT2STR=HSM_Evt2Str
CPPFLAGS = $(CFLAGS) -std=c++14
LDFLAGS = -lstdc++


.PHONY: all clean
all: clean compile

compile: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
