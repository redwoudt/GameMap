OBJS = main.o DataConverter.o DataIO.o GameMap.o Factory.o PathFinder.o Receiver.o TestSuite.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)
MYPROGRAM = GameMap
ARCHIVE = $(MYPROGRAM).tar.gz
$(MYPROGRAM) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(MYPROGRAM)

DataConverter.o: src/DataConverter.cpp src/DataConverter.h 
	$(CC) $(CFLAGS) src/DataConverter.cpp
DataIO.o: src/DataIO.cpp src/DataIO.h
	$(CC) $(CFLAGS) src/DataIO.cpp
main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp
GameMap.o: src/GameMap.cpp src/GameMap.h
	$(CC) $(CFLAGS) src/GameMap.cpp
PathFinder.o: src/PathFinder.cpp src/PathFinder.h 
	$(CC) $(CFLAGS) src/PathFinder.cpp
Receiver.o: src/Receiver.cpp src/Receiver.h 
	$(CC) $(CFLAGS) src/Receiver.cpp
Factory.o: src/Factory.cpp src/Factory.h 
	$(CC) $(CFLAGS) src/Factory.cpp
TestSuite.o: src/TestSuite.cpp src/TestSuite.h 
	$(CC) $(CFLAGS) src/TestSuite.cpp

clean:
	\rm *.o *~ $(MYPROGRAM)

tar:
	tar cfz $(ARCHIVE) *.cpp *.h *.txt makefile 
