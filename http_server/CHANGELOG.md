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