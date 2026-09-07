#include <stdio.h>                          //no explanation
#include <stdlib.h>                         //needed for exit()
#include <sys/socket.h>                     //needed for all socket functions
#include <arpa/inet.h>                      //needed for ip and port conversion to binary
#include <netinet/in.h>                     //needed for predefined struct sockaddr_in variable type
#include <unistd.h>                         //for using the close() function
#include <strings.h>                        //for using the bzero() function to erase all data from memory

void errorhandler(const char *);


int main(void)
{
    int sockfd;                              //sockfd--socket file descriptor
    struct sockaddr_in server_addr;          //declaring a variable(server_addr) of type [struct sockaddr_in]
    bzero(&server_addr,sizeof(server_addr));

    //configuring socket
    server_addr.sin_family = AF_INET;                   //defining the internet family
    server_addr.sin_port = htons(8080);                 //user gives the port
    server_addr.sin_addr.s_addr = INADDR_ANY;


    /* UNCOMMENT THIS AND COMMENT OUT THE ABOVE LINE IF YOU WANT TO ENTER YOUR OWN IP ADDRESS
    char *ip="YOUR_IP_ADDRESS";
    if(inet_pton(AF_INET, ip, &server_addr.sin_addr)){errorhandler("ip assign error")};        
                        //user gives the ip address
    */



    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){errorhandler("socket creation problem");}
    //creating a socket

    if((bind(sockfd, (const struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr)))==-1){errorhandler("binding problem");}
    //binding the socket to an address

    if((listen(sockfd,2))==-1){errorhandler("listen problem");}
    //listening for a connection

    
    int c_sockfd;//socket file for client
    bzero(&c_sockfd,sizeof(c_sockfd));
    struct sockaddr_in client_addr;//declaring a variable(server_addr) of type [struct sockaddr_in] for client
    
    socklen_t addr_len = sizeof(client_addr);
    if((c_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len))==-1){errorhandler("accept error");}
    //accept incoming connections from client

    if (c_sockfd==-1){errorhandler("client accept problem");}
    
    const char* send_buf = "Hello World";
    write(c_sockfd, send_buf, sizeof(send_buf));//writing to the connected client

    
    close(c_sockfd);    //closing client socket
    
    close(sockfd);      //closing server socket
    return 0;
}


/*this function is made to handle any errors at runtime*/

void errorhandler(const char *message)
{
    perror(message);
    exit(1);
}
