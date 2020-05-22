#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

#include <string.h>         // bzero
#include <arpa/inet.h>      // struct sockaddr_in
#include <unistd.h>         // close
#include <stdio.h>          // printf

int puerto = 7200;

int main(void) {
    int num[2];
    int s, res;
    unsigned int clilen;
    struct sockaddr_in server_addr, msg_to_client_addr;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    /* se asgina una direccion al socket der servidor */
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(puerto);

    bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr));
    clilen = sizeof(msg_to_client_addr);

    printf("\n\tServidor iniciado, esperando mensajes...\n");

    while(1) {
        recvfrom(s, (char*)num, 2*sizeof(int), 0, (struct sockaddr*)&msg_to_client_addr, &clilen);

        printf("\n\tDatos recibidos \
            \n\tIP: %s \
            \n\tPuerto: %d \
            \n\tNumeros: %d y %d.", inet_ntoa(msg_to_client_addr.sin_addr), ntohs(msg_to_client_addr.sin_port), num[0], num[1]);

        res = num[0] + num[1];

        printf("\n\tRespuesta enviada: %d.\n", res);

        /* envia la peticion al cliente. La estructura msg_to_client_addr contiene la direccion socet del cliente */
        sendto(s, (char*)&res, sizeof(int), 0, (struct sockaddr*)&msg_to_client_addr, clilen);
    }
}