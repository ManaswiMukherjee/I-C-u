#include <stdio.h>
#include <stdlib.h>              //needed for exit()
#include <sys/socket.h>          //needed for all socket functions
#include <netinet/in.h>          //needed for predefined struct sockaddr_in variable type
#include <arpa/inet.h>           //needed for ip and port conversion to binary [htons() & inet_pton()]
#include <unistd.h>              //for using the close() function
#include <strings.h>             //for using the bzero() function to erase all data from memory

void errorhandler(const char *);

int main()
{
    int sockfd;

    struct sockaddr_in server_addr;
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){errorhandler("socket creation problem");}
    
    bzero(&server_addr,sizeof(server_addr));//clearing all previous data from memory
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr)<=0){errorhandler("ip assign error");};
    
    socklen_t addr_len = sizeof(server_addr);
    if(connect(sockfd, (const struct sockaddr *)&server_addr, addr_len)){errorhandler("connection error");}
    
    char* read_msg = NULL;
    size_t buf_size = 1024 * 1024;
    read_msg = calloc(buf_size, sizeof(char));
    if(read_msg == NULL){errorhandler("cannot init array");}

    ssize_t r_ptr = 0;
    int count = 0;
    while((size_t)r_ptr != buf_size){
        ssize_t r_sz = read(sockfd, &read_msg[r_ptr], buf_size - r_ptr);
        if(r_sz == -1){errorhandler("read problem");}
        if(r_sz == 0){break;}  // server closed connection / EOF before full buffer received
        printf("Read %zd bytes in iteration %d, total %zd so far\n", r_sz, count, r_ptr);
        count += 1;
        r_ptr += r_sz;
    }

    printf("\nTotal received: %zd bytes\n", r_ptr);
    
    close(sockfd);

    return 0;
}

void errorhandler(const char *message)
{
    perror(message);
    exit(1);
}