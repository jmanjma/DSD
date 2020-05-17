#include "Respuesta.h"
#include <sys/time.h>       // strcut timeval, gettimeofday()

Respuesta::Respuesta(int pl) {
    socketlocal = new SocketDatagrama(pl);
}

struct mensaje* Respuesta::getRequest() {
    struct mensaje *msj_recibir, aux;
    PaqueteDatagrama paquete_recibir(sizeof(aux));

    socketlocal->recibe(paquete_recibir);
    msj_recibir = (struct mensaje*)paquete_recibir.obtieneDatos();

    printf("\n\tDatos recibidos \
            \n\t\tIP: %s \
            \n\t\tPuerto: %d \
            \n\t\tId: %d \
            \n\t\tMensaje: ", paquete_recibir.obtieneDireccion(), paquete_recibir.obtienePuerto(), msj_recibir->requestId);
    struct registro reg;
    memcpy(&reg, (struct registro*)msj_recibir->arguments, sizeof(reg));
    printf("%s%s%s\n", reg.celular, reg.CURP, reg.partido);

    return msj_recibir;
}

void Respuesta::sendReply(char* respuesta) {
    struct mensaje msj_enviar;

    msj_enviar.messageType = 1;
    bzero(msj_enviar.arguments, TAM_MAX_DATA);
    memcpy(msj_enviar.arguments, respuesta, sizeof(struct id_timeval));

    PaqueteDatagrama paquete_enviar(sizeof(msj_enviar));
    paquete_enviar.inicializaDatos((char*)&msj_enviar);

    // sleep(2);

    socketlocal->envia(paquete_enviar);

    struct id_timeval t_aux;
    memcpy(&t_aux, msj_enviar.arguments, sizeof(struct id_timeval));
    printf("\t\tRespuesta enviada: %d => %ld:%ld.\n", t_aux.id, t_aux.t.tv_sec, t_aux.t.tv_usec);
}