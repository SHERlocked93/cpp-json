TARGET	= bin/dttest

CC    	= gcc
CXX		= g++
INC 	= -I./include -I./extlib

CFILE	= ./extlib/cJSON/*.c ./extlib/hsm/*.c ./extlib/easylogger/*.c
CPPFILE	= ./src/*.cpp ./tests/*.cpp
OBJ 	= $(addsuffix .o, $(basename $(wildcard $(CPPFILE))))
OBJ		+= $(addsuffix .o, $(basename $(wildcard $(CFILE))))
C_FLAGS 	= -Werror $(INC) -g
C_FLAGS	+= -DHSM_COLOR_ENABLE
C_FLAGS	+= -DHSM_DEBUG_EVT2STR=HSM_Evt2Str
CPP_FLAGS = $(C_FLAGS) -std=c++14
LDFLAGS = -lstdc++


.PHONY: all clean
all: clean compile

compile: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPP_FLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(C_FLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
