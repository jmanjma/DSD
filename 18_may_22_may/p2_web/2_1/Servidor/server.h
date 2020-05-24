#ifndef SERVER_H_
#define SERVER_H_

// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

struct device_settings {
  char setting1[100];
  char setting2[100];
};

static void handle_save(struct mg_connection *nc, struct http_message *hm);

static void handle_get_cpu_usage(struct mg_connection *nc);

static void handle_ssi_call(struct mg_connection *nc, const char *param);

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data);

static void push_data_to_all_websocket_connections(struct mg_mgr *m);

int main_web(int puerto, char *ip);

# endif