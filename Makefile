cc = g++
exec = fux
cflags = -std=c++20 -g -Wall
src = $(wildcard src/main.cpp)
ex = src/examples
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all:
	$(cc) $(src) -o $(exec) $(cflags)

f = main

gen: 
	./$(exec) generate $(ex)/$(f).fux

code: 
	./$(exec) vm $(ex)/$(f).fux

clean:
	-rm $(exec)