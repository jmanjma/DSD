all: cliente.cpp Solicitud.o SocketDatagrama.o PaqueteDatagrama.o
	g++ -std=c++11 cliente.cpp Solicitud.o SocketDatagrama.o PaqueteDatagrama.o -o cliente -lpthread
Solicitud.o: Solicitud.cpp Solicitud.h
	g++ Solicitud.cpp -c
SocketDatagrama.o: SocketDatagrama.cpp SocketDatagrama.h
	g++ SocketDatagrama.cpp -c
PaqueteDatagrama.o: PaqueteDatagrama.cpp  PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c
clear:
	rm -rf *.o