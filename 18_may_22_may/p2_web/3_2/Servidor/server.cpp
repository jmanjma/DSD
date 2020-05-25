// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "server.h"
#include <iostream>

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static struct device_settings s_settings = {"value1", "value2"};

// Variables de socket cliente /////////////////////////////////////////////////
#include <stdlib.h>
#include <string>
#include "SocketDatagrama.h"
#include "mensaje.h"
struct mensaje msj_enviar, *msj_recibir;
SocketDatagrama socket_udp(0);
char *ip;
int puerto;

// static void handle_save(struct mg_connection *nc, struct http_message *hm) {
//   // Get form variables and store settings values
//   mg_get_http_var(&hm->body, "setting1", s_settings.setting1,
//                   sizeof(s_settings.setting1));
//   mg_get_http_var(&hm->body, "setting2", s_settings.setting2,
//                   sizeof(s_settings.setting2));

//   // Send response
//   mg_http_send_redirect(nc, 302, mg_mk_str("/"), mg_mk_str(NULL));
// }

static void handle_get_cpu_usage(struct mg_connection *nc) {
	// Broadcast
	socket_udp.setBroadcast();

	msj_enviar.messageType = 0;
	msj_enviar.operationId = 2;
	msj_enviar.requestId = 0;
	bzero(msj_enviar.arguments, TAM_MAX_DATA);

    PaqueteDatagrama paquete_enviar((char*)&msj_enviar, sizeof(msj_enviar), ip, puerto), paquete_recibir(sizeof(msj_enviar));

	socket_udp.envia(paquete_enviar);

    int n, res;
	string resultado = "{", aux;
    for (int i=0 ; i<NUM_SERVS ; i++) {
        n = socket_udp.recibeTimeout(paquete_recibir, 2, 0);

        if (n>-1) {
            aux = paquete_recibir.obtieneDireccion();
            msj_recibir = (struct mensaje*)paquete_recibir.obtieneDatos();
			memcpy(&res, (int*)msj_recibir->arguments, sizeof(int));
            // printf("\n\t\tRespuesta recibida: %d.\n", res);
            resultado += "\"" + aux + "\":" + std::to_string(res) + ",";
        }
    }
    resultado.back() = '}';

	// Use chunked encoding in order to avoid calculating Content-Length
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	// Output JSON object which holds CPU usage data
	mg_printf_http_chunk(nc, resultado.c_str());

	// Send empty chunk, the end of response
	mg_send_http_chunk(nc, "", 0);
}

// static void handle_ssi_call(struct mg_connection *nc, const char *param) {
//   if (strcmp(param, "setting1") == 0) {
//     mg_printf_html_escape(nc, "%s", s_settings.setting1);
//   } else if (strcmp(param, "setting2") == 0) {
//     mg_printf_html_escape(nc, "%s", s_settings.setting2);
//   }
// }

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
	struct http_message *hm = (struct http_message *) ev_data;

	switch (ev) {
		case MG_EV_HTTP_REQUEST:
			// if (mg_vcmp(&hm->uri, "/save") == 0) {
			//   handle_save(nc, hm);
			// } else 
			if (mg_vcmp(&hm->uri, "/get_cpu_usage") == 0) {
				handle_get_cpu_usage(nc);
			} else {
				mg_serve_http(nc, hm, s_http_server_opts);  // Serve static content
			}
		//  break;
		// case MG_EV_SSI_CALL:
		//   handle_ssi_call(nc, (const char*)ev_data);
		//   break;
		default:
			break;
	}
}

// static void push_data_to_all_websocket_connections(struct mg_mgr *m) {
//   struct mg_connection *c;
//   int memory_usage = (double)3;
//   // int memory_usage = (double) rand() / RAND_MAX * 100.0;
//   for (c = mg_next(m, NULL); c != NULL; c = mg_next(m, c)) {
//     if (c->flags & MG_F_IS_WEBSOCKET) {
//       mg_printf_websocket_frame(c, WEBSOCKET_OP_TEXT, "%d", memory_usage);
//     }
//   }
// }

int main_web(int _puerto, char *_ip) {
	puerto = _puerto;
	ip = _ip;

	struct mg_mgr mgr;
	struct mg_connection *nc;
	cs_stat_t st; /////

	mg_mgr_init(&mgr, NULL);
	nc = mg_bind(&mgr, s_http_port, ev_handler);
	if (nc == NULL) {
		fprintf(stderr, "Cannot bind to %s\n", s_http_port);
		exit(1);
	}

	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = "web_root";  // Set up web root directory

	if (mg_stat(s_http_server_opts.document_root, &st) != 0) {
		fprintf(stderr, "%s", "Cannot find web_root directory, exiting\n");
		exit(1);
	}

	printf("\tSeervidor web iniciado en el puerto %s\n", s_http_port);
	for (;;) {
		// static time_t last_time; //
		// time_t now = time(NULL); //
		mg_mgr_poll(&mgr, 1000);
		// if (now - last_time > 0) { //
		//   push_data_to_all_websocket_connections(&mgr); //
		//   last_time = now; //
		// } //
	}
	mg_mgr_free(&mgr);

	return 0;
}
