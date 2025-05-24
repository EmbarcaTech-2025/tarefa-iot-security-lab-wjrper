/* subscriber.c  –  Recebe, decifra XOR e bloqueia replay            */
#include <stdio.h>
#include <stdint.h>
#include "MQTTClient.h"      /* libpaho-mqtt3c */
#include "xor_cipher.h"      /* mesma função usada no firmware */

#define HOST    "tcp://127.0.0.1:1883"
#define TOPIC   "escola/sala1/temperatura"
#define USER    "aluno"
#define PASSWD  "senha123"
#define XOR_KEY 42

static uint32_t ultima_ts = 0;

/* ------------------------------------------------------------------ */
/* Callback de mensagem                                               */
/*  - Precisa devolver int (Paho espera 1 = handled)                  */
/* ------------------------------------------------------------------ */
static int msg_cb(void *ctx, char *topic, int tlen, MQTTClient_message *msg)
{
    char plain[64];

    /* 1. Decifra XOR */
    xor_encrypt(msg->payload, (uint8_t *)plain, msg->payloadlen, XOR_KEY);
    plain[msg->payloadlen] = '\0';

    /* 2. Extrai valor e timestamp */
    float    valor;
    uint32_t ts;
    printf("Recebido: %s\n", plain);
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

    /* ✅ Registra o callback ANTES de conectar */
    MQTTClient_setCallbacks(cli, NULL, NULL, msg_cb, NULL);

    /* Configura opções de conexão */
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

    /* Loop infinito – callbacks rodam dentro do yield */
    while (1) MQTTClient_yield();

    return 0;
}
