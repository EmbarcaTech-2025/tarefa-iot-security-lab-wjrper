#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "include/wifi_conn.h"
#include "include/mqtt_comm.h"
#include "include/xor_cipher.h"  
#include <time.h>

int main(void)
{
    stdio_init_all();
    connect_to_wifi("PIZZIOLO_2G", "revil2301revil2301");          // Wi-Fi
    mqtt_setup("bitdog1", "192.168.15.5", "aluno", "senha123");    // Broker

    const char *topic = "escola/sala1/temperatura";
    char plain[64];

    while (true) {
        int len = snprintf(plain, sizeof plain,
                           "{\"valor\":26.5,\"ts\":%lu}", time(NULL));

        // ✅ Publica JSON puro (sem cifrar)
        mqtt_comm_publish(topic, (uint8_t*)plain, len);

        //Se quiser cifrar, descomenta abaixo:
        /*
        uint8_t enc[64];
        const uint8_t XOR_KEY = 42;
        xor_encrypt((uint8_t*)plain, enc, len, XOR_KEY);
        mqtt_comm_publish(topic, enc, len);
        */

        sleep_ms(5000);
    }
}
