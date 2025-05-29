[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/G8V_0Zaq)

# Tarefa: IoT Security Lab - EmbarcaTech 2025

Autor: Wagner Junior e Pedro Oliveira

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Brasília, 29 de Maio 2025

---

# IoT Security Lab – BitDogLab + MQTT + XOR + Anti-Replay

Projeto desenvolvido para o laboratório de segurança em sistemas embarcados utilizando a placa **BitDogLab**, comunicação via **MQTT**, mensagens em **JSON**, cifra **XOR** e proteção contra ataques de **replay**.

## 🔧 Tecnologias Utilizadas

- [Raspberry Pi Pico W (BitDogLab)]
- MQTT (Mosquitto Broker)
- C (firmware + subscriber)
- Lib Paho MQTT C (no subscriber)
- VS Code + PlatformIO
- WireShark (análise de tráfego)

---

## 📡 Objetivo do Projeto

Garantir uma comunicação segura entre dispositivos IoT, publicando dados com:

- Autenticação no broker MQTT
- Timestamp embutido para evitar mensagens repetidas (replay)
- Mensagens em formato JSON
- Cifra XOR simples para simular criptografia de payload

---

## 📁 Estrutura do Projeto

```
📦 tarefa-iot-security-lab-wjrper
├── bitdog-firmware/
│   ├── iot_security_lab.c          # Publicador na BitDogLab
│   ├── include/
│   │   ├── mqtt_comm.h
│   │   ├── wifi_conn.h
│   │   └── xor_cipher.h
│   ├── src/
│   │   ├── mqtt_comm.c
│   │   ├── wifi_conn.c
│   │   └── xor_cipher.c
├── host/
│   ├── subscriber.c                # Subscriber no PC
│   └── subscriber.exe              # Executável gerado
├── README.md                       # Este arquivo

```

---

## 🚀 Como Executar

### 📌 Pré-requisitos

- Mosquitto instalado: https://mosquitto.org
- libpaho-mqtt3c instalada (para compilar o subscriber)
- VS Code + extensões para C/C++ ou PlatformIO (firmware)
- Conexão Wi-Fi local

---

### 1. Iniciar o Broker Mosquitto

```bash
mosquitto -c "C:\Program Files (x86)\mosquitto\mosquitto.conf" -v
```

> A porta padrão usada é `1883`. O broker aceita conexões com usuário `aluno` e senha `senha123`.

---

### 2. Subir o código na BitDogLab (Publisher)

Arquivo: `iot_security_lab.c`

Publica a cada 5 segundos uma mensagem:

```json
{"valor":26.5,"ts":1716589200}
```

> Pode ser enviada em claro ou cifrada com XOR.

---

### 3. Executar o Subscriber no PC

```bash
cd host
gcc subscriber.c xor_cipher.c -lpaho-mqtt3c -o subscriber.exe
./subscriber.exe
```

> O subscriber decifra (se necessário), faz o parse da mensagem JSON e rejeita mensagens repetidas com timestamp antigo.

---

## 🧪 Exemplo de Saída no Subscriber

```
Payload cifrado recebido: 7B 22 76 61 6C 6F 72 22 3A 32 36 2E 35 2C 22 74 73 22 3A 31 37 31 36 35 38 39 32 30 30 7D
Decifrado: {"valor":26.5,"ts":1716589200}
Nova leitura: 26.5  (ts=1716589200)
```

> Em caso de replay:
```
Replay detectado! ts=1716589200 <= 1716589200
```

---

## 📊 Possibilidades de Escalabilidade

- Uso de tópicos diferentes por sala/ambiente:
  - `escola/sala1/temp`
  - `escola/lab1/presenca`
- Cada BitDogLab pode ter seu usuário no broker
- Dashboard em Node-RED ou Grafana para visualização
- Substituir XOR por TLS ou criptografia robusta (AES)

---

## 📜 Licença
GNU GPL-3.0.
