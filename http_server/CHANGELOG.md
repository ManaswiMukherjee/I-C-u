## Error1  on running `./server.out` -  binding problem: Address already in use

Wait a while after running immediately after

## Error2 on running the setup
```
./client.out 
1000
Hello Wo
```

### Fix - Changed from pointer string to fixed array string. `sizeof` is not workable with pointer strings it is for array strings.
### We could also use `strlen(send_buf)`

## Error3 - BUFFER OVER-READ stack info got leaked

**Identity/account info:**
- Your username (`Manaswi`), full home directory path, hostname (`fedora`), and your Linux distro (Fedora, XFCE desktop)

**System paths and config:**
- Various `XDG_*` paths, config/cache directories, and your `PWD` at the time — revealing your exact working directory (`/home/Manaswi/Desktop/C programs/http_server`), which also tells you the project folder name

**Development environment details:**
- VS Code extension paths and debugger socket paths
- Python-related env vars (`PYTHONSTARTUP`, `IDF_PATH` pointing to an ESP8266 SDK — suggesting you also do embedded/IoT dev)
- Editor (`nano`), locale (`en_IN.utf8` — suggesting India-based locale settings)

**Session/auth-adjacent info:**
- `SSH_AUTH_SOCK` path (a socket path used by your SSH agent — not a key itself, but reveals the agent socket location)
- `SESSION_MANAGER`, `XDG_RUNTIME_DIR`, `DISPLAY` — desktop session internals

### Fix - changed fwrite line `fwrite(read_msg, 1, nbytes, stdout);`


## For accepting connections one by one, does the packet of B get lost while A's packet is being processed(Both the packets were sent at once).
#### listen holds those packets

## Grouping of lines below in fresh and one time declaration for use in loop structure

```C
int c_sockfd;
bzero(&c_sockfd,sizeof(c_sockfd));
struct sockaddr_in client_addr;
socklen_t addr_len = sizeof(client_addr);
if((c_sockfd = accept(sockfd, ...))==-1){errorhandler("accept error");}
if (c_sockfd==-1){errorhandler("client accept problem");}
const char send_buf[] = "Hello World";
write(c_sockfd, send_buf, sizeof(send_buf));
close(c_sockfd);
```

### one time
```C
int c_sockfd;
bzero(&c_sockfd,sizeof(c_sockfd));
struct sockaddr_in client_addr;
```

### fresh inits
```c
socklen_t addr_len = sizeof(client_addr);
if((c_sockfd = accept(sockfd, ...))==-1){errorhandler("accept error");}
if (c_sockfd==-1){errorhandler("client accept problem");}
const char send_buf[] = "Hello World";
write(c_sockfd, send_buf, sizeof(send_buf));
close(c_sockfd);
```

### `socklen_t addr_len = sizeof(client_addr);` stores the budget for the size of type of socket we are using, here sockaddr_in and not ipv6
