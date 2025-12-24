# Diagrama de Bloques — Sistema LoRa Globo Estratosférico

[CÁPSULA]
Sensores → ESP32 → LoRa TX ))))))))))))))))  ~50–100 km  → ((((((((((((((( LoRa RX → ESP32 → PC

La cápsula mide variables físicas y las transmite por radio LoRa.
La estación base recibe los paquetes y los decodifica para seguimiento y recuperación.
