using namespace std;

#include "SocketDatagrama.h"
#include <errno.h>          // errno
#include <stdlib.h>

SocketDatagrama::SocketDatagrama(int _puerto) {
    s = socket(AF_INET, SOCK_DGRAM, 0);

    bzero((char*)&direccionLocal, sizeof(direccionLocal));
    direccionLocal.sin_family = AF_INET;
    direccionLocal.sin_addr.s_addr = INADDR_ANY;
    direccionLocal.sin_port = htons(_puerto);

    bind(s, (struct sockaddr*) &direccionLocal, sizeof(direccionLocal));
}

SocketDatagrama::~SocketDatagrama() {
    close(s);
}


int SocketDatagrama::recibe(PaqueteDatagrama &p) {
    char datos[p.obtieneLongitud()];
    unsigned int clilen = sizeof(direccionForanea);

    recvfrom(s, datos, p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, &clilen);

    p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
    p.inicializaPuerto(ntohs(direccionForanea.sin_port));
    p.inicializaDatos(datos);

    return 0;
}

int SocketDatagrama::recibeTimeout(PaqueteDatagrama &p, time_t segundos, suseconds_t microsegundos) {
    char datos[p.obtieneLongitud()];
    unsigned int clilen = sizeof(direccionForanea);

    timeout.tv_sec = segundos;
    timeout.tv_usec = microsegundos;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

    int n = recvfrom(s, datos, p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, &clilen);

    if (n < 0) {
        if (errno == EWOULDBLOCK)
            fprintf(stderr, "\t\t** Tiempo para recepción transcurrido\n");
        else
            fprintf(stderr, "\t\t** Error en recvfrom\n");
    } else {
        p.inicializaIp(inet_ntoa(direccionForanea.sin_addr));
        p.inicializaPuerto(ntohs(direccionForanea.sin_port));
        p.inicializaDatos(datos);
    }

    return n;
}

int SocketDatagrama::envia(PaqueteDatagrama &p) {
    if (p.obtienePuerto()>-1) {
        bzero((char*)&direccionForanea, sizeof(direccionForanea));
        direccionForanea.sin_family = AF_INET;
        direccionForanea.sin_addr.s_addr = inet_addr(p.obtieneDireccion());
        direccionForanea.sin_port = htons(p.obtienePuerto());
    }
    
    sendto(s, p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr*)&direccionForanea, sizeof(direccionForanea));

    return 0;
}

void SocketDatagrama::setBroadcast() {
    int yes = 1;
    int res = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int));
    if(res<0) {
        printf("\n\tError al inicializar socket en modo broadcast!.");
        exit(-1);
    }
}

void SocketDatagrama::unsetBroadcast() {
    int yes = 0;
    int res = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int));
    if(res<0) {
        printf("\n\tError al desinicializar socket en modo broadcast!.");
        exit(-1);
    }
}