//client requests for html from servers


#include <stdio.h>
#include <stdlib.h>              //needed for exit()
#include <sys/socket.h>          //needed for all socket functions
#include <netinet/in.h>          //needed for predefined struct sockaddr_in variable type
#include <arpa/inet.h>           //needed for ip and port conversion to binary [htons() & inet_pton()]
#include <unistd.h>              //for using the read, write & close function
#include <strings.h>             //for using the bzero() function to erase all data from memory
#include <string.h>              //for using the strlen function

void errorhandler(const char *); //defining the error handler function

int main(int argc, char ** argv)
{
    int sockfd;

    struct sockaddr_in server_addr;
    //if(argc != 2){errorhandler("give ip");}
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){errorhandler("socket creation problem");}
    
    bzero(&server_addr,sizeof(server_addr));//clearing all previous data from memory
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);

    if(inet_pton(AF_INET, "142.250.71.110", &server_addr.sin_addr)<=0){errorhandler("ip assign error");};
    
    socklen_t addr_len = sizeof(server_addr);
    if(connect(sockfd, (const struct sockaddr *)&server_addr, addr_len)){errorhandler("connection error");}
    
    const char *req = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
    if(write(sockfd, req, strlen(req))==-1){errorhandler("could not write");}
    
    char read_msg[100000];
    int n;
    while ((n = read(sockfd, read_msg, sizeof read_msg)) > 0) {
    fwrite(read_msg, 1, n, stdout);
    }

    if(n<0){errorhandler("read error");}
    /*
    char read_msg[100];
    read(sockfd, read_msg, sizeof(read_msg));
    printf("%d",sizeof read_msg);*/
    
    close(sockfd);

    return 0;
}

void errorhandler(const char *message)
{
    perror(message);
    exit(1);
}