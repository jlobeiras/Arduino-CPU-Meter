CC = g++
CXX = g++
CPPFLAGS = -std=c++11 -Wall -g
LDFLAGS = -g

all: SerialCom

main.o: main.cpp SerialCom.h

SerialCom.o: SerialCom.h SerialCom.cpp SerialComLinux.h SerialComWin.h

SerialCom: main.o SerialCom.o

clean:
	 rm -f main.o SerialCom.o
