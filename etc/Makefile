CC = gcc
CFLAGS = -DWGSDIR=\"$(WGSDIR)\" -DLCCDIR=\"$(LCCDIR)\"
BINDIR = $(HOME)/bin
WGSDIR = $(HOME)/cvs/wgs-src
LCCDIR = $(HOME)/lcc/output/

scpu: scpu.o lcc.o
	$(CC) scpu.o lcc.o -o lcc

install:
	cp lcc $(BINDIR)
