/* subscriber.c  –  Recebe JSON puro e bloqueia replay */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "MQTTClient.h"      /* libpaho-mqtt3c */

#define HOST    "tcp://127.0.0.1:1883"
#define TOPIC   "escola/sala1/temperatura"
#define USER    "aluno"
#define PASSWD  "senha123"

static uint32_t ultima_ts = 0;

/* ------------------------------------------------------------------ */
/* Callback de mensagem                                               */
/* ------------------------------------------------------------------ */
static int msg_cb(void *ctx, char *topic, int tlen, MQTTClient_message *msg)
{
    char plain[64];

    /* 1. Copia payload puro */
    memcpy(plain, msg->payload, msg->payloadlen);
    plain[msg->payloadlen] = '\0';

    printf("Recebido: %s\n", plain);

    /* 2. Extrai valor e timestamp */
    float    valor;
    uint32_t ts;
    if (sscanf(plain, "{\"valor\":%f,\"ts\":%u}", &valor, &ts) != 2) {
        puts("Parse ERROR");
        goto done;
    }

    /* 3. Anti-replay */
    if (ts > ultima_ts) {
        ultima_ts = ts;
        printf("Nova leitura: %.1f  (ts=%u)\n", valor, ts);
    } else {
        printf("Replay detectado! ts=%u <= %u\n", ts, ultima_ts);
    }

done:
    MQTTClient_freeMessage(&msg);
    return 1;                   /* mensagem tratada */
}

/* ------------------------------------------------------------------ */
int main(void)
{
    MQTTClient cli;
    MQTTClient_create(&cli, HOST, "pc_sub",
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTClient_setCallbacks(cli, NULL, NULL, msg_cb, NULL);

    MQTTClient_connectOptions co = MQTTClient_connectOptions_initializer;
    co.username = USER;
    co.password = PASSWD;

    printf("Tentando conectar ao broker: %s\n", HOST);
    int rc = MQTTClient_connect(cli, &co);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Erro ao conectar no broker. Código de erro: %d\n", rc);
        return 1;
    }

    puts("Conectado ao broker!");

    MQTTClient_subscribe(cli, TOPIC, 0);
    puts("Inscrito no tópico!");

    while (1) MQTTClient_yield();

    return 0;
}
