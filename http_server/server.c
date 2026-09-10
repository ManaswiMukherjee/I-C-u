#include <stdio.h>                          //no explanation
#include <stdlib.h>                         //needed for exit()
#include <sys/socket.h>                     //needed for all socket functions
#include <arpa/inet.h>                      //needed for ip and port conversion to binary
#include <netinet/in.h>                     //needed for predefined struct sockaddr_in variable type
#include <unistd.h>                         //for using the close() function
#include <strings.h>                        //for using the bzero() function to erase all data from memory
#include <string.h>                         //for using memset

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



    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){errorhandler("socket creation problem");}
    //creating a socket

    if((bind(sockfd, (const struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr))) == -1){errorhandler("binding problem");}
    //binding the socket to an address

    if((listen(sockfd,2)) == -1){errorhandler("listen problem");}
    //listening for a connection

    
    int c_sockfd;                       //socket file for client
    bzero(&c_sockfd, sizeof(c_sockfd));
    struct sockaddr_in client_addr;     //declaring a variable(server_addr) of type [struct sockaddr_in] for client
    
    // data to be sent
    //const char* send_buf
    size_t buf_siz_big = 1024 * 1024;
    char* send_buf_big = NULL;
    if((send_buf_big = calloc(buf_siz_big, sizeof(char))) == NULL){errorhandler("cannot init array");}
    memset(send_buf_big, 'F', buf_siz_big);


    // loop structure for multiple connections
    while(1){
    
        socklen_t addr_len = sizeof(client_addr);   // stores budget for socket type

        if((c_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len)) == -1){errorhandler(" client accept error");}
        //accept incoming connections from client
        
        /*//manually setting the size for a one time send so that partial write can be tested
        int small_size = 4096;
        if(setsockopt(c_sockfd, SOL_SOCKET, SO_SNDBUF, &small_size, sizeof(small_size)) == -1){errorhandler("setsockopt problem");}


        int actual_size;
        socklen_t opt_len = sizeof(actual_size);
        getsockopt(c_sockfd, SOL_SOCKET, SO_SNDBUF, &actual_size, &opt_len);
        printf("Actual SO_SNDBUF: %d\n", actual_size);*/
    
        ssize_t w_ptr = 0;
        int count = 0;
        while((size_t)w_ptr != buf_siz_big){
            ssize_t w_sz = write(c_sockfd, &send_buf_big[w_ptr], buf_siz_big - w_ptr);
            if(w_sz == -1){errorhandler("write problem");}  // checking if write encounters errors
            printf("Wrote %zd bytes in iteration %d and completed sending %zd bytes of the total data\n ", w_sz, count, w_ptr);
            count += 1;
            w_ptr += w_sz;
            
        }
        

        char dst[INET_ADDRSTRLEN];                  // dst array created with max possible size of ip
        inet_ntop(AF_INET, &client_addr.sin_addr , dst, INET_ADDRSTRLEN);   //
        printf("Wrote %zd bytes to %s through port %u\n", w_ptr, dst, ntohs(client_addr.sin_port));
    
        close(c_sockfd);    /* closing client socket as too many open sockets will lead to exceeding
        limits of the OS's max FD limit */
    }    
    
    // control never reaches these lines, like mcu programming no return 0 statement there.
    close(sockfd);      //closing server socket
    return 0;
}


/*this function is made to handle any errors at runtime*/

void errorhandler(const char *message)
{
    perror(message);
    exit(1);
}
