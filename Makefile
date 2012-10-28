CC=c++ -std=gnu++11 -stdlib=libc++

all: tclj

tclj: tool.o TinyClojure.o
	$(CC) src/*.o -o tclj

tool.o: src/tool.cpp src/TinyClojure.h
	$(CC) -c src/tool.cpp -o src/tool.o

TinyClojure.o: src/TinyClojure.cpp src/TinyClojure.h
	$(CC) -c src/TinyClojure.cpp -o src/TinyClojure.o

clean:
	rm -f src/*.o tclj