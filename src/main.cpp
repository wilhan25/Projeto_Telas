#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// VARIÁVEIS DE MOVIMENTO
int quadradoX = 0;     // Posição horizontal inicial do quadrado
int velocidadeX = 15;   // Quantos pixels ele anda por frame (Velocidade)
int direcao = 1;       // 1 significa Direita, -1 significa Esquerda

void setup() {
  Wire.begin(13, 14);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
}

void loop() {
  // PASSO 2: Limpa o frame anterior
  display.clearDisplay();

  // PASSO 3: Desenha o quadrado na posição atual da variável 'quadradoX'
  // fillRect(X, Y, Largura, Altura, Cor)
  display.fillRect(quadradoX, 27, 10, 10, SSD1306_WHITE);

  // PASSO 4: Envia para a tela e aguarda um pouco (~30 frames por segundo)
  display.display();
  delay(30); 

  // PASSO 1: Atualiza a posição para o próximo frame
  quadradoX = quadradoX + (velocidadeX * direcao);

  // LOGICA DE COLISÃO: Bateu nas bordas?
  // O quadrado tem 10px de largura, então na direita ele bate em 128 - 10 = 118
  if (quadradoX >= 118) {
    direcao = -1; // Inverte para ir para a esquerda
  }
  if (quadradoX <= 0) {
    direcao = 1;  // Inverte para ir para a direita
  }
}