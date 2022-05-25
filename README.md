在ChatRoom/下:
在本地通过命令make chatserver即可编译启动服务端
在本地通过命令make chatclient即可编译客户端

`./Server` 启动服务端
`./Client` 启动客户端
`tail -f text` 模拟启动聊天窗口
```
struct Message {
    char from[20];
    int flag; // flag value 0, 1, 2  public message, private message, notice message;
    char message[1024];
}

```

```
Server_Port = Server Port
```

private message : 
```
@username message + enter
```
public message : 
```
message + enter 
```


fangsong
