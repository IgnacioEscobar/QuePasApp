#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define IP "127.0.0.1"
#define PUERTO "4040"
#define PACKAGESIZE 1024

int main(void) {
	puts("QuePasApp <cliente>");
	/***/
	int             statusAddrInfo;
	struct addrinfo hints;
		memset(&hints, 0, sizeof hints);
		hints.ai_family   = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *serverInfo;
	if ((statusAddrInfo = getaddrinfo(IP, PUERTO, &hints, &serverInfo)) != 0){
		fprintf(stderr, "Error en getaddrinfo() : ", gai_strerror(statusAddrInfo));
		exit(1);
	}
	/**/
	int socketFD;
	if ((socketFD = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) == -1){
		// Ser mas especifico con el error, revisando la variable global errno
		printf("Error en la creacion del socket");
		exit(1);
	}
	/**/
	if ((connect(socketFD, serverInfo->ai_addr, serverInfo->ai_addrlen)) == -1){
		// Ser mas especifico con el error, revisando la variable global errno
		printf("Error en la conexion");
		exit(1);
	}
	freeaddrinfo(serverInfo);
	/**/
	printf("Se ha conectado al servidor");
	int enviar = 1;
	char message[PACKAGESIZE];
	while(enviar){
		fgets(message, PACKAGESIZE, stdin);
		if (!strcmp(message,"exit\n")) enviar = 0;
		if (enviar) send(socketFD, message, strlen(message) + 1, 0);
	}
	/**/
	close(socketFD);
	return EXIT_SUCCESS;
}
