#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {

  tft.init(); //inicaliza a tela
  tft.setRotation(3); // seta a orientação da tela entra as opções de 0 a 3
  uint16_t corFundo = tft.color565(10,30,60); //uma cor para o fundo deste exemplo
  tft.fillScreen(corFundo); //seta o fundo para acor criada

  // (X, Y, Largura, Altura, Cor)
  tft.drawRect(60,44,40,40,TFT_WHITE);

  // config do texto
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, corFundo);
  tft.setCursor(45,95);
  tft.print("TESTE 1: OK");

}

void loop() {

}