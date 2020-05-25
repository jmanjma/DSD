#include "mensaje.h"
#include "Solicitud.h"
#include <sys/time.h>

#include <iostream>

Solicitud::Solicitud() {
    socketlocal = new SocketDatagrama(0);
}

int Solicitud::doOperation(char *IP, int puerto, int opId, int reqId, char *args, int id_serv, int *lamport, int *lamport_remoto) {
    int n;
    struct mensaje msj_enviar, *msj_recibir;
    struct timeval t;

    msj_enviar.messageType = 0;
    msj_enviar.operationId = opId;
    msj_enviar.requestId = reqId;
    msj_enviar.id_serv = id_serv;
    bzero(msj_enviar.arguments, TAM_MAX_DATA);
    memcpy(msj_enviar.arguments, args, sizeof(char)*TAM_MAX_DATA);
    memcpy(&msj_enviar.lamport, lamport, sizeof(int)*NUM_SERVS);

    PaqueteDatagrama paquete_enviar((char*)&msj_enviar, sizeof(msj_enviar), IP, puerto), paquete_recibir(sizeof(msj_enviar));

    printf("\n\tDatos enviados \
            \n\t\tIP: %s \
            \n\t\tPuerto: %d \
            \n\t\tId: %d \
            \n\t\tMensaje: ", paquete_enviar.obtieneDireccion(), paquete_enviar.obtienePuerto(), msj_enviar.requestId);
    struct registro reg;
    memcpy(&reg, (struct registro*)msj_enviar.arguments, sizeof(reg));
    printf("%s%s%s\n", reg.celular, reg.CURP, reg.partido);

    for (int i=0 ; i<7 ; i++) {
        socketlocal->envia(paquete_enviar);
        n = socketlocal->recibeTimeout(paquete_recibir, 2, 500000);

        if (n>-1) {
            msj_recibir = (struct mensaje*)paquete_recibir.obtieneDatos();
            memcpy(&t, msj_recibir->arguments, sizeof(struct timeval));
            printf("\t\tRespuesta recibida: %d => [", msj_recibir->requestId);
            for (int i=0 ; i<NUM_SERVS ; i++)
                printf(" %d", msj_recibir->lamport[i]);
            printf(" ] %ld:%ld.\n", t.tv_sec, t.tv_usec);
            memcpy(lamport_remoto, &msj_recibir->lamport, sizeof(int)*NUM_SERVS);
            return msj_recibir->requestId;
        }
    }
    
    printf("\t\t*** Se agoto el numero de intentos.\n");
    exit(-1);
}