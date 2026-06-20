#include <Arduino.h>
#include <TFT_eSPI.h>

// Definição dos pinos dos botões (Ajuste se necessário)
#define BOTAO_ACAO 12  // Botão 1: Enter / Voltar
#define BOTAO_NEXT 13  // Botão 2: Navegar (Descer)

TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite telaVirtual = TFT_eSprite(&tft); // Instancia a tela virtual (Buffer)

// Estados do menu
enum Telas { TELA_MENU, TELA_STATUS, TELA_CONFIGS, TELA_SOBRE };
Telas telaAtual = TELA_MENU; 

const char* menuItens[] = {"1. STATUS", "2. CONFIGS", "3. SOBRE"};
const int TOTAL_ITENS = 3;
int itemSelecionado = 0;   
volatile bool atualizarTela = true; // Flag compartilhada entre as tarefas

// Protótipo da tarefa do FreeRTOS
void tarefaBotoes(void *pvParameters);

// --- 1. TELA: MENU PRINCIPAL (Renderizado via Sprite) ---
void desenharMenuPrincipal() {
  telaVirtual.fillSprite(TFT_BLACK); // Limpa o buffer virtual em vez da tela real
  
  telaVirtual.setTextSize(1); 
  telaVirtual.setTextColor(TFT_YELLOW); 
  telaVirtual.setTextDatum(TC_DATUM);
  telaVirtual.drawString("MENU PRINCIPAL", telaVirtual.width() / 2, 6);
  telaVirtual.drawFastHLine(10, 18, telaVirtual.width() - 20, TFT_YELLOW);

  telaVirtual.setTextDatum(ML_DATUM);
  for (int i = 0; i < TOTAL_ITENS; i++) {
    int posY = 40 + (i * 24); 
    if (i == itemSelecionado) {
      // Desenha o retângulo de seleção azul
      telaVirtual.fillRect(10, posY - 10, telaVirtual.width() - 20, 20, TFT_BLUE);
      telaVirtual.setTextColor(TFT_WHITE, TFT_BLUE);
    } else {
      telaVirtual.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    telaVirtual.drawString(menuItens[i], 20, posY);
  }

  // Envia o bloco de memória pronto direto para o display físico (Origem 0,0)
  telaVirtual.pushSprite(0, 0); 
}

// --- 2. TELA: STATUS (Renderizado via Sprite) ---
void desenharTelaStatus() {
  telaVirtual.fillSprite(tft.color565(10, 40, 10)); // Fundo verde escuro no buffer
  
  telaVirtual.setTextSize(1); 
  telaVirtual.setTextColor(TFT_WHITE); 
  telaVirtual.setTextDatum(TC_DATUM);
  telaVirtual.drawString("[ 1. TELA DE STATUS ]", telaVirtual.width() / 2, 10);
  
  // Reseta o datum para TL_DATUM para o print processar o \n corretamente
  telaVirtual.setTextDatum(TL_DATUM); 
  telaVirtual.setCursor(35, 45); 
  telaVirtual.setTextSize(2);
  telaVirtual.print("CPU: 42%\nRAM: OK");

  telaVirtual.pushSprite(0, 0); // Joga na tela
}

// --- 3. TELA: CONFIGURAÇÕES (Renderizado via Sprite) ---
void desenharTelaConfigs() {
  telaVirtual.fillSprite(tft.color565(40, 10, 10)); // Fundo vermelho escuro no buffer
  
  telaVirtual.setTextSize(1); 
  telaVirtual.setTextColor(TFT_WHITE); 
  telaVirtual.setTextDatum(TC_DATUM);
  telaVirtual.drawString("[ 2. CONFIGURACOES ]", telaVirtual.width() / 2, 10);
  
  telaVirtual.setTextDatum(TL_DATUM);
  telaVirtual.setCursor(20, 50);
  telaVirtual.setTextSize(1);
  telaVirtual.print("Use os botoes para\nalterar o brilho...");

  telaVirtual.pushSprite(0, 0); // Joga na tela
}

// --- 4. TELA: SOBRE (Renderizado via Sprite) ---
void desenharTelaSobre() {
  telaVirtual.fillSprite(TFT_NAVY); // Fundo azul marinho no buffer
  
  telaVirtual.setTextSize(1); 
  telaVirtual.setTextColor(TFT_YELLOW); 
  telaVirtual.setTextDatum(TC_DATUM);
  telaVirtual.drawString("[ 3. SOBRE ]", telaVirtual.width() / 2, 10);
  
  telaVirtual.setTextDatum(TL_DATUM); 
  telaVirtual.setCursor(15, 42); 
  telaVirtual.setTextColor(TFT_WHITE);
  telaVirtual.print("ESP32 + TFT_eSPI\nVersao 1.0.0\nCriado por Wilhan A.");

  telaVirtual.pushSprite(0, 0); // Joga na tela
}

// --- SETUP PRINCIPAL ---
void setup() {
  tft.init();
  tft.setRotation(3); // Inverte para Modo Paisagem (160x128)
  tft.fillScreen(TFT_BLACK);

  // Aloca a memória RAM (~40KB) para o buffer de double buffering
  telaVirtual.createSprite(160, 128); 

  // Configuração dos botões físicos com pull-up interno
  pinMode(BOTAO_ACAO, INPUT_PULLUP);
  pinMode(BOTAO_NEXT, INPUT_PULLUP);

  // Criação da tarefa independente no FreeRTOS para leitura dos botões
  xTaskCreate(
    tarefaBotoes,    // Função
    "LerBotoes",     // Nome
    2048,            // Stack size
    NULL,            // Parâmetros
    1,               // Prioridade
    NULL             // Handle
  );
}

// --- LOOP PRINCIPAL (Tarefa de Renderização da Tela) ---
void loop() {
  if (atualizarTela) {
    switch (telaAtual) {
      case TELA_MENU:    desenharMenuPrincipal(); break;
      case TELA_STATUS:  desenharTelaStatus();    break;
      case TELA_CONFIGS: desenharTelaConfigs();   break;
      case TELA_SOBRE:   desenharTelaSobre();     break;
    }
    atualizarTela = false; 
  }
  
  vTaskDelay(pdMS_TO_TICKS(5)); // Pausa curta para gerenciamento de contexto do FreeRTOS
}

// --- TAREFA FREERTOS: MONITORAMENTO DOS BOTÕES ---
void tarefaBotoes(void *pvParameters) {
  bool ultimoEstadoAcao = HIGH;
  bool ultimoEstadoNext = HIGH;

  while(1) {
    bool estadoAcao = digitalRead(BOTAO_ACAO);
    bool estadoNext = digitalRead(BOTAO_NEXT);

    // Lógica do Botão de Ação (Aperto detectado na transição de HIGH para LOW)
    if (estadoAcao == LOW && ultimoEstadoAcao == HIGH) {
      if (telaAtual == TELA_MENU) {
        if (itemSelecionado == 0) telaAtual = TELA_STATUS;
        if (itemSelecionado == 1) telaAtual = TELA_CONFIGS;
        if (itemSelecionado == 2) telaAtual = TELA_SOBRE;
      } else {
        telaAtual = TELA_MENU; // Se estiver em uma sub-tela, volta para o menu
      }
      atualizarTela = true;
      vTaskDelay(pdMS_TO_TICKS(150)); // Debounce por software para evitar repetição
    }
    ultimoEstadoAcao = estadoAcao;

    // Lógica do Botão Próximo (Aperto detectado na transição de HIGH para LOW)
    if (estadoNext == LOW && ultimoEstadoNext == HIGH) {
      if (telaAtual == TELA_MENU) {
        itemSelecionado = (itemSelecionado + 1) % TOTAL_ITENS; // Efeito carrossel
        atualizarTela = true;
      }
      vTaskDelay(pdMS_TO_TICKS(150)); // Debounce
    }
    ultimoEstadoNext = estadoNext;

    vTaskDelay(pdMS_TO_TICKS(20)); // Libera processador por 20ms antes de ler de novo
  }
}