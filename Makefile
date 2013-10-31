######DEEPA CHARI
######FALL 2012
######CS 490 - SENIOR PROJECT
######YALE COLLEGE

PROG = model.out
FLAGS = -Wall -pedantic -g -std=c++0x -fmax-errors=1
CPP = corpus.cpp outputs.cpp analysis.cpp algorithm.cpp main.cpp

$(PROG): header.hpp $(CPP) 
	g++ $(FLAGS) $(CPP) -o $(PROG)
	
.cpp.o:
	g++ $(FLAGS) $< -o $@

clean:
	rm *.out
	
# unit tests for debugging
corpus: corpus.o
	g++ $(FLAGS) corpus.o -o corpus.out
outputs: corpus.o outputs.o
	g++ $(FLAGS) corpus.o outputs.o -o outputs.out