CC=/opt/loongarch64-linux-gnu-2021-12-10-vector/bin/loongarch64-linux-gnu-gcc
CFLAGS += -static
CFLAGS += -c
CFLAGS += -Wall
LIBS = -static -o

all:
	$(CC) $(CFLAGS) *.c
	$(CC) *.o $(LIBS) uart
	rm *.o