# Makefile for COMP1521 Lab03

CC=dcc
CFLAGS=-Wall -Werror

all : watb maf

watb : where_are_the_bits.o
	$(CC) -o watb where_are_the_bits.o

maf : make_a_float.o
	$(CC) -o maf make_a_float.o

clean :
	rm -f watb maf where_are_the_bits.o make_a_float.o core


