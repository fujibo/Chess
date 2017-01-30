chess :	test.o board.o think_board.o te.o
	g++ -std=c++11 -g -o $@ $^ -lm -O3

test.o : test.cpp
	g++ -std=c++11 -g -c -o $@ $^ -lm -O3
board.o : board.cpp
	g++ -std=c++11 -g -c -o $@ $^ -lm -O3
think_board.o : think_board.cpp
	g++ -std=c++11 -g -c -o $@ $^ -lm -O3
te.o : te.cpp
	g++ -std=c++11 -g -c -o $@ $^ -lm -O3
clean :
	rm -f *.o chess
