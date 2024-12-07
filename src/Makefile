CC=g++
CFLAGS=-Wall -Wextra -O3 -std=c++11 -fsanitize=address,undefined -g -fopenmp -Wno-implicit-fallthrough
# CFLAGS=-Wall -Wextra -std=c++11 -fopenmp
LFLAGS=-lglfw
OUT=n-body

all: $(OUT)

$(OUT): *.cpp *.hpp
	$(CC) $(CFLAGS) -o $(OUT) glad.c *.cpp  $(LFLAGS)

o: *.cpp *.hpp
	$(CC) $(CFLAGS) -O3 -o $(OUT) glad.c *.cpp $(LFLAGS)

view: *.cpp *.hpp
	$(CC) $(CFLAGS) -O3 -o $(OUT) glad.c *.cpp $(LFLAGS)


clean:
	rm -f $(OUT)