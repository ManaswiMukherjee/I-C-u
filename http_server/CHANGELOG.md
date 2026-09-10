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

# Day-1 9/9/26
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
if((c_sockfd = accept(sockfd, ...)) == -1){errorhandler("accept error");}
const char send_buf[] = "Hello World";
write(c_sockfd, send_buf, sizeof(send_buf));
close(c_sockfd);
```

#### removed redundant line `if (c_sockfd==-1){errorhandler("client accept problem");}`

### `socklen_t addr_len = sizeof(client_addr);` stores the budget for the size of type of socket we are using, here sockaddr_in and not ipv6


# Day-2 10/9/26
### Sending one message in multiple chunks with checking
## Why might write fail to send all the data required at once?
#### Two reasons:
* The network may have problems
* The size of the socket send buffer may be smaller than the size of the message that is required to be sent. It belongs to the kernel and not the network stack

` your program's buf[] --> write()-->  [kernel socket send buffer]  ---> NIC ---> network ---> receiver`

### loop for multiple times write
```c
ssize_t w_ptr = 0;  
while((size_t)w_ptr != sizeof(send_buf)){
    ssize_t w_sz = write(c_sockfd, &send_buf[w_ptr], sizeof(send_buf) - w_ptr);
    if(w_sz == -1){errorhandler("write problem");}  // checking if write encounters errors
    w_ptr += w_sz;
}
```

## Adding size debugging log lines to test the setup
```c
ssize_t w_ptr = 0;
int count = 0;
while((size_t)w_ptr != sizeof(send_buf)){
    ssize_t w_sz = write(c_sockfd, &send_buf[w_ptr], sizeof(send_buf) - w_ptr);
    if(w_sz == -1){errorhandler("write problem");}  // checking if write encounters errors
    printf("Wrote %zd bytes in iteration %d and completed sending %zd bytes of the total data", w_sz, count, w_ptr);
    count += 1;
    w_ptr += w_sz;
}
```

#### command to find the default send buffer size for the kernel `cat /proc/sys/net/core/wmem_default`
#### my machine is `212992`

## What library to use to generate 1MB of data?
### A cousin of bzero i.e. is `memset`

## Where to store the 1MB of data stack is not possible?
### Heap is only limited to virtual memory space while stack is limited by software limits so using dynamic memory allocation seems more good.

## How to check calloc for errors?
### Use NULL check. If calloc returns NULL then error has occured

### Not using free anywhere in the script as the amount of times calloc is called is only once and there is no risk of memory leaks. Whenever a process gets terminated all the memory related to it is freed by the os anyway, so no need of free.


## No way to test partial writes
### A blocking write on a healthy TCP connection does not return early just because the send buffer is small. If the buffer fills up mid-transfer, the kernel doesn't hand control back to your program — it internally pauses (blocks), waits for the connected peer to drain some data (freeing room), copies more in, and repeats this entirely inside the kernel.