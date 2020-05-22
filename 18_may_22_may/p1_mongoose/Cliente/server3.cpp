#define MG_ENABLE_HTTP_STREAMING_MULTIPART 1
#include "mongoose.h"
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

// Variables de socket cliente /////////////////////////////////////////////////
#include <stdlib.h>
#include "SocketDatagrama.h"

int num[2], *res;

string calc_time(struct timeval &start, struct timeval &end) {
	double res;

	res = (end.tv_sec * 1000000) + end.tv_usec;
	res -= (start.tv_sec * 1000000) + start.tv_usec;

	return std::to_string(res/1000000) + " segundos.<br>";
}

static void handle_size(struct mg_connection *nc, struct http_message *hm, string resultado) {
	char query[resultado.length()];

	// mg_get_http_var(&hm->body, "query", query, sizeof(query));
	sprintf(query, resultado.c_str(), (int)strlen(query));
	printf("\tCadena enviada: %s\n", query);

	mg_send_head(nc, 200, strlen(query), "Content-Type: text/plain");
	mg_printf(nc, "%s", query);
}

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
	char query[256];
 	struct http_message *hm = (struct http_message *) p;


	if (ev == MG_EV_HTTP_REQUEST) {
		if (mg_vcmp(&hm->uri, "/search") == 0) { 			
			mg_get_http_var(&hm->body, "query", query,sizeof(query));

			// Socket cliente //////////////////////////////////////////////////
			SocketDatagrama socket_udp(0);
			socket_udp.setBroadcast();

			struct timeval start, end;

			num[0] = 5;
			num[1] = 2;
			PaqueteDatagrama paquete_enviar((char*)num, 2*sizeof(int), query, 7200), paquete_recibir(2*sizeof(int));

			gettimeofday(&start, NULL);
			socket_udp.envia(paquete_enviar);
			printf("\n\tDatos enviados \
            \n\tIP: %s \
            \n\tPuerto: %d \
            \n\tNumeros: %d y %d.\n", paquete_enviar.obtieneDireccion(), paquete_enviar.obtienePuerto(), num[0], num[1]);

			string resultado = "IP servidor => Latencia<br>", aux;
			int n;
			for (int i=0 ; i<2 ; i++) {
				n = socket_udp.recibeTimeout(paquete_recibir, 2, 0);

				if (n>-1) {
					gettimeofday(&end, NULL);

					aux = paquete_recibir.obtieneDireccion();
					resultado += aux + " => " + calc_time(start, end);

					res = (int*)paquete_recibir.obtieneDatos();
					printf("\tRespuesta recibida: %d.\n", *res);
				}
			}
			handle_size(nc, hm, resultado); 
		}else{
			mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
		}
	}
}

int main(void) {
	struct mg_mgr mgr;
	struct mg_connection *nc;
	mg_mgr_init(&mgr, NULL);

	printf("Starting web server on port %s\n", s_http_port);
	nc = mg_bind(&mgr, s_http_port, ev_handler);
	if (nc == NULL) {
		printf("Failed to create listener\n");
		return 1;
	}
	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = "www"; // Serve current directory
	s_http_server_opts.enable_directory_listing = "yes";
	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);
	return 0;
}
