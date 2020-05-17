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
#include "trie.h"

void realiza_op(struct mensaje *msj, int &file_destino, char* name_file, int &cont, Respuesta &res, struct TrieNode *root, struct timeval &last);
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
    struct timeval last;
    struct TrieNode *root = getNode(); 

    printf("\n\tServidor iniciado\n");

    cont = 0;
    while(1) {
        msj_recibir = res.getRequest();
        realiza_op(msj_recibir, file_destino, name_file, cont, res, root, last);
    }

    close(file_destino);

    return 0;
}

void realiza_op(struct mensaje *msj, int &file_destino, char* name_file, int &cont, Respuesta &res, struct TrieNode *root, struct timeval &last) {
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
    } else {
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
            }
        } else {
            printf("\t\t**Mensaje repetido, id anterior enviado.\n");
            memcpy(&t, (struct timeval*)&last, sizeof(struct timeval));
        }
    }
    
    res.sendReply((char*)&t, cont);
}

void print_celnums(vector<string> &celnums) {
    for (string num: celnums) 
        printf("%s ", num.c_str());
    printf("\n");
}