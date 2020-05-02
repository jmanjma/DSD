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
    if(argc != 4){
		printf("Forma de uso: %s <ip_serv> <puerto_serv> <archivo_origen>\n", argv[0]);
		exit(0);
	}

    Solicitud sol = Solicitud();
    srand(time(NULL));
    char* argumentos = new char[4000];

    char *ip = argv[1];
    int puerto = atoi(argv[2]);
    char *name_file = argv[3];
    char *res;

    int file_in, nbytes;
    struct registro reg;

    if ((file_in = open(name_file, O_RDONLY))==-1) {
		perror(name_file);
		exit(-1);
	}

    printf("\n\tEmitiendo votos...\n");

    int id = 0;
    sol.doOperation(ip, puerto, 0, id++, argumentos);
    while((nbytes = read(file_in, (char*)&reg, sizeof(reg))) == sizeof(reg)) {
        bzero(argumentos, 4000);
        memcpy(argumentos, (char*)&reg, sizeof(char)*4000);
        res = sol.doOperation(ip, puerto, 1, id++, argumentos);
    }

    return 0;
}