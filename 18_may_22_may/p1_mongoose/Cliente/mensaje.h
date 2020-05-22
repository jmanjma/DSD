#ifndef MENSAJE_H_
#define MENSAJE_H_

#define TAM_MAX_DATA 4000
#define NUM_SERVS 3

// Definicion del formato de mensaje
struct mensaje {
    int messageType;            // 0=Solicitud, 1=Respuesta
    unsigned int requestId;     // Identificador del mensaje
    int operationId;            // Identificador de la operacion
    char arguments[TAM_MAX_DATA];
    int lamport[NUM_SERVS];
    char id_serv;
};

#endif