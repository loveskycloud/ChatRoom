all:
	g++ -I ./include ./src/*.cpp -o server -pthread
	mv ./server ./bin

server1:
	g++ -I ./include ./src/*.cpp ./server/server.cpp -o server_1 -pthread
	mv ./server_1 ./bin
	./bin/server_1 8888

client1:
	g++ -I ./include ./src/* ./client/client.c -o client_1 -pthread
	mv ./client_1 ./bin
	./bin/client_1 39.98.62.82 8888
clean:
	rm ./bin/*
