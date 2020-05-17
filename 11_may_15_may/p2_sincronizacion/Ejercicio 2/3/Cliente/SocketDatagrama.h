#ifndef SOCKETDATAGRAMA_H_
#define SOCKETDATAGRAMA_H_

#include <string.h>         // bzero
#include <arpa/inet.h>      // struct sockaddr_in
#include <unistd.h>         // close
#include <stdio.h>          // printf

using namespace std;

#include "PaqueteDatagrama.h"

class SocketDatagrama {
    private:
        struct sockaddr_in direccionLocal;
        struct sockaddr_in direccionForanea;
        int s;                                  // ID socket
        struct timeval timeout;
    public:
        SocketDatagrama(int);
        ~SocketDatagrama();
        int recibe(PaqueteDatagrama &p);        // Recibe un paquete tipo datagrama proveniente de este socket
        int envia(PaqueteDatagrama &p);         // Envia un paquete tipo datagrama desde este socket
        int recibeTimeout(PaqueteDatagrama &p, time_t segundos, suseconds_t microsegundos);
};

#endif