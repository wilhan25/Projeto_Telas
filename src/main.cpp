#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Largura_Tela 128
#define Comprimento_Tela 64
#define SDA_PIN 13
#define SCL_PIN 14

Adafruit_SSD1306 display(Largura_Tela,Comprimento_Tela, &Wire, -1);

void setup(){
  Wire.begin(13,14);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    for(;;);
  }
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(4,4);
  display.print("-- CONFIGURACOES --");

  display.fillRoundRect(4,18,120,14,4,SSD1306_WHITE);

  display.setTextColor(SSD1306_BLACK);
  display.setCursor(10,21);
  display.print(">1. SENSOR DE TEMP");

  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,36);
  display.print(" 2. CONTROLE DE LED");

  display.setCursor(10,51);
  display.print(" 3, SPACE IMPACT :)");

  display.display();
}

void loop(){

}