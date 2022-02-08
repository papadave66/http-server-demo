server:	http-server-demo.c
	$(CC) http-server-demo.c -o server

run:	server
	./server

clean:
	rm -f server
