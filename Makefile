all: main.cpp
	g++ -g -o quine main.cpp

clean:
	rm quine
