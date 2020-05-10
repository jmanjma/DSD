#ifndef _SOLICITUD_H_
#define _SOLICITUD_H_

#include "SocketDatagrama.h"

struct id_timeval {
    int id;
    struct timeval t;
};

struct registro{
	char celular[11];
	char CURP[19];
	char partido[4];
};

class Solicitud {
    private:
        SocketDatagrama *socketlocal;
    public:
        Solicitud();
        int doOperation(char *IP, int puerto, int operationId, int requestId, char *argumnts, bool bandera);
};

#endif