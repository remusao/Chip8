CXX=g++
CXXFLAGS=-std=c++11 -g -Wall -Wextra -lsfml-graphics -lsfml-window -lsfml-system
SOURCE=src/main.cc
BIN=chip8

all:
	${CXX} ${CXXFLAGS} ${SOURCE} -o ${BIN}

clean:
	@rm -frv ${BIN}
	@find . -name "*.o" -delete
