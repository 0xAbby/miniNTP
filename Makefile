
CC= g++
ARGS= -Wall -O2 -std=c++20
SRC= ./
BUILD= ./build

default: NTPclient
	${CC} ${ARGS} -o miniNTP ${BUILD}/NTPclient.o ${SRC}/miniNTP.cpp

NTPclient: 
	${CC} ${ARGS} -c ${SRC}/NTPclient.cpp -o ${BUILD}/NTPclient.o


format:
	astyle --style=allman --indent=spaces=2 ./*.cpp
	astyle --style=allman --indent=spaces=2 ./*.h
	rm ./*.orig

clean:
	rm -rf miniNTP ${BUILD}/*.o