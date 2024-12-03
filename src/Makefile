CC=g++
# CFLAGS=-Wall -Wextra -std=c++11 -fsanitize=address,undefined -g
CFLAGS=-Wall -Wextra -std=c++11 -g
LFLAGS=-lglfw
OUT=n-body

all: $(OUT)

$(OUT): *.cpp *.hpp
	$(CC) -o $(OUT) glad.c *.cpp ${CFLAGS} ${LFLAGS}

o: *.cpp *.hpp
	$(CC) -o $(OUT) glad.c *.cpp -O3 ${LFLAGS}

clean:
	rm -f $(OUT)