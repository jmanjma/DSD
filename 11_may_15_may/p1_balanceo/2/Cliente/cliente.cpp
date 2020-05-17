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
#include <thread>
#include <iostream>

typedef struct Item {
    int id;
    struct registro reg;
} item;

void func_hilo(char *IP, int puerto, Solicitud &sol, vector<item> &regs) {
    int res;
    item aux;
    char *argumentos = new char[TAM_MAX_DATA];

    while(!regs.empty()) {
        memcpy(&aux, (item*)&regs[0], sizeof(item));
        memcpy(argumentos, (char*)&aux.reg, sizeof(struct registro));
        res = sol.doOperation(IP, puerto, 1, aux.id, argumentos);
        if (res!=aux.id) {
            continue;
        }
        regs.erase(regs.begin());
    }
}

int main(int argc, char *argv[]) {
    if(argc < 5){
		printf("Forma de uso: %s <ip_serv>... <puerto_serv> <archivo_origen> <no_votos>\n", argv[0]);
		exit(0);
	}

    Solicitud sol = Solicitud();
    srand(time(NULL));
    char* argumentos = new char[TAM_MAX_DATA];

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

    int file_in;
    struct registro reg, reg_last;

    if ((file_in = open(name_file, O_RDONLY))==-1) {
		perror(name_file);
		exit(-1);
	}

    printf("\n\tEmitiendo votos...\n");

    int ids[num_servs];
    vector<item> regs[num_servs];
    Solicitud sols[num_servs];
    
    for (int index=0 ; index<num_servs ; index++) {
        ids[index] = 0;
        regs[index].reserve(n);
        sols[index] = Solicitud();
    }

    for (auto it = servs.begin(); it != servs.end(); it++) {
        sol.doOperation((char*)(*it).c_str(), puerto, 0, 0, argumentos);
    }

    item aux;
    int cont = 0, nbytes;
    while((nbytes = read(file_in, (char*)&reg, sizeof(reg))) == sizeof(reg) && cont<n) {
        memcpy(&aux.reg, (struct registro*)&reg, sizeof(reg));
        // Para 1 //////////////////////////////////////////////////////////////////////////////////////////////////////
        // aux.id = ++ids[0];
        // regs[0].push_back(aux);

        // Para 2 //////////////////////////////////////////////////////////////////////////////////////////////////////
        if ((reg.celular[9]-48)>=0 && (reg.celular[9]-48)<5) {
            aux.id = ++ids[0];
            regs[0].push_back(aux);
        } else {
            aux.id = ++ids[1];
            regs[1].push_back(aux);
        }

        // Para 3 //////////////////////////////////////////////////////////////////////////////////////////////////////
        // if ((reg.celular[9]-48)>=0 && (reg.celular[9]-48)<4) {
        //     aux.id = ++ids[0];
        //     regs[0].push_back(aux);
        // } else if ((reg.celular[9]-48)>=4 && (reg.celular[9]-48)<7) {
        //     aux.id = ids[1]++;
        //     regs[1].push_back(aux);
        // } else {
        //     aux.id = ids[2]++;
        //     regs[2].push_back(aux);
        // }
        
        cont++;
    }

    // Para 1 //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // thread serv1(func_hilo, (char*)servs[0].c_str(), 7200, ref(sols[0]), ref(regs[0]));
    // serv1.join();

    // Para 2 //////////////////////////////////////////////////////////////////////////////////////////////////////////
    thread serv1(func_hilo, (char*)servs[0].c_str(), 7200, ref(sols[0]), ref(regs[0]));
    thread serv2(func_hilo, (char*)servs[1].c_str(), 7200, ref(sols[1]), ref(regs[1]));
    serv1.join();
    serv2.join();

    // Para 3 //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // thread serv1(func_hilo, (char*)servs[0].c_str(), 7200, ref(sols[0]), ref(regs[0]));
    // thread serv2(func_hilo, (char*)servs[1].c_str(), 7200, ref(sols[1]), ref(regs[1]));
    // thread serv3(func_hilo, (char*)servs[2].c_str(), 7200, ref(sols[2]), ref(regs[2]));
    // serv1.join();
    // serv2.join();
    // serv3.join();

    return 0;
}