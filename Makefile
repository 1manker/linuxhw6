##########################
#MakeFile
#Lucas Manker
#COSC 3750 Spring 2020
#Homework #6
#3/7/20
#
#Makefile for hw6
#########################

CC=gcc
CFLAGS=-ggdb -Wall

.PHONY: clean

wytar: wytar.o
	$(CC) $(CFLAGS) wytar.o -o wytar

wytar.o: wytar.h wytar.c
	$(CC) $(CFLAGS) -c wytar.h wytar.c


clean:
	/bin/rm -f wytar
