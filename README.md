在本地通过命令make server1即可编译启动服务端
make client1 启动客户端
make server starting the server

```
struct Message {
    char from[20];
    int flag; // flag value 0, 1, 2  public message, private message, notice message;
    char message[1024];
}

```
