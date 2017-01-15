hw1: hw1.o utility.o
	g++ -W hw1.o utility.o -o hw1

utility.o: utility.h utility.cpp
	g++ -W -c utility.cpp

hw1.o: hw1.cpp
	g++ -W -c hw1.cpp

clean:
	rm -f *.o *.gch hw1
