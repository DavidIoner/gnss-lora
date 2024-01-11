//inclusão de bibliotecas
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "heltec.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LoRa.h>

#define BAND    915E6  //Escolha a frequência
int test = 0;
String JSONData;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(16, 17);  // RX, TX - ajuste os pinos conforme necessário

String horaAnterior = "";

String toJSON(float latitude, float longitude, String hora) {
    StaticJsonDocument<200> doc;
    doc["latitude"] = latitude;
    doc["longitude"] = longitude;
    doc["hora"] = hora;

    // Serializar o JSON em uma string
    String jsonStr;
    serializeJson(doc, jsonStr);
    return jsonStr;
}


String packet;

int count = 0;

void sendPacket(String data) {
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();
}

void taskReadGNSS(void *pvParameters) {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();
        String hora = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
        if (hora != horaAnterior) {
          // Serial.println("--------------------");
          JSONData = toJSON((latitude, 5), (longitude, 5), hora);
          sendPacket(JSONData);
          // Serial.println(JSONData);
        }

        horaAnterior = hora;
      }
    }
  }
}

void boardSetup() {
  Heltec.begin(true /*Display*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, BAND /*BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->clear();
  Heltec.display->drawString(33, 5, "Iniciado");
  Heltec.display->drawString(10, 30, String(test));
  Heltec.display->display();
}
/******************* função principal (setup) *********************/
void setup()
{
  // Serial.begin(9600);
  gpsSerial.begin(9600);
  boardSetup();
  test++;
  xTaskCreatePinnedToCore(taskReadGNSS, "taskReadGNSS", 1000, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(taskCore1_1s, "TaskCore1_1s", 1000, NULL, 1, NULL, 1);

  delay(1000);
  
}
// ---------- loop desabilitado ---------- //
void loop(){}