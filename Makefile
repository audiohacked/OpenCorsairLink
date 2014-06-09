CC =  g++
CFLAGS := -Wall -g
LDFLAGS := -lhidapi-hidraw

all: OpenCorsairLink

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

OpenCorsairLink: main.o CorsairLink.o CorsairFanInfo.o
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

clean:
	rm -rf *.o OpenCorsairLink

