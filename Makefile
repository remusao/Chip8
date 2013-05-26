CXX=clang++
CXXFLAGS=-std=c++11 -DDEBUG -O3 -Wall -Wextra -lsfml-graphics -lsfml-window -lsfml-system
SOURCE=src/main.cc
BIN=chip8

all:
	${CXX} ${CXXFLAGS} ${SOURCE} -o ${BIN}

clean:
	@rm -frv ${BIN}
	@find . -name "*.o" -delete
