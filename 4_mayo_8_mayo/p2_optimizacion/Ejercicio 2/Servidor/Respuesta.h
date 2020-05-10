#ifndef RESPUESTA_H_
#define RESPUESTA_H_

#include "SocketDatagrama.h"
#include "mensaje.h"

struct id_timeval {
    int id;
    struct timeval t;
};

struct registro{
	char celular[11];
	char CURP[19];
	char partido[4];
};

class Respuesta {
    private:
        SocketDatagrama *socketlocal;
    public:
        Respuesta(int pl);
        struct mensaje *getRequest(void);
        void sendReply(char *respuesta);
};

#endif