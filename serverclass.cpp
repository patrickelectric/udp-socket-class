/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib/socket.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int sockfd;

void create_server(int port)
{
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  //faz socket
    if (sockfd < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr)); //limpa serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);                // convert u_int da ordem do host para para network byte order
    /*it exists in a name space
    (address family) but has no address assigned to it.  bind() assigns
    the address specified by addr to the socket referred*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
          sizeof(serv_addr)) < 0) 
          error("ERROR on binding");

    /*marks the socket referred to by sockfd as a passive socket,
    that is, as a socket that will be used to accept incoming connection
    requests using*/
    listen(sockfd,5); //maximo pendentes
}


int newsockfd;

void accept_socket()
{
    struct sockaddr_in cli_addr;
    socklen_t clilen;

    clilen = sizeof(cli_addr);
    /*It extracts the first connection
    request on the queue of pending connections for the listening socket,
    sockfd*/
    newsockfd = accept(sockfd, 
             (struct sockaddr *) &cli_addr, 
             &clilen);
    if (newsockfd < 0) 
      error("ERROR on accept");

}

    char buffer[256];
    int n;

void read_scoket()
{
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);        //le resposta
    if (n < 0) error("ERROR reading from socket");
}

void write_socket()
{
    n = write(newsockfd,"I got your message",18); // manda resposta
    if (n < 0) error("ERROR writing to socket");
}

void close_socket()
{
    close(newsockfd);
    close(sockfd);
}

int main(int argc, char *argv[])
{

    if (argc < 2) 
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    lesocket meuscoket;

    meuscoket.secreate(atoi(argv[1]));
    //create_server(atoi(argv[1]));
    meuscoket.seaccept();    
    //accept_socket();

    //read_scoket();

    sleep(1);

    printf("Here is the message: ");
    while(meuscoket.seavaiable()>0) printf("%c",meuscoket.seread());
    printf("\n");

    meuscoket.sewrite();
    meuscoket.seclose();

    return 0; 
}
