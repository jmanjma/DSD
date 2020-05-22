#include <stdio.h>
using namespace std;

#include "SocketDatagrama.h"

int main() {
    int num[2], *res;
    SocketDatagrama socket = SocketDatagrama(0);

    num[0] = 5;
    num[1] = 2;
    PaqueteDatagrama paquete_enviar((char*)num, 2*sizeof(int), "127.0.0.1", 7200), paquete_recibir(sizeof(int));

    socket.envia(paquete_enviar);
    printf("\n\tDatos enviados \
            \n\tIP: %s \
            \n\tPuerto: %d \
            \n\tNumeros: %d y %d.", paquete_enviar.obtieneDireccion(), paquete_enviar.obtienePuerto(), num[0], num[1]);

    socket.recibe(paquete_recibir);
    res = (int*)paquete_recibir.obtieneDatos();
    printf("Respuesta recibida: %d.\n", *res);

    return 0;
}