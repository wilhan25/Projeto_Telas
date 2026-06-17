#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
int bolaRaio = 8, bolaX,bolaY, velocX = 3,velocY = 3;

unsigned long tempoAnteriorBola = 0;
const long intervaloBola = 16;

unsigned long tempoAnteriorTexto = 0;
const long intervaloTexto = 500;
bool mostrarTexto = true;

void setup() {

  tft.init(); //inicaliza a tela
  tft.setRotation(3); // seta a orientação da tela entra as opções de 0 a 3
  tft.fillScreen(TFT_BLACK); //seta o fundo para preto

  // calculo pra ficar no meio da tela no inicio
  bolaX = tft.width()/2; 
  bolaY = tft.height()/2;
}

void loop() {

  unsigned long tempoAtual = millis();

  if(tempoAtual - tempoAnteriorBola >= intervaloBola){
    tempoAnteriorBola = tempoAtual;

    tft.fillCircle(bolaX,bolaY,bolaRaio,TFT_BLACK);

    bolaX += velocX;
    bolaY += velocY;
    if(bolaX >= (tft.width() - bolaRaio) || bolaX <=bolaRaio){
      velocX = -velocX;
    }
    if(bolaY>= (tft.height() - bolaRaio) || bolaY <= bolaRaio){
      velocY = -velocY;
    }

    tft.fillCircle(bolaX,bolaY,bolaRaio, TFT_GREEN);
  }

  if(tempoAtual - tempoAnteriorTexto >= intervaloTexto){
    tempoAnteriorTexto = tempoAtual;

    mostrarTexto = !mostrarTexto;

    tft.setTextSize(1);
    tft.setTextDatum(BC_DATUM); //Alinhamento no centro inferior da tela

    if(mostrarTexto){
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawString("SISTEMA ATIVO", tft.width()/2, tft.height()-10);
    }else {
      // Desenha o texto com a cor preta para "esconder" ele
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.drawString("SISTEMA ATIVO", tft.width() / 2, tft.height() - 10);
    }
  }
}