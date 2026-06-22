#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

// Importa os arquivos gerados pelo EEZ Studio
#include "ui/ui.h"
#include "ui/screens.h"

TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT 128
#define PINO_POT      34  // Pino analógico do potenciômetro

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 10];

// Função ponte de renderização (LVGL -> TFT_eSPI)
void meu_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

// Tarefa do FreeRTOS: Motor Gráfico do LVGL
void tarefaLVGL(void *pvParameters) {
  while (1) {
    lv_timer_handler(); 
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

// Tarefa do FreeRTOS: O Coração do Tempo do LVGL
void tarefaTick(void *pvParameters) {
  while (1) {
    lv_tick_inc(1);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

// --- NOVA TAREFA: LEITURA DO POTENCIÔMETRO ---
void tarefaLeituraPot(void *pvParameters) {
  pinMode(PINO_POT, INPUT);

  while (1) {
    // Lê o ADC do ESP32 (0 a 4095)
    int leitura_crua = analogRead(PINO_POT);

    // Converte a leitura para uma porcentagem de 0.00 a 100.00
    float porcentagem = (leitura_crua / 4095.0) * 100.0;

    // 1. Atualiza o valor do Arco do EEZ Studio (o arco aceita inteiros de 0 a 100)
    lv_arc_set_value(objects.arco_pot, (int)porcentagem);

    // 2. Formata o texto para exibir com duas casas decimais e vírgula (ex: "42,50")
    char buffer_texto[10];
    snprintf(buffer_texto, sizeof(buffer_texto), "%05.2f", porcentagem);
    if (buffer_texto[2] == '.') {
      buffer_texto[2] = ','; // Troca o ponto pela vírgula para combinar com seu layout
    }

    // 3. Atualiza o Label do EEZ Studio dinamicamente
    lv_label_set_text(objects.valor_pot, buffer_texto);

    // Aguarda 50ms antes da próxima leitura (Estabilidade)
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void setup() {
  Serial.begin(115200);

  // Inicializa o display físico
  tft.init();
  tft.setRotation(3); 

  // Inicializa o LVGL
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = meu_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Inicializa a interface gerada pelo EEZ Studio
  ui_init();

  // Cria as tarefas no FreeRTOS
  xTaskCreate(tarefaTick, "LVGL_Tick", 1024, NULL, 3, NULL);
  xTaskCreate(tarefaLVGL, "LVGL_Task", 4096, NULL, 2, NULL);
  xTaskCreate(tarefaLeituraPot, "LerPot", 2048, NULL, 1, NULL);
}

void loop() {
  // O loop fica vazio. O FreeRTOS gerencia tudo nas tarefas paralelas.
  vTaskDelete(NULL);
}