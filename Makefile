all: prog.out

getListOfStrings: interpreter.o
	g++ -o a.out $^

prog.out: main.o orc.o orcinput.o orcrunner.o
	g++ -std=c++11 -o $@ $^

main.o: main.cpp
	g++ -std=c++11 -c $^ 

orc.o: orc.cpp
	g++ -std=c++11 -c $^

orcinput.o: orcinput.cpp
	g++ -std=c++11 -c $^

orcrunner.o: orcrunner.cpp
	g++ -std=c++11 -c $^

interpreter.o: interpreter.cpp
	g++ -std=c++11 -c $^
	


clean:
	rm *.out *.o 