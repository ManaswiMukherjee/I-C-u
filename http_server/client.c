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
    
    char read_msg[1000];     //100 is a size we would like to read at once
    int nbytes = read(sockfd, read_msg, sizeof(read_msg));
    
    if(nbytes > 0){
        fwrite(read_msg, 1, nbytes, stdout);
    printf("\n%d\n",sizeof read_msg);
    }
    
    close(sockfd);

    return 0;
}

void errorhandler(const char *message)
{
    perror(message);
    exit(1);
}