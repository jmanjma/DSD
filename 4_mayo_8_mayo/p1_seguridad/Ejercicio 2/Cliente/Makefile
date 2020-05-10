all: cliente.cpp Solicitud.o SocketDatagrama.o PaqueteDatagrama.o
	g++ cliente.cpp Solicitud.o SocketDatagrama.o PaqueteDatagrama.o -o cliente
Solicitud.o: Solicitud.cpp Solicitud.h
	g++ Solicitud.cpp -c
SocketDatagrama.o: SocketDatagrama.cpp SocketDatagrama.h
	g++ SocketDatagrama.cpp -c
PaqueteDatagrama.o: PaqueteDatagrama.cpp  PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c