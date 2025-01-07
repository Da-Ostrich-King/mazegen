NAME = mazegen

CC = g++
CCFLAGS = 
LINKFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
LINKFLAGSWINDOWS = -lgdi32 -lwinmm

SHELL := /bin/bash

BUILDDIR := build
SRCDIR := src

SOURCEFILES := $(shell ls $(SRCDIR)/*.cpp)
OBJECTFILES := $(shell echo $(SOURCEFILES) | sed 's/\.cpp/\.o/g' | sed 's/src/$(BUILDDIR)\/objects/g')

all: $(OBJECTFILES)
	mkdir -p $(BUILDDIR)
	$(CC) $(LINKFLAGS) $^ -o $(BUILDDIR)/$(NAME)

allWindows: $(OBJECTFILES)
	mkdir $(BUILDDIR) || true
	$(CC) $(LINKFLAGSWINDOWS) $(SRCDIR)/libraylib.a $^ -o $(BUILDDIR)/$(NAME)


$(OBJECTFILES): $(SOURCEFILES)
	@printf "Compiling $@\n"
	mkdir -p $(shell dirname "$@")
	test $@ -nt $(shell echo "$@" | sed 's/\.o/\.cpp/g' | sed 's/build\/objects/src/g') || g++ -c $(shell echo "$@" | sed 's/\.o/\.cpp/g' | sed 's/build\/objects/src/g') -o $@ $(FLAGS)
	@printf "Compiled $@\n\n"

