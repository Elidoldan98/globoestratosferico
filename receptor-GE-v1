#include <SPI.h>
#include <LoRa.h>

// ===== LoRa (XL1278 / SX1278 433MHz) =====
#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_CS    5  // poné el mismo CS que uses en este ESP
#define LORA_RST   14
#define LORA_DIO0  26

uint32_t rxCounter = 0;
unsigned long lastRxMs = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("❌ LoRa RX NO detectado");
    while (1);
  }

  LoRa.enableCrc();
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);

  Serial.println("📥 RX listo: escuchando paquetes...");
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    String msg = "";
    while (LoRa.available()) {
      msg += (char)LoRa.read();
    }

    rxCounter++;
    lastRxMs = millis();

    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    Serial.println("========== RX LORA ==========");
    Serial.print("Paquete #"); Serial.println(rxCounter);
    Serial.print("Bytes: "); Serial.println(packetSize);
    Serial.print("Mensaje: "); Serial.println(msg);
    Serial.print("📶 RSSI: "); Serial.print(rssi); Serial.println(" dBm");
    Serial.print("✨ SNR: "); Serial.print(snr, 1); Serial.println(" dB");
    Serial.println("=============================");
  }

  // Si hace mucho no llega nada, avisamos (cada ~2s)
  if (millis() - lastRxMs > 3000) {
    Serial.println("⏳ Sin paquetes (ver antena/distancia/cableado)");
    lastRxMs = millis(); // para que no spamee demasiado
  }

  delay(200);
}
