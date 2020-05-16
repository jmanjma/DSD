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

int main(int argc, char *argv[]) {
    if(argc != 4){
		printf("Forma de uso: %s <archivo_entrada> <archivo_salida_1> <archivo_salida_2>\n", argv[0]);
		exit(0);
	}

    int file_in;
    int file_out[2];
    int cont, nbytes;

    // entrada
    if ((file_in = open(argv[1], O_RDONLY))==-1) {
		perror(argv[1]);
		exit(-1);
	}

    // salidas
    if ((file_out[0] = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, 0666))==-1) {
        perror(argv[2]);
        exit(-1);
    }
    if ((file_out[1] = open(argv[3], O_WRONLY|O_TRUNC|O_CREAT, 0666))==-1) {
        perror(argv[3]);
        exit(-1);
    }

    struct registro reg;
    cont = 0;
    while((nbytes = read(file_in, (char*)&reg, sizeof(reg))) == sizeof(reg)) {
        write(file_out[cont%2], &reg, sizeof(reg));
        // printf("%d(%d).- %s%s%s\n", cont%2, cont, reg.celular, reg.CURP, reg.partido);
        cont++;
    }

    close(file_in);
    close(file_out[0]);
    close(file_out[1]);

    return 0;
}