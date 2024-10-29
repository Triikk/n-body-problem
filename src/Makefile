CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -O3 -fsanitize=address,undefined
OUT=n-body

all: $(OUT)

$(OUT): *.cpp
	$(CC) -o $(OUT) *.cpp ${CFLAGS}

clean:
	rm -f $(OUT)