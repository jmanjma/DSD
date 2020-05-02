#include "mensaje.h"
#include "Solicitud.h"
#include <iostream>

Solicitud::Solicitud() {
    socketlocal = new SocketDatagrama(0);
}

char* Solicitud::doOperation(char *IP, int puerto, int opId, int reqId, char *args) {
    int n;
    struct mensaje msj_enviar, *msj_recibir;

    msj_enviar.messageType = 0;
    msj_enviar.operationId = opId;
    msj_enviar.requestId = reqId;
    bzero(msj_enviar.arguments, 4000);
    memcpy(msj_enviar.arguments, args, sizeof(char)*4000);

    PaqueteDatagrama paquete_enviar((char*)&msj_enviar, sizeof(msj_enviar), IP, puerto), paquete_recibir(sizeof(msj_enviar));

    printf("\n\tDatos enviados \
            \n\t\tIP: %s \
            \n\t\tPuerto: %d \
            \n\t\tTipo: %d \
            \n\t\tId: %d \
            \n\t\tOperacion: %d \
            \n\t\tMensaje: ", paquete_enviar.obtieneDireccion(), paquete_enviar.obtienePuerto(), msj_enviar.messageType, msj_enviar.requestId, msj_enviar.operationId);
    struct registro reg;
    memcpy(&reg, (struct registro*)msj_enviar.arguments, sizeof(reg));
    printf("%s%s%s\n", reg.celular, reg.CURP, reg.partido);

    for (int i=0 ; i<7 ; i++) {
        socketlocal->envia(paquete_enviar);
        n = socketlocal->recibeTimeout(paquete_recibir, 2, 500000);

        if (n>-1) {
            msj_recibir = (struct mensaje*)paquete_recibir.obtieneDatos();
            printf("\t\tRespuesta recibida: %s.\n", msj_recibir->arguments);
            return msj_recibir->arguments;
        }
    }
    
    printf("\t\t*** Se agoto el numero de intentos.\n");
    exit(-1);
}