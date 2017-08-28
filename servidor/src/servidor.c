#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PUERTO "4040"
#define PACKAGESIZE 1024
#define BACKLOG 5

int main(void) {
	puts("QuePasApp <servidor>");
	/***/
	int             statusAddrInfo;
	struct addrinfo hints;
		memset(&hints, 0, sizeof hints);
		hints.ai_family   = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags    = AI_PASSIVE;
	struct addrinfo *serverInfo;
	if ((statusAddrInfo = getaddrinfo(NULL, PUERTO, &hints, &serverInfo)) != 0){
		fprintf(stderr, "Error en getaddrinfo() : ", gai_strerror(statusAddrInfo));
		exit(1);
	}
	/**/
	int listenerSocket;
	if ((listenerSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) == -1){
		// Ser mas especifico con el error, revisando la variable global errno
		printf("Error en la creacion del socket");
		exit(1);
	}
	/**/
	if ((bind(listenerSocket, serverInfo->ai_addr, serverInfo->ai_addrlen)) == -1){
		// Ser mas especifico con el error, revisando la variable global errno
		printf("Error en el bindeo del socket");
		exit(1);
	}
	freeaddrinfo(serverInfo);
	listen(listenerSocket, BACKLOG);
	/**/
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int socketCliente = accept(listenerSocket, (struct sockaddr *) &addr, &addrlen);
	/**/
	char package[PACKAGESIZE];
	int status = 1; // Estructura que manjea el status de los recieve.
	printf("Cliente conectado. Esperando mensajes:\n");
	while (status != 0){
		status = recv(socketCliente, (void*) package, PACKAGESIZE, 0);
		if (status != 0) printf("%s", package);
	}
	/**/
	close(socketCliente);
	close(listenerSocket);
	return EXIT_SUCCESS;
}
