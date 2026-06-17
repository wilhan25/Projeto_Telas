#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Largura_Tela 128
#define Comprimento_Tela 64
#define SDA_PIN 13
#define SCL_PIN 14

Adafruit_SSD1306 display(Largura_Tela,Comprimento_Tela, &Wire, -1);

int porcentagem = 0;

void setup(){
  Wire.begin(13,14);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    for(;;);
  }
  display.clearDisplay();
}

void loop(){
  display.clearDisplay(); //limap tela

  // (x que vai começar, y que vai começar, largura q vai ter, altura que vai ter, cor)
  display.drawRect(0,0,128,64,SSD1306_WHITE); //faz uma borda por toda tela

  display.setTextSize(1); //tamanho do texto
  display.setTextColor(SSD1306_WHITE); //cor do texto
  display.setCursor(34,12);
  display.print("CARREGANDO");

   // (x que vai começar, y que vai começar, largura q vai ter, altura que vai ter, cor)
  display.drawRect(10,34,104,12,SSD1306_WHITE);

  display.fillRect(12,36,porcentagem,8,SSD1306_WHITE);

  display.setCursor(54,50);
  display.print(porcentagem);
  display.print("%");

  display.display(); //joga oq está na memória para a tela

  porcentagem++; // Aumenta o progresso
  if (porcentagem > 100) {
    porcentagem = 0; // Quando chega em 100, reinicia o ciclo
    delay(1000);     // Espera 1 segundo antes de recomeçar
  }

  delay(30); // Controla a velocidade do carregamento

}