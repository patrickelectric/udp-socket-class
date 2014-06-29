#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int sockfd, n;
char buffer[256];

void send_msg(char* msg)
{
    n = write(sockfd,msg,strlen(msg));        // escreve o buffer no socket
    if (n < 0)                                      // checa se deu certo a escrita
        error("ERROR writing to socket");
}

void receive_msg()
{
    bzero(buffer,256);                              // limpa o buffer
    n = read(sockfd,buffer,255);                    // ve resposta
    if (n < 0)                                      // erro na leitura
        error("ERROR reading from socket");
}

void close_socket()
{
    close(sockfd);                                  // fecha socket
}

void connect_socket(char* arg_host, int arg_port)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);       // faz o socket
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(arg_host);                // pega o ip ou o hostnam
    if (server == NULL) {                           // check server
        fprintf(stderr,"ERROR, no such host\n");    // passar para perror
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));  // limpa string
    serv_addr.sin_family = AF_INET;                 // cria a familia para permitir o protocolo              
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,        // copia h_lenght bytes de h_addr para s_addr
         server->h_length);
    serv_addr.sin_port = htons(arg_port);             // convert u_int da ordem do host para para network byte order
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)       // ve se conseguio conectar
        error("ERROR connecting");

}


void send2socket(char* arg_host, int arg_port, char* msg)
{
    connect_socket(arg_host,arg_port);

    send_msg(msg);

    receive_msg();

    printf("%s\n",buffer);                          // printa resposta

    close_socket();
}

int main(int argc, char *argv[])
{
    
    if (argc < 4) {
       printf("usage %s hostname port msg\n", argv[0]);   // passar para perror
       exit(0);
    }

    send2socket(argv[1], atoi(argv[2]), argv[3]);

    return 0;
}
