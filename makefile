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
	./bin/client_1 192.168.116.128 8888
chatserver:
	g++ -g ./server/server_bak.c -I ./include ./src/get_value.c ./src/common.c ./src/Tcp.c -pthread -o Server
chatclient:
	g++ ./client/client_bak.c -I ./include ./src/get_value.c ./src/Tcp.c ./src/common.c -o Client
clean:
	rm ./bin/*
