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

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <iostream>

using namespace std;

#include "Respuesta.h"
#include "trie.h"
#include "server.h"

void realiza_op(struct mensaje *msj, int &file_destino, char* name_file, int &cont, Respuesta &res, struct TrieNode *root, struct timeval &last, int *lamport_local);
void print_celnums(vector<string> &vector);
int max(int a, int b);
void update_lamport(int *lamport_local, int *lamport_remoto, int id);

int main(int argc, char *argv[]) {
    if(argc != 4){
		printf("Forma de uso: %s <puerto_local> <archivo_destino> <ip_broadcast>\n", argv[0]);
		exit(0);
	}

    int lamport_local[NUM_SERVS];
    for (int i=0 ; i<NUM_SERVS ; i++)
        lamport_local[i] = 0;

    thread web(main_web, atoi(argv[1]), argv[3]);
    
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
    struct timeval last;
    struct TrieNode *root = getNode(); 

    printf("\n\tServidor iniciado\n");

    cont = 0;
    while(1) {
        msj_recibir = res.getRequest();
        realiza_op(msj_recibir, file_destino, name_file, cont, res, root, last, lamport_local);
    }

    close(file_destino);

    return 0;
}

void realiza_op(struct mensaje *msj, int &file_destino, char* name_file, int &cont, Respuesta &res, struct TrieNode *root, struct timeval &last, int *lamport_local) {
    int aux = cont;
    struct timeval t;

    if (msj->operationId==0) {
        close(file_destino);
        if ((file_destino = open(name_file, O_WRONLY|O_TRUNC|O_CREAT, 0666))==-1) {
            perror(name_file);
            exit(-1);
        }
        printf("\n\t\tBase de datos inicializada.\n");
        cont = 0;
        t.tv_sec = 0;
        t.tv_usec  = 0;
        memcpy(&last, (struct timeval*)&t, sizeof(struct timeval));
    } else if (msj->operationId==1) {
        if (cont+1==msj->requestId){
            aux = ++cont;
            struct registro reg;
            memcpy(&reg, (struct registro*)msj->arguments, sizeof(reg));
            if (search(root, reg.celular)) {
                t.tv_sec = 0;
                t.tv_usec  = 0;
            } else {
                insert(root, reg.celular);
                write(file_destino, &reg, sizeof(reg));
                gettimeofday(&t, NULL);
                memcpy(&last, (struct timeval*)&t, sizeof(struct timeval));
                update_lamport(lamport_local, msj->lamport, msj->id_serv);
            }
        } else {
            printf("\t\t**Mensaje repetido, id anterior enviado.\n");
            memcpy(&t, (struct timeval*)&last, sizeof(struct timeval));
        }
    } else if (msj->operationId==2) {
        // printf("\t\tMensaje broadcast!.\n");
        int votos_actuales = lamport_local[msj->id_serv];
        res.sendReplyBroadcast((char*)&votos_actuales);
        return;
    }
    
    res.sendReply((char*)&t, cont, msj);
}

void print_celnums(vector<string> &celnums) {
    for (string num: celnums) 
        printf("%s ", num.c_str());
    printf("\n");
}

int max(int a, int b) {
    return (a>b)?a:b;
}

void update_lamport(int *lamport_local, int *lamport_remoto, int id) {
    for (int i=0 ; i<NUM_SERVS ; i++) {
        if (i==id) {
            lamport_remoto[id] = ++lamport_local[id];
        } else {
            lamport_local[i] = max(lamport_local[i], lamport_remoto[i]);
            lamport_remoto[i] = lamport_local[i];
        }
    }
}