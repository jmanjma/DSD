#include <stdio.h>
#include <stdlib.h>         // atoi
using namespace std;

#include "Solicitud.h"
#include "SocketDatagrama.h"
#include <time.h>           // time
#include <string.h>         // bzero

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if(argc != 5){
		printf("Forma de uso: %s <ip_serv> <puerto_serv> <archivo_origen> <no_votos>\n", argv[0]);
		exit(0);
	}

    Solicitud sol = Solicitud();
    srand(time(NULL));
    char* argumentos = new char[4000];

    char *ip = argv[1];
    int puerto = atoi(argv[2]);
    char *name_file = argv[3];
    int n = atoi(argv[4]);
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
    sol.doOperation(ip, puerto, id++, 0, argumentos, bandera);
    while((nbytes = read(file_in, (char*)&reg, sizeof(reg))) == sizeof(reg) && id<=n) {
        bzero(argumentos, 4000);
        memcpy(argumentos, (char*)&reg, sizeof(char)*4000);
        id_res = sol.doOperation(ip, puerto, 1, id, argumentos, bandera);
        // printf("\t\tlocal(%d) vs remoto(%d)\n", id, id_res);
        if (id_res<id) {
            lseek(file_in, -34, SEEK_CUR);
            bandera = false;
            continue;
        }
        bandera = true;
        // if (id_res!=id) {
        //     lseek(file_in, -34, SEEK_CUR);
        //     continue;
        //     // printf("inconsistencia! => local(%d) vs remoto(%d)\n", id, id_res);
        //     // exit(1);
        // }
        id++;
    }

    return 0;
}