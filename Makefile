hw1: hw1.o utility.o
	g++ -W -std=c++11 hw1.o utility.o -o $@

utility.o: utility.h utility.cpp
	g++ -W -std=c++11 -c utility.cpp

clean:
	rm -f *.o *.gch hw1
