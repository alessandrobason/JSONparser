CC = g++
FLAGS = -std=c++14
LIB = json/parser.cpp json/deserializer.cpp json/readfile.cpp
OUTREAD = jreader.exe
OUTWRITE = jwriter.exe

read:
	$(CC) $(FLAGS) -o $(OUTREAD) example-read.cpp $(LIB)

write:
	$(CC) $(FLAGS) -o $(OUTWRITE) example-write.cpp $(LIB)

clean:
	rm $(OUTREAD) $(OUTWRITE)
