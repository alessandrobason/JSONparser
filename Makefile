CC = g++
FLAGS = -std=c++14
LIB = src/parser.cpp src/deserializer.cpp src/readfile.cpp
OUTREAD = jreader
OUTWRITE = jwriter

read:
	$(CC) $(FLAGS) -o $(OUTREAD) examples/read.cpp $(LIB)

write:
	$(CC) $(FLAGS) -o $(OUTWRITE) examples/write.cpp $(LIB)

clean:
	rm $(OUTREAD) $(OUTWRITE)
