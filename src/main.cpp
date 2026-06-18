#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

enum Telas{
  TELA_MENU,
  TELA_STATUS,
  TELA_CONFIGS,
  TELA_SOBRE
};

Telas telaAtual = TELA_MENU;

const char* menuItens[] = {"1. STATUS", "2. CONFIGS", "3. SOBRE"};
const int TOTAL_ITENS = 3;
int itemSelecionado = 0;
bool atualizarTela = true;

void tela_MainMenu(){
  tft.fillScreen(TFT_BLACK);

  // cabeçalho
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("MENU PRINCIPAL", tft.width()/2, 6);
  tft.drawFastHLine(10,18,tft.width() - 20, TFT_YELLOW);

  // itens do menu
  tft.setTextDatum(ML_DATUM);
  for(int i = 0; i <TOTAL_ITENS; i++){
    int posY = 40 + (i*24);

    if(i == itemSelecionado){
      tft.fillRect(10, posY -10, tft.width() - 20,20,TFT_BLUE);
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    } else{
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    tft.drawString(menuItens[i],20,posY);
  }
}

void tela_Status() {
  tft.fillScreen(tft.color565(10, 40, 10)); // Fundo verde escuro
  
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("[ 1. TELA DE STATUS ]", tft.width() / 2, 10);
  
  tft.setCursor(40, 50); 
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  
  tft.println("CPU: 42%"); 
  tft.setCursor(40, 70); 
  tft.print("RAM: OK");
}

void tela_Configs() {
  tft.fillScreen(tft.color565(40, 10, 10)); // Fundo vermelho escuro
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("[ 2. CONFIGURACOES ]", tft.width() / 2, 10);
  
  tft.setTextDatum(MC_DATUM);
  tft.setTextDatum(MC_DATUM); 

tft.drawString("Use os botoes para", tft.width() / 2, (tft.height() / 2) - 10);
tft.drawString("alterar o brilho...", tft.width() / 2, (tft.height() / 2) + 10);
  
  tft.setTextColor(TFT_GOLD);
  tft.drawString("Pressione 'B' para Voltar", tft.width() / 2, tft.height() - 15);
}

void tela_Sobre(){
  tft.fillScreen(TFT_NAVY); 

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextDatum(TC_DATUM); 
  tft.drawString("[ 3. SOBRE ]", tft.width() / 2, 10);
  
  tft.setTextDatum(TL_DATUM); 
  
  tft.setCursor(15, 42); 
  tft.setTextColor(TFT_WHITE);
  tft.print("ESP32 + TFT_eSPI\nVersao 1.0.0\nCriado por Eng. Wilhan A.");
  

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_GOLD);
  tft.drawString("Pressione 'Q' para Voltar", tft.width() / 2, tft.height() - 15);
}

void setup() {
  Serial.begin(115200); // Inicializa a comunicação com o computador
  
  tft.init();
  tft.setRotation(3); 
  tft.fillScreen(TFT_BLACK);

  Serial.println("Menu Inciado Use W e S para navegar, E para confirmar, Q para Voltar");
}

void loop() {
  if(atualizarTela){
    switch (telaAtual)
    {
      case TELA_MENU:
        tela_MainMenu();
        break;
      case TELA_STATUS:
        tela_Status();
        break;
      case TELA_CONFIGS:
        tela_Configs();
        break;
      case TELA_SOBRE:
        tela_Sobre();
        break;
    }
    atualizarTela = false;
  }

  if(Serial.available() > 0){
    char comando = Serial.read();

    if(comando == 'q' || comando == 'Q'){
      if(telaAtual != TELA_MENU){
        telaAtual = TELA_MENU;
        atualizarTela = true;
      }
    }

    if(telaAtual == TELA_MENU){
      if(comando == 's' || comando == 'S'){
        itemSelecionado = (itemSelecionado + 1) % TOTAL_ITENS;
        atualizarTela = true;
      }
      if(comando == 'w' || comando == 'W'){
        itemSelecionado = (itemSelecionado -1 + TOTAL_ITENS) % TOTAL_ITENS;
        atualizarTela = true;
      }

      if(comando == 'e' || comando == 'E'){
        if(itemSelecionado == 0) telaAtual = TELA_STATUS;
        if(itemSelecionado == 1) telaAtual = TELA_CONFIGS;
        if(itemSelecionado == 2) telaAtual = TELA_SOBRE;

        atualizarTela = true;
      }
    }
  }
}