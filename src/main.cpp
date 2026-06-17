#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
int bolaRaio = 8, bolaX,bolaY, velocX = 2,velocY = 2;

void setup() {

  tft.init(); //inicaliza a tela
  tft.setRotation(3); // seta a orientação da tela entra as opções de 0 a 3
  tft.fillScreen(TFT_BLACK); //seta o fundo para preto

  // calculo pra ficar no meio da tela no inicio
  bolaX = tft.width()/2; 
  bolaY = tft.height()/2;
}

void loop() {

  tft.fillCircle(bolaX,bolaY,bolaRaio, TFT_BLACK); //apaga a bolinha anterior

  bolaX += velocX;
  bolaY += velocY;
  
  // Se a posição X da bola passar da largura da tela (menos o raio) ou for menor que o raio:
  if(bolaX >= (tft.width()- bolaRaio) || bolaX<= bolaRaio){
    velocX = -velocX;
  }
  // Colisão no Topo ou no Fundo
  if(bolaY >=(tft.height()-bolaRaio) || bolaY <= bolaRaio){
    velocY = -velocY;
  }

  tft.fillCircle(bolaX,bolaY,bolaRaio,TFT_GREEN); //desenha a bolinha

  delay(16); //delay para ficar a 60fps

}