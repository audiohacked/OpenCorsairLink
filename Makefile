CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lhidapi-hidraw

all: OpenCorsairLink

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

OpenCorsairLink: main.o CorsairLink.o CorsairFan.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o OpenCorsairLink

