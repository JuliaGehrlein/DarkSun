#If home PC
CC = g++
CXXFLAGS = -Wall -pipe -std=c++17

DIR=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))/include #
INCLUDES = -I/usr/include/gsl -I/usr/include/hdf5/serial -I$(DIR)

LIBS =
LINKS = -lSQuIDS -lnuSQuIDS -lhdf5 -lhdf5_hl -lgsl -lcblas

#If macbook crap
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
		LIBS = -L/usr/lib/x86_64-linux-gnu/hdf5/serial -L/usr/lib/x86_64-linux-gnu/
endif

OBJDIR=build
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(patsubst %.cpp,%.o,$(SOURCES))))

vpath %.cpp src particle_stuff
$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $^ -o $@

main: $(OBJS)
	$(CC) $(CXXFLAGS) $(INCLUDES) $(LIBS) $(OBJS) -o a $(LINKS)

.PHONY: clean

clean:
	rm -rf $(OBJS)
