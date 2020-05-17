#ifndef PAQUETEDATAGRAMA_H_
#define PAQUETEDATAGRAMA_H_

#include <stdio.h>      // printf
#include <string.h>     // memcpy

using namespace std;

class PaqueteDatagrama {
    private:
        char* datos;                // Almacena los datos
        unsigned int longitud;      // Almacena la longitud de la cadena de datos
        char ip[16];                // Almacena la IP
        int puerto;                 // Almacena el puerto
    public:
        PaqueteDatagrama(char *, unsigned int, char *, int);
        PaqueteDatagrama(unsigned int);
        ~PaqueteDatagrama();
        char* obtieneDireccion();
        unsigned int obtieneLongitud();
        int obtienePuerto();
        char* obtieneDatos();
        void inicializaPuerto(int);
        void inicializaIp(char*);
        void inicializaDatos(char*);
};

#endif