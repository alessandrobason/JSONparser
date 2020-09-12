CC = g++
FLAGS = -std=c++14
CPP = example-read.cpp json/parser.cpp json/deserializer.cpp
OUT = jparser.exe

all:
	$(CC) $(FLAGS) -o $(OUT) $(CPP)
