CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -fsanitize=address,undefined -g
LFLAGS=-lglfw
OUT=n-body

all: $(OUT)

$(OUT): *.cpp *.hpp
	$(CC) -o $(OUT) glad.c *.cpp ${CFLAGS} ${LFLAGS}

clean:
	rm -f $(OUT)