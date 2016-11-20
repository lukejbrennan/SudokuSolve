all: main

main: main.cpp Puzzle.h
	g++ main.cpp -o main

clean:
	rm main
