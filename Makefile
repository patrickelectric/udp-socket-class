# project directories
PRJDIR 		 := $(shell pwd)

OUTDIR       := $(PRJDIR)/bin

CODDIR       := $(PRJDIR)
LIBDIR       := $(CODDIR)/lib
SRCDIR       := $(CODDIR)/src

# C source files

#modules
C_SRC_1 = $(SRCDIR)/client.cpp
C_SRC_1+= $(LIBDIR)/*.c
C_SRC_2 = $(SRCDIR)/server.cpp
C_SRC_2+= $(LIBDIR)/*.c
C_LIB   = $(LIBDIR)/
	
# Define programs and commands.
CC      = 	g++

# compiler flags
#CFLAGS  =
#for ddd
#CFLAGS  += -ggdb -g -O0

all: client server

client: $(C_SRC_1)
	$(CC) $^ -I$(C_LIB) -I$(C_LIB) -o $(OUTDIR)/client

server: $(C_SRC_2)
	$(CC) $^ -I$(C_LIB) -I$(C_LIB) -o $(OUTDIR)/server
