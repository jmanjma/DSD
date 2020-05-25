using namespace std;

#include "PaqueteDatagrama.h"

PaqueteDatagrama::PaqueteDatagrama(char* _datos, unsigned int _longitud, char* _ip, int _puerto) {
    datos = new char[_longitud];
    memcpy(datos, _datos, _longitud);
    longitud = _longitud;
    memcpy(ip, _ip, sizeof(char)*16);
    puerto = _puerto;
}

PaqueteDatagrama::PaqueteDatagrama(unsigned int _longitud) {
    datos = new char[_longitud];
    longitud = _longitud;
    puerto = -1;
}

PaqueteDatagrama::~PaqueteDatagrama() {
    delete(datos);
}

char* PaqueteDatagrama::obtieneDireccion() {
    return ip;
}

unsigned int PaqueteDatagrama::obtieneLongitud() {
    return longitud;
}

int PaqueteDatagrama::obtienePuerto() {
    return puerto;
}

char* PaqueteDatagrama::obtieneDatos() {
    return datos;
}

void PaqueteDatagrama::inicializaPuerto(int _puerto) {
    puerto = _puerto;
}

void PaqueteDatagrama::inicializaIp(char* _ip) {
    memcpy(ip, _ip, sizeof(char)*16);
}

void PaqueteDatagrama::inicializaDatos(char* _datos) {
    memcpy(datos, _datos, longitud);
}