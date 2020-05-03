#ifndef MENSAJE_H_
#define MENSAJE_H_

#define TAM_MAX_DATA 4000

// Definicion de identificadores paraoperaciones permitiddas
#define suma 1

// Definicion del formato de mensaje
struct mensaje {
    int messageType;            // 0=Solicitud, 1=Respuesta
    unsigned int requestId;     // Identificador del mensaje
    int operationId;            // Identificador de la operacion
    char arguments[TAM_MAX_DATA];
};

#endif