CC=g++
CFLAGS=-Isingle_include/ -g -std=c++17

client: client.cpp requests.cpp helpers.cpp buffer.cpp jsonparser.cpp
	$(CC) $(CFLAGS) -o client client.cpp requests.cpp helpers.cpp buffer.cpp jsonparser.cpp

run: client
	./client

clean:
	rm -f *.o client
