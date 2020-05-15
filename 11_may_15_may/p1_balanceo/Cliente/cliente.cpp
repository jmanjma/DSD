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

    int num_servs = argc-4;

    vector<string> servs;
    servs.reserve(num_servs);
    
    int index = 0;

    for(index=1 ; index<=num_servs ;index++ )
        servs.push_back(argv[index]);

    int puerto = atoi(argv[index++]);
    char *name_file = argv[index++];
    int n = atoi(argv[index++]);
    int id_res[num_servs];

    int file_in, nbytes;
    struct registro reg, reg_last;

    if ((file_in = open(name_file, O_RDONLY))==-1) {
		perror(name_file);
		exit(-1);
	}

    printf("\n\tEmitiendo votos...\n");

    int id = 0;
    bool bandera = false;
    for (auto it = servs.begin(); it != servs.end(); it++) {
        sol.doOperation((char*)(*it).c_str(), puerto, 0, id, argumentos);
    }
    id++;
    while(id<=n) {
        for (int s=0 ; s<num_servs ; s++) {
            read_file(file_in, reg, argumentos);
            if ((reg.celular[9]-48)>=0 && (reg.celular[9]-48)<5) {
                id_res[0] = sol.doOperation((char*)servs[0].c_str(), puerto, 1, id, argumentos);
            } else {
                id_res[1] = sol.doOperation((char*)servs[1].c_str(), puerto, 1, id, argumentos);
            }
        }
        for (int ids=0 ; ids<num_servs ; ids++) {
            if (id_res[ids]<id) {
                bandera = true;
                break;
            }
        }
        if (bandera) {
            bandera = false;
            lseek(file_in, -34*(num_servs), SEEK_CUR);
            continue;
        }
        id++;
    }

    return 0;
}