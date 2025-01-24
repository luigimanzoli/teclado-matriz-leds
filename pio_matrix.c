#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "pio_matrix.pio.h"

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 9


//vetor para criar imagem na matriz de led - 1
double desenho_apagado[25] = {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
};


double desenho_mt_pequeno[25] = {
    0.0, 0.1, 0.0, 0.1, 0.0,
    0.1, 0.0, 0.1, 0.0, 0.1,
    0.1, 0.0, 0.0, 0.0, 0.1,
    0.0, 0.1, 0.0, 0.1, 0.0,
    0.0, 0.0, 0.1, 0.0, 0.0
};
double desenho_pequeno[25] = {
    0.0, 0.3, 0.0, 0.3, 0.0,
    0.3, 0.0, 0.3, 0.0, 0.3,
    0.3, 0.0, 0.0, 0.0, 0.3,
    0.0, 0.3, 0.0, 0.3, 0.0,
    0.0, 0.0, 0.3, 0.0, 0.0
};

double desenho_medio[25] = {
    0.0, 0.5, 0.0, 0.5, 0.0,
    0.5, 0.0, 0.5, 0.0, 0.5,
    0.5, 0.0, 0.0, 0.0, 0.5,
    0.0, 0.5, 0.0, 0.5, 0.0,
    0.0, 0.0, 0.5, 0.0, 0.0
};

double desenho_grande[25] = {
    0.0, 0.7, 0.0, 0.7, 0.0,
    0.7, 0.0, 0.7, 0.0, 0.7,
    0.7, 0.0, 0.0, 0.0, 0.7,
    0.0, 0.7, 0.0, 0.7, 0.0,
    0.0, 0.0, 0.7, 0.0, 0.0
};

double desenho_mt_grande[25] = {
    0.0, 0.9, 0.0, 0.9, 0.0,
    0.9, 0.0, 0.9, 0.0, 0.9,
    0.9, 0.0, 0.0, 0.0, 0.9,
    0.0, 0.9, 0.0, 0.9, 0.0,
    0.0, 0.0, 0.9, 0.0, 0.0
};
// Declaração dos pinos das colunas e linhas do teclado matricial
const uint8_t colunas[4] = {4, 3, 2, 1};
const uint8_t linhas[4] = {5, 6, 7, 8};

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor vermelha para cada LED
        valor_led = matrix_rgb(0.0, desenho[24 - i], 0.0); // Apenas o valor vermelho está ativo
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}


const char teclado[4][4] = {
    {'1', '2', '3', 'A'}, 
    {'4', '5', '6', 'B'}, 
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
// Protótipo da função para leitura do teclado
char leitura_teclado();

// Inicializa o sistema de clock
void inicializar_clock() {
    bool ok = set_sys_clock_khz(128000, false);
    if (ok) {
        printf("Clock set to %ld\n", clock_get_hz(clk_sys));
    } else {
        printf("Falha ao configurar o clock\n");
    }
}

// Configura os pinos do teclado matricial
void configurar_teclado(const uint *colunas, const uint *linhas) {
    for (int i = 0; i < 4; i++) {
        gpio_init(colunas[i]);
        gpio_set_dir(colunas[i], GPIO_OUT);
        gpio_put(colunas[i], 1); // Colunas inicialmente em nível alto
    }

    for (int i = 0; i < 4; i++) {
        gpio_init(linhas[i]);
        gpio_set_dir(linhas[i], GPIO_IN);
        gpio_pull_up(linhas[i]); // Linhas com pull-up ativado
    }
}

// Configura a PIO
void configurar_pio(PIO pio, uint *offset, uint *sm) {
    *offset = pio_add_program(pio, &pio_matrix_program);
    *sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, *sm, *offset, OUT_PIN);
}

// Realiza a animação do coração batendo
void animacao_coracao(double *desenho_mt_pequeno, double *desenho_pequeno, double *desenho_medio, double *desenho_grande, 
                      double *desenho_mt_grande, double *desenho_apagado, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int j = 0; j < 6; j++) {
        desenho_pio(desenho_mt_pequeno, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_pio(desenho_pequeno, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_pio(desenho_medio, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_pio(desenho_grande, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_pio(desenho_mt_grande, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
    }
    desenho_pio(desenho_apagado, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
}

// Função principal
int main() {
    PIO pio = pio0;
    uint offset, sm;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;

    // Colunas e linhas do teclado
    const uint colunas[4] = {2, 3, 4, 5}; // Ajuste os números dos pinos conforme necessário
    const uint linhas[4] = {6, 7, 8, 9}; // Ajuste os números dos pinos conforme necessário

    // Inicializa clock, stdio e configurações
    stdio_init_all();
    inicializar_clock();
    configurar_teclado(colunas, linhas);
    configurar_pio(pio, &offset, &sm);

    printf("Sistema inicializado. Aguardando entrada...\n");

    while (true) {
        char tecla = leitura_teclado(); // Leitura do teclado

        switch (tecla) {
            case '1': // Caso o usuário aperte "1"
                animacao_coracao(desenho_mt_pequeno, desenho_pequeno, desenho_medio, desenho_grande, 
                                 desenho_mt_grande, desenho_apagado, valor_led, pio, sm, r, g, b);
                break;

            default: // Para outras teclas ou nenhuma tecla pressionada
                desenho_pio(desenho_apagado, valor_led, pio, sm, r, g, b);
                break;
        }

        sleep_ms(200); // Atraso para evitar múltiplas leituras
    }

    return 0;
}
// Função para leitura do teclado matricial
char leitura_teclado() {
    char numero = 'n'; // Valor padrão caso nenhuma tecla seja pressionada

    for (int coluna = 0; coluna < 4; coluna++) {
        gpio_put(colunas[coluna], 0); // Coluna ativa em nível baixo

        for (int linha = 0; linha < 4; linha++) {
            if (gpio_get(linhas[linha]) == 0) {
                numero = teclado[3 - linha][coluna]; // Mapeamento da tecla pressionada
                while (gpio_get(linhas[linha]) == 0) {
                    sleep_ms(10); // Espera enquanto a tecla estiver pressionada
                }
                break;
            }
        }

        gpio_put(colunas[coluna], 1); // Coluna desativada em nível alto

        if (numero != 'n') {
            break; // Sai do loop se uma tecla foi pressionada
        }
    }

    return numero; // Retorna a tecla pressionada
}
