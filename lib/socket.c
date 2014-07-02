/**
  ******************************************************************************
  * @file    socket.c
  * @author  Patrick José Pereira
  * @version V1.3
  * @date    2-Jul-2014
  * @brief   A library to make comunication with socket
  *
  ******************************************************************************/

#include "socket.h"


/** \brief Print error message. 
  *
  * @param  buffer      Receive msg to print
  * @retval NONE
  */
void lesocket::error(const char* buffer)
{
	perror(buffer);
    exit(1);
}

/** \brief Write a byte on socket, used only by client. 
  *
  * @param  byte        Byte to send to server  
  * @retval NONE
  */
void lesocket::sowrite(char byte)
{
    n = write(sockfd,&byte,1);                       // escreve o buffer no socket
    if (n < 0)                                      // checa se deu certo a escrita
        error("ERROR writing to socket");
}

/** \brief Check how much bytes have on the socket to read. 
  *
  * @param  NONE
  * @retval int         Return number of bytes on the socket      
  */
int lesocket::soavaiable()
{
	int counte;
	ioctl(sockfd,FIONREAD,&counte);
	return counte;
}

/** \brief Read a byte on the socket. 
  *
  * @param  NONE        
  * @retval char        Return char on the socket        
  */
char lesocket::soread()
{
	char return_buffer;
    n = read(sockfd,&return_buffer,1);               // ve resposta
    if (n < 0)                                      // erro na leitura
        error("ERROR reading from socket");
    return return_buffer;
}

/** \brief Close client connection. 
  *
  * @param  NONE        
  * @retval NONE
  */
void lesocket::soclose()
{
    close(sockfd);                                  // fecha socket
}

/** \brief Connect client on server. 
  *
  * @param  host        Host address of server         
  * @param  port        Port number        
  * @retval NONE
  */
void lesocket::soconnect(char* host, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);       // faz o socket
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(host);                   // pega o ip ou o hostnam
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

/** \brief Create a server on Port. 
  * @param  port        Port number        
  * @retval NONE
  */
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

/** \brief  Accept new connections. 
  * @param  NONE  
  * @retval NONE
  */
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

/** \brief  Return number of bytes on socket. 
  * @param  NONE      
  * @retval int         Return number of bytes on socket
  */
int lesocket::seavaiable()
{
	int counte;
	ioctl(newsockfd,FIONREAD,&counte);
	return counte;
}

/** \brief Check how much bytes have on the socket to read. 
  *
  * @param  NONE
  * @retval int         Return number of bytes on the socket      
  */
char lesocket::seread()
{
	char return_buffer;
    n = read(newsockfd,&return_buffer,1);        //le resposta
    if (n < 0) 
    	error("ERROR reading from socket");
    return return_buffer;
}

/** \brief Write a byte on socket, used only by server. 
  *
  * @param  byte        Byte to send client  
  * @retval NONE
  */
void lesocket::sewrite(char byte)
{
	n = write(newsockfd,&byte,1); // manda resposta
    if (n < 0) error("ERROR writing to socket");
}

/** \brief Close client & server connections. 
  *
  * @param  NONE        
  * @retval NONE
  */
void lesocket::seclose()
{
    close(newsockfd);
    close(sockfd);
}