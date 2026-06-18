#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
const char* menuItens[] = {"1. STATUS","2. CONFIGS", "3. SENSORES", "4. SOBRE"};
const int TOTAL_ITENS = 4;

int itemSelecionado = 0;
bool atualizarTela = true;

void desenharMenu(){
  tft.fillScreen(TFT_BLACK); //pinta a tela toda de preto para limpar e desenhar o proximo quadro

  // cabeçalho
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("MENU PRINCIPAL", tft.width()/2,6);
  tft.drawFastHLine(10,18,tft.width()-20, TFT_YELLOW);

  // corpo do menu
  tft.setTextDatum(ML_DATUM);
  for(int i = 0; i< TOTAL_ITENS; i++){
    int posY = 38 + (i*20);

    if(i == itemSelecionado){
      tft.fillRect(10, posY - 8, tft.width()-20,16,TFT_BLUE);
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    }
    else{
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    
    tft.drawString(menuItens[i], 20,posY);
  }
}

void setup() {
  Serial.begin(115200); // Inicializa a comunicação com o computador
  vTaskDelay(pdMS_TO_TICKS(500));
  
  tft.init();
  tft.setRotation(3); 
  tft.fillScreen(TFT_BLACK);

  Serial.println("Menu Inciado Use W e S para navegar");  
}

void loop() {
  if(atualizarTela){
    desenharMenu();
    atualizarTela = false;
  }

  if(Serial.available() > 0){
    char comando = Serial.read();

    if(comando == 's' || comando =='S'){
      itemSelecionado++;
      if(itemSelecionado >= TOTAL_ITENS){
        itemSelecionado = 0;
      }
      atualizarTela = true;
    }

    if(comando == 'w' || comando =='W'){
      itemSelecionado--;
      if(itemSelecionado < 0){
        itemSelecionado = TOTAL_ITENS -1;
      }
      atualizarTela = true;
    }
  }
}