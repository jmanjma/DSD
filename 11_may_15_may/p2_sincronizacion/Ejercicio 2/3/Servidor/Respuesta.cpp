#include "Respuesta.h"
#include <sys/time.h>       // strcut timeval, gettimeofday()

Respuesta::Respuesta(int pl) {
    socketlocal = new SocketDatagrama(pl);
}

struct mensaje* Respuesta::getRequest() {
    struct mensaje *msj_recibir;
    PaqueteDatagrama paquete_recibir(sizeof(struct mensaje));

    socketlocal->recibe(paquete_recibir);

    msj_recibir = (struct mensaje*)paquete_recibir.obtieneDatos();
    printf("\n\tDatos recibidos \
            \n\t\tIP: %s \
            \n\t\tPuerto: %d \
            \n\t\tId: %d \
            \n\t\tMensaje: ", paquete_recibir.obtieneDireccion(), paquete_recibir.obtienePuerto(), msj_recibir->requestId);
    struct registro reg;
    memcpy(&reg, (struct registro*)msj_recibir->arguments, sizeof(struct registro));
    printf("%s%s%s\n", reg.celular, reg.CURP, reg.partido);

    return msj_recibir;
}

void Respuesta::sendReply(char* respuesta, int cont, struct mensaje *msj) {
    struct mensaje msj_enviar;

    msj_enviar.messageType = 1;
    msj_enviar.requestId = cont;
    bzero(msj_enviar.arguments, TAM_MAX_DATA);
    memcpy(msj_enviar.arguments, respuesta, sizeof(struct timeval));
    memcpy(&msj_enviar.lamport, (char*)msj->lamport, sizeof(msj->lamport));

    PaqueteDatagrama paquete_enviar(sizeof(msj_enviar));
    paquete_enviar.inicializaDatos((char*)&msj_enviar);

    // sleep(2);

    socketlocal->envia(paquete_enviar);

    struct timeval t;
    memcpy(&t, msj_enviar.arguments, sizeof(struct timeval));
    printf("\t\tRespuesta enviada: %d => [", msj_enviar.requestId);
    for (int i=0 ; i<NUM_SERVS ; i++)
        printf(" %d", msj_enviar.lamport[i]);
    printf(" ] %ld:%ld.\n", t.tv_sec, t.tv_usec);
}