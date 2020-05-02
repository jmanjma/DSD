#include <stdio.h>
#include <stdlib.h>         // atoi
#include <string.h>           // len
#include <string>           // tostring
#include <fcntl.h>
#include <unistd.h>
using namespace std;

#include "Respuesta.h"

char* realiza_op(struct mensaje *msj, int &file_destino, char* name_file);

int main(int argc, char *argv[]) {
    if(argc != 3){
		printf("Forma de uso: %s <puerto_local> <archivo_origen>\n", argv[0]);
		exit(0);
	}
    char *name_file = argv[2];
    int file_destino;

    if ((file_destino = open(name_file, O_WRONLY|O_TRUNC|O_CREAT, 0666))==-1) {
        perror(name_file);
        exit(-1);
    }

    Respuesta res = Respuesta(atoi(argv[1]));
    struct mensaje *msj_recibir;
    int cont;
    char* mensaje;

    printf("\n\tServidor iniciado\n");

    cont = 0;
    while(1) {
        msj_recibir = res.getRequest();
        mensaje = realiza_op(msj_recibir, file_destino, name_file);
        res.sendReply(mensaje);
    }

    close(file_destino);

    return 0;
}

char* realiza_op(struct mensaje *msj, int &file_destino, char* name_file) {
    if (msj->operationId==0) {
        close(file_destino);
        if ((file_destino = open(name_file, O_WRONLY|O_TRUNC|O_CREAT, 0666))==-1) {
            perror(name_file);
            exit(-1);
        }
        printf("\n\t\tBase de datos inicializada.\n");
    } else {
        struct registro reg;
        memcpy(&reg, (struct registro*)msj->arguments, sizeof(reg));
        write(file_destino, &reg, sizeof(reg));
    }
    
    return (char*)"ACK";
}