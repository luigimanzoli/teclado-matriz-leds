#include <stdio.h>
#include <stdint.h>
#include "hardware/pio.h"
#include "pico/stdlib.h"

// Definindo o número de LEDs na matriz (5x5)
#define NUM_PIXELS 25

// Desenho da estrela (5x5)
double desenho_estrela[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0
};

// Função para gerar o valor do LED em RGB
uint32_t matrix_rgb(double r, double g, double b) {
    uint32_t valor = 0;
    valor |= (uint32_t)(r * 255) << 16;  // Red
    valor |= (uint32_t)(g * 255) << 8;   // Green
    valor |= (uint32_t)(b * 255);        // Blue
    return valor;
}

// Função para desenhar a estrela
void desenho_estrela_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(desenho[24 - i], desenho[24 - i], desenho[24 - i]); // Todos os LEDs com a mesma intensidade (estrela)
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}

// Função principal
int main() {
    stdio_init_all();

    PIO pio = pio0;  // Usando PIO 0
    uint sm = 0;     // Usando o state machine 0
    double r = 1.0, g = 0.0, b = 0.0;  // Inicializando cor vermelha

    while (true) {
        // Aqui você pode adicionar lógica para capturar a tecla pressionada
        char tecla = getchar_timeout_us(0);  // Aguardando uma tecla ser pressionada

        if (tecla == '1') {
            // Caso o usuário pressione a tecla '1', desenha a estrela
            uint32_t valor_led;
            desenho_estrela_pio(desenho_estrela, valor_led, pio, sm, r, g, b);
        }
        
        // Caso você deseje adicionar mais interações para outras teclas, pode continuar com a lógica abaixo:
        // switch (tecla) {
        //     case '2':
        //         // Adicionar outro padrão ou ação
        //         break;
        //     default:
        //         // Ação padrão
        //         break;
        // }

        sleep_ms(100);  // Pausa para evitar sobrecarga no loop
    }

    return 0;

