//inclusão de bibliotecas
#include "heltec.h"
#define BAND    915E6  //Escolha a frequência

String packet;

/* Protótipo da função */
int count = 0;

/*
  Nome da função: sendPacket
  objetivo: envia a temperatura via LoRa armazenada na variável currentTemp.
*/
void sendPacket(int count)
{
  LoRa.beginPacket();
  LoRa.print(count);
  LoRa.endPacket();
}
/******************* função principal (setup) *********************/
void setup()
{
  pinMode(LED,OUTPUT); //inicializa o LED
  
  Heltec.begin(true /*Display*/, true /*LoRa*/, true /*Serial*/, true /*PABOOST*/, BAND /*BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->clear();
  Heltec.display->drawString(33, 5, "Iniciado");
  Heltec.display->drawString(10, 30, "com Sucesso!");
  Heltec.display->display();
  delay(1000);
  
}
/******************* função em loop (loop) *********************/
void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_16);
  
  Heltec.display->drawString(30, 5, "Enviando");
  Heltec.display->drawString(33, 30, "test");
  Heltec.display->drawString(78, 30, "°C");
  Heltec.display->display();

  sendPacket(count); 
  count += 1;
}