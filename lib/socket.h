#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <netdb.h> 

class lesocket
{
  private:
  	//client variables
  	int n;
  	int sockfd;
  	int newsockfd;

  	//client functions
    void error(const char* msg);

  public:
  	//client 
    void sowrite(char* buffer);
	char* soread();
    void soclose();
    void soconnect(char* host, int port);

    //server
    void secreate(int port);
    void seaccept();
    int seavaiable();
    char seread();
    void sewrite();
    void seclose();

};

// private
void lesocket::error(const char* buffer)
{
	perror(buffer);
    exit(1);
}

// public
void lesocket::sowrite(char* buffer)
{
    n = write(sockfd,buffer,strlen(buffer));        // escreve o buffer no socket
    if (n < 0)                                      // checa se deu certo a escrita
        error("ERROR writing to socket");
}

char* lesocket::soread()
{
	char* return_buffer;
    n = read(sockfd,return_buffer,64);                    // ve resposta
    if (n < 0)                                      // erro na leitura
        error("ERROR reading from socket");
    return return_buffer;
}

void lesocket::soclose()
{
    close(sockfd);                                  // fecha socket
}

void lesocket::soconnect(char* host, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);       // faz o socket
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(host);                // pega o ip ou o hostnam
    if (server == NULL) {                           // check server
        fprintf(stderr,"ERROR, no such host\n");    // passar para perror
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));  // limpa string
    serv_addr.sin_family = AF_INET;                 // cria a familia para permitir o protocolo              
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,        // copia h_lenght bytes de h_addr para s_addr
         server->h_length);
    serv_addr.sin_port = htons(port);             // convert u_int da ordem do host para para network byte order
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)       // ve se conseguio conectar
        error("ERROR connecting");
}

void lesocket::secreate(int port)
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

void lesocket::seaccept()
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

int lesocket::seavaiable()
{
	int counte;
	ioctl(newsockfd,FIONREAD,&counte);
	return counte;
}

char lesocket::seread()
{
	char return_buffer;
    n = read(newsockfd,&return_buffer,1);        //le resposta
    if (n < 0) 
    	error("ERROR reading from socket");
    return return_buffer;
}

void lesocket::sewrite()
{
	n = write(newsockfd,"I got your message",18); // manda resposta
    if (n < 0) error("ERROR writing to socket");
}

void lesocket::seclose()
{
    close(newsockfd);
    close(sockfd);
}