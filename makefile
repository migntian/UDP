.PHONY:clean

all:server client

server:server.c
	gcc -o $@ $^

client:client.c
	gcc -o $@ $^

clean:
	rm -f server client

