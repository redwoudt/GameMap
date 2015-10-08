OBJS = main.o DataConverter.o DataIO.o GameMap.o Factory.o PathFinder.o Receiver.o TestSuite.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)
MYPROGRAM = GameMap
ARCHIVE = $(MYPROGRAM).tar.gz
$(MYPROGRAM) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(MYPROGRAM)

DataConverter.o: DataConverter.cpp DataConverter.h 
	$(CC) $(CFLAGS) DataConverter.cpp
DataIO.o: DataIO.cpp DataIO.h
	$(CC) $(CFLAGS) DataIO.cpp
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
GameMap.o: GameMap.cpp GameMap.h
	$(CC) $(CFLAGS) GameMap.cpp
PathFinder.o: PathFinder.cpp PathFinder.h 
	$(CC) $(CFLAGS) PathFinder.cpp
Receiver.o: Receiver.cpp Receiver.h 
	$(CC) $(CFLAGS) Receiver.cpp
Factory.o: Factory.cpp Factory.h 
	$(CC) $(CFLAGS) Factory.cpp
TestSuite.o: TestSuite.cpp TestSuite.h 
	$(CC) $(CFLAGS) TestSuite.cpp

clean:
	\rm *.o *~ $(MYPROGRAM)

tar:
	tar cfz $(ARCHIVE) *.cpp *.h *.txt makefile 
