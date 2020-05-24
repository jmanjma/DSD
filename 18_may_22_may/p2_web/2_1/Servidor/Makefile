all: servidor.cpp Respuesta.o SocketDatagrama.o PaqueteDatagrama.o server.o
	g++ -std=c++11 servidor.cpp Respuesta.o SocketDatagrama.o PaqueteDatagrama.o server.o mongoose.c -o servidor -lpthread
Respuesta.o: Respuesta.cpp Respuesta.h
	g++ Respuesta.cpp -c
SocketDatagrama.o: SocketDatagrama.cpp SocketDatagrama.h
	g++ SocketDatagrama.cpp -c
PaqueteDatagrama.o: PaqueteDatagrama.cpp  PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c
server.o: server.cpp  server.h
	g++ server.cpp -c
clear:
	rm -rf *.o