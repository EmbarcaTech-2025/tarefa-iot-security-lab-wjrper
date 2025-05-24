#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "include/wifi_conn.h"
#include "include/mqtt_comm.h"
#include "include/xor_cipher.h"
#include <time.h>

#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "wifi_conn.h"
#include "mqtt_comm.h"
#include "xor_cipher.h"
#include <time.h>

int main(void)
{
    stdio_init_all();
    connect_to_wifi("PIZZIOLO_2G", "revil2301revil2301");          // Wi-Fi OK?
    mqtt_setup("bitdog1", "192.168.15.5", "aluno", "senha123");    // broker certo?

    const char *topic      = "escola/sala1/temperatura";
    const uint8_t XOR_KEY  = 42;  // Pode deixar, mas não vai usar
    char   plain[64];

    while (true) {
        int len = snprintf(plain, sizeof plain,
                           "{\"valor\":26.5,\"ts\":%lu}", time(NULL));

        // Não cifra, publica em claro
        mqtt_comm_publish(topic, (uint8_t*)plain, len);   // ✅ publica JSON puro

        // Se quiser cifrar, descomente:
        // uint8_t enc[64];
        // xor_encrypt((uint8_t*)plain, enc, len, XOR_KEY);
        // mqtt_comm_publish(topic, enc, len);

        sleep_ms(5000);
    }
}




/* 
 * Comandos de terminal para testar o MQTT:
 * 
 * Inicia o broker MQTT com logs detalhados:
 * mosquitto -c mosquitto.conf -v
 * 
 * Assina o tópico de temperatura (recebe mensagens):
 * mosquitto_sub -h localhost -p 1883 -t "escola/sala1/temperatura" -u "aluno" -P "senha123"
 * 
 * Publica mensagem de teste no tópico de temperatura:
 * mosquitto_pub -h localhost -p 1883 -t "escola/sala1/temperatura" -u "aluno" -P "senha123" -m "26.6"
 */