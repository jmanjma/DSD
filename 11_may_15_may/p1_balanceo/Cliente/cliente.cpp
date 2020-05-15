#include <stdio.h>
#include <stdlib.h>         // atoi
using namespace std;

#include "mensaje.h"
#include "Solicitud.h"
#include "SocketDatagrama.h"
#include <time.h>           // time
#include <string.h>         // bzero
#include <vector>

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void read_file(int file_in, struct registro &reg, char *argumentos) {
    int nbytes = read(file_in, (char*)&reg, sizeof(reg));
    if (nbytes != sizeof(reg) )
        exit(-1);
    else
        bzero(argumentos, TAM_MAX_DATA);
        memcpy(argumentos, (char*)&reg, sizeof(char)*TAM_MAX_DATA);
}

int main(int argc, char *argv[]) {
    if(argc < 5){
		printf("Forma de uso: %s <ip_serv>... <puerto_serv> <archivo_origen> <no_votos>\n", argv[0]);
		exit(0);
	}

    Solicitud sol = Solicitud();
    srand(time(NULL));
    char* argumentos = new char[TAM_MAX_DATA];

    struct id_timeval tid;

    vector<string> servs;
    servs.reserve(argc-4);
    
    int index = 0;

    for(index=1 ; index<=(argc-4) ;index++ )
        servs.push_back(argv[index]);

    int puerto = atoi(argv[index++]);
    char *name_file = argv[index++];
    int n = atoi(argv[index++]);
    int id_res;

    int file_in, nbytes;
    struct registro reg, reg_last;

    if ((file_in = open(name_file, O_RDONLY))==-1) {
		perror(name_file);
		exit(-1);
	}

    printf("\n\tEmitiendo votos...\n");

    int id = 0;
    bool bandera = true;
    for (auto it = servs.begin(); it != servs.end(); it++) {
        // sol.doOperation((*it).c_str(), puerto, 0, id, argumentos, bandera);
    }
    id++;
    while(id<=n) {
        read_file(file_in, reg, argumentos);
        if ((reg.celular[9]-48)>=0 && (reg.celular[9]-48)<5) {
            printf("%c(%d) - %s\n", reg.celular[9], reg.celular[9]-48, servs[0].c_str());
        } else {
            printf("%c(%d) - %s\n", reg.celular[9], reg.celular[9]-48, servs[1].c_str());
        }
    //     id_res = sol.doOperation(ip, puerto, 1, id, argumentos, bandera);
    //     // printf("\t\tlocal(%d) vs remoto(%d)\n", id, id_res);
    //     if (id_res<id) {
    //         lseek(file_in, -34, SEEK_CUR);
    //         bandera = false;
    //         continue;
    //     }
    //     bandera = true;
        id++;
    }

    return 0;
}

Buenas tardes profesor tengo unas dudas con respecto a la práctica de balanceo:
1. Se puede enviar a cada servido los números en orden consecutivo?
    Ej. Utilizando n numero de servidores y n numero de hilos: el requestId