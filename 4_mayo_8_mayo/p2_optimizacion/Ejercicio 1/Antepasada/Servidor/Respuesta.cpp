#include "Respuesta.h"

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
    memcpy(msj_enviar.arguments, respuesta, sizeof(respuesta));

    PaqueteDatagrama paquete_enviar(sizeof(msj_enviar));
    paquete_enviar.inicializaDatos((char*)&msj_enviar);

    socketlocal->envia(paquete_enviar);

    int id_aux;
    memcpy(&id_aux, msj_enviar.arguments, sizeof(int));
    printf("\t\tRespuesta enviada: %d.\n", id_aux);
}