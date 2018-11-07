TARGET_TCP_CLIENT=tcp_client
TARGET_TCP_SERVER=tcp_server
CC=gcc
CFLAGS=-g -Wall -std=gnu99 \
       -I./
LIBS=

ODIR=obj
SRC=$(wildcard ./*.c)
OBJS=$(patsubst %.c, $(ODIR)/%.o, $(SRC))


all: dir $(TARGET_TCP_CLIENT) $(TARGET_TCP_SERVER) 


$(TARGET_TCP_CLIENT): $(ODIR)/tcp_client.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(TARGET_TCP_SERVER): $(ODIR)/tcp_server.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 


dir:
	mkdir -p $(ODIR)

.PHONY: clean

clean:
	rm -rf $(ODIR) $(TARGET_TCP_CLIENT) $(TARGET_TCP_SERVER)
