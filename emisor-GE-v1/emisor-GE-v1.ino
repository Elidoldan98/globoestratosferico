#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <LoRa.h>

// ===== BMP =====
Adafruit_BMP280 bmp;
const float SEA_LEVEL_PRESSURE = 1013.25;

// ===== LoRa (XL1278 / SX1278 433MHz) =====
#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_CS    5   // <- el que te funcionó
#define LORA_RST   14
#define LORA_DIO0  26

unsigned long lastTime = 0;
float lastAltitude = 0;
uint32_t pktCounter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // BMP280 (I2C)
  Wire.begin(21, 22);
  if (!bmp.begin(0x76)) {
    Serial.println("❌ BMP280 no detectado");
    while (1);
  }
  Serial.println("✅ BMP280 OK");

  // LoRa (SPI)
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("❌ LoRa XL1278 NO detectado");
    while (1);
  }
  Serial.println("📡 LoRa XL1278 (433 MHz) detectado OK");

  // Parámetros recomendados para pruebas cerca (suave)
  LoRa.setTxPower(2);               // baja potencia para no castigar
  LoRa.setSpreadingFactor(7);       // rápido
  LoRa.setSignalBandwidth(125E3);   // típico
  LoRa.enableCrc();

  Serial.println("🚀 TX listo: enviando cada 2s...");
}

void loop() {
  if (millis() - lastTime >= 2000) {
    lastTime = millis();

    float tempC = bmp.readTemperature();
    float pressurePa = bmp.readPressure();
    float pressurehPa = pressurePa / 100.0;
    float altitude = bmp.readAltitude(SEA_LEVEL_PRESSURE);

    float airDensity = pressurePa / (287.05 * (tempC + 273.15));
    float verticalSpeed = (altitude - lastAltitude) / 2.0; // porque ahora es cada 2s
    lastAltitude = altitude;

    pktCounter++;

    // Serial local (debug)
    Serial.println("========== TX BMP280 ==========");
    Serial.print("#"); Serial.println(pktCounter);
    Serial.print("🌡 Temp: "); Serial.print(tempC, 2); Serial.println(" °C");
    Serial.print("📉 Presion: "); Serial.print(pressurehPa, 2); Serial.println(" hPa");
    Serial.print("🏔 Altura: "); Serial.print(altitude, 2); Serial.println(" m");
    Serial.print("🌬 Densidad: "); Serial.print(airDensity, 3); Serial.println(" kg/m3");
    Serial.print("🚀 Vel. vertical: "); Serial.print(verticalSpeed, 2); Serial.println(" m/s");

    // Paquete LoRa (texto simple)
    // Formato: CNT,T,P,A,D,V
    LoRa.beginPacket();
    LoRa.print(pktCounter);
    LoRa.print(",");
    LoRa.print(tempC, 2);
    LoRa.print(",");
    LoRa.print(pressurehPa, 2);
    LoRa.print(",");
    LoRa.print(altitude, 2);
    LoRa.print(",");
    LoRa.print(airDensity, 3);
    LoRa.print(",");
    LoRa.print(verticalSpeed, 2);
    LoRa.endPacket();  // manda

    Serial.println("📤 Paquete enviado por LoRa");
    Serial.println("=============================");
  }
}
