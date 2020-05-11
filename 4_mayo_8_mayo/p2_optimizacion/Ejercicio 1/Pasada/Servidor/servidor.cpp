#include <stdio.h>
#include <stdlib.h>         // atoi
#include <string.h>           // len
#include <string>           // tostring
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>       // strcut timeval, gettimeofday()
#include <vector>
#include <string>
#include <algorithm> 

using namespace std;

#include "Respuesta.h"

void realiza_op(struct mensaje *msj, int &file_destino, char* name_file, int &cont, Respuesta &res, vector<string> &celnums);
void print_celnums(vector<string> &vector);

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

    struct registro reg;
    vector<string> celnums;
    celnums.reserve(70000000);

    printf("\n\tServidor iniciado\n");

    cont = 0;
    while(1) {
        msj_recibir = res.getRequest();
        realiza_op(msj_recibir, file_destino, name_file, cont, res, celnums);
        // if (celnums.size()>0)
        //     printf("%s\n", celnums.at(celnums.size()-1).c_str());
        // print_celnums(celnums);
    }

    close(file_destino);

    return 0;
}

void realiza_op(struct mensaje *msj, int &file_destino, char* name_file, int &cont, Respuesta &res, vector<string> &celnums) {
    int aux = cont;
    struct id_timeval tid;

    if (msj->operationId==0) {
        close(file_destino);
        if ((file_destino = open(name_file, O_WRONLY|O_TRUNC|O_CREAT, 0666))==-1) {
            perror(name_file);
            exit(-1);
        }
        printf("\n\t\tBase de datos inicializada.\n");
        cont = 0;
        tid.id = 0;
        tid.t.tv_sec = 0;
        tid.t.tv_usec  = 0;
    } else {
        if (cont+1==msj->requestId){
            aux = ++cont;
            struct registro reg;
            memcpy(&reg, (struct registro*)msj->arguments, sizeof(reg));
            if (binary_search(celnums.begin(), celnums.end(), reg.celular)) {
                printf("0\n");
                tid.id = aux;
                tid.t.tv_sec = 0;
                tid.t.tv_usec  = 0;
            } else {
                celnums.push_back(reg.celular);
                sort(celnums.begin(), celnums.end()); 
                write(file_destino, &reg, sizeof(reg));
                fsync(file_destino);
                tid.id = aux;
                gettimeofday(&tid.t, NULL);
            }
        } else {
            printf("2\n");
            // printf("\t\t**Mensaje repetido, id anterior enviado.\n");
            tid.id = 0;
            tid.t.tv_sec = 0;
            tid.t.tv_usec  = 0;
        }
    }
    
    res.sendReply((char*)&tid);
}

void print_celnums(vector<string> &celnums) {
    for (string num: celnums) 
        printf("%s ", num.c_str());
    printf("\n");
}