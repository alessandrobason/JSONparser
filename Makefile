CC = g++
FLAGS = -std=c++14
LIB = src/parser.cpp src/deserializer.cpp src/readfile.cpp

all: read write

read:
	$(CC) $(FLAGS) -o jreader examples/read.cpp $(LIB)

write:
	$(CC) $(FLAGS) -o jwriter examples/write.cpp $(LIB)

clean:
	rm $(OUTREAD) $(OUTWRITE)
