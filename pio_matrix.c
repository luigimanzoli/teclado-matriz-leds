#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"

//arquivo .pio
#include "pio_matrix.pio.h"


void npSetLED(int index, uint8_t r, uint8_t g, uint8_t b);
void npWrite(PIO pio, uint sm);

// Defina o pino do buzzer
#define PIN_BUZZER 21
const uint32_t freq = 350; // Frequência do buzzer em Hz

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 9

//todos apagados
double desenho_apagado[25] = {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
};


//ativando todas as cores
double luz_total[25] = {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
};
//ativando todas as cores com 80% de intensidade
double luz_80_total[25] = {
    0.8, 0.8, 0.8, 0.8, 0.8,
    0.8, 0.8, 0.8, 0.8, 0.8,
    0.8, 0.8, 0.8, 0.8, 0.8,
    0.8, 0.8, 0.8, 0.8, 0.8,
    0.8, 0.8, 0.8, 0.8, 0.8
};
//ativando todas as cores com 50% de intensidade
double luz_50_total[25] = {
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5
};

//ativando todas as cores com 20% de intensidade
double luz_20_total[25] = {
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 0.2, 0.2, 0.2
};
// Declaração dos pinos das colunas e linhas do teclado matricial
const uint8_t colunas[4] = {4, 3, 2, 1};
const uint8_t linhas[4] = {5, 6, 7, 8};

const char teclado[4][4] = {
    {'1', '2', '3', 'A'}, 
    {'4', '5', '6', 'B'}, 
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
// Protótipo da função para leitura do teclado
char leitura_teclado();

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
void desenho_apagado_total(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor nenhuma
        valor_led = matrix_rgb(0.0, 0.0, 0.0); // Nenhum ativo
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}
//desenhando o azul completo
void desenho_azul(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor azul para cada LED
        valor_led = matrix_rgb(desenho[24 - i], 0.0, 0.0); // Apenas o valor azul está ativo
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}

void desenho_verde(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor azul para cada LED
        valor_led = matrix_rgb(0.0, 0.0, desenho[24 - i]); // Apenas o valor verde está ativo
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}
void desenho_branco(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor azul para cada LED
        valor_led = matrix_rgb(desenho[24 - i], desenho[24 - i], desenho[24 - i]); // Todos ativos
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
    }
}
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

void emitir_sinal_sonoro() {
    gpio_set_function(PIN_BUZZER, GPIO_FUNC_PWM);
    int slice_num = pwm_gpio_to_slice_num(PIN_BUZZER); // Obtenção do slice de PWM
    
    uint32_t clk_sys = clock_get_hz(clk_sys); // Frequência do sistema
    uint16_t wrap = clk_sys / freq - 1; // Cálculo do valor de wrap
    
    pwm_set_wrap(slice_num, wrap); // Configuração do valor de wrap
    pwm_set_gpio_level(PIN_BUZZER, wrap / 2); // Configuração do nível de PWM
    pwm_set_enabled(slice_num, true); // Liga o buzzer
    
    sleep_ms(200); // Emite o som por 200 ms
    
    pwm_set_enabled(slice_num, false); // Desliga o buzzer
}

// Realiza a animação do coração batendo
void animacao_coracao(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    //desenho muito pequeno de coracao
   double desenhos_coracao[5][25] = {
//desenho com intensidade muito baixa de coracao
    {
    0.0, 0.1, 0.0, 0.1, 0.0,
    0.1, 0.0, 0.1, 0.0, 0.1,
    0.1, 0.0, 0.0, 0.0, 0.1,
    0.0, 0.1, 0.0, 0.1, 0.0,
    0.0, 0.0, 0.1, 0.0, 0.0
},
//desenho com intensidade baixa de coracao
 {
    0.0, 0.3, 0.0, 0.3, 0.0,
    0.3, 0.0, 0.3, 0.0, 0.3,
    0.3, 0.0, 0.0, 0.0, 0.3,
    0.0, 0.3, 0.0, 0.3, 0.0,
    0.0, 0.0, 0.3, 0.0, 0.0
},
//desenho com intensidade media de coracao
{
    0.0, 0.5, 0.0, 0.5, 0.0,
    0.5, 0.0, 0.5, 0.0, 0.5,
    0.5, 0.0, 0.0, 0.0, 0.5,
    0.0, 0.5, 0.0, 0.5, 0.0,
    0.0, 0.0, 0.5, 0.0, 0.0
},
//desenho com intensidade alta de coracao
{
    0.0, 0.7, 0.0, 0.7, 0.0,
    0.7, 0.0, 0.7, 0.0, 0.7,
    0.7, 0.0, 0.0, 0.0, 0.7,
    0.0, 0.7, 0.0, 0.7, 0.0,
    0.0, 0.0, 0.7, 0.0, 0.0
},
//desenho com intensidade muito alta de coracao
 {
    0.0, 0.9, 0.0, 0.9, 0.0,
    0.9, 0.0, 0.9, 0.0, 0.9,
    0.9, 0.0, 0.0, 0.0, 0.9,
    0.0, 0.9, 0.0, 0.9, 0.0,
    0.0, 0.0, 0.9, 0.0, 0.0
 }
   };


int sequencia[] = {0, 1, 2, 3, 4, 3, 2, 1, 0}; // Ordem dos desenhos do coração
    for (int j = 0; j < 3; j++) { // Quantidade de vezes que o coração bate
        for (int i = 0; i < sizeof(sequencia) / sizeof(sequencia[0]); i++) {
            desenho_pio(desenhos_coracao[sequencia[i]], valor_led, pio, sm, r, g, b);
            sleep_ms(200);
        }
        emitir_sinal_sonoro();
    }
    // Exibe o desenho apagado ao final
    desenho_pio(desenho_apagado, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
}

void animation2(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    double frame1[25] ={0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0};

    double frame2[25] ={0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0};
    
    double frame3[25] ={0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0, 
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0};
    
    double frame4[25] ={0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0};
    
    double frame5[25] ={1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0};


    for (int16_t i = 0; i < NUM_PIXELS; i++){
        valor_led = matrix_rgb(b=0.0, frame1[24-i], g=0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    sleep_ms(100);
    for (int16_t i = 0; i < NUM_PIXELS; i++){
        valor_led = matrix_rgb(b=0.0, frame2[24-i], g=0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    sleep_ms(100);
    for (int16_t i = 0; i < NUM_PIXELS; i++){
        valor_led = matrix_rgb(b=0.0, frame3[24-i], g=0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    sleep_ms(100);
    for (int16_t i = 0; i < NUM_PIXELS; i++){
        valor_led = matrix_rgb(b=0.0, frame4[24-i], g=0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    sleep_ms(100);
    for (int16_t i = 0; i < NUM_PIXELS; i++){
        valor_led = matrix_rgb(b=0.0, frame5[24-i], g=0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    sleep_ms(100);
}


void animation3(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    double frame_1[25] ={
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0
    };

    double frame_2[25] ={
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0
    };

    double frame_3[25] ={
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0
    };

    double frame_4[25] ={
         0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0
    };

    double frame_5[25] ={
         1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0
    };

    for(int i = 0; i < 5; i++){
        desenho_azul(frame_1, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_azul(frame_2, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_azul(frame_3, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_azul(frame_4, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_azul(frame_5, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
    }
    desenho_azul(desenho_apagado, valor_led, pio, sm, r, g, b);
    sleep_ms(200);

}

void todos_azul(double *luz_total, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b ){
    desenho_azul(luz_total,valor_led, pio, sm, r, g, b );
}
void todos_vermelho(double *luz_80_total, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b ){
    desenho_pio(luz_80_total, valor_led, pio, sm, r, g, b);    
}
void todos_verde(double *luz_50_total, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b ){
    desenho_verde(luz_50_total, valor_led, pio, sm, r, g, b); 
}
void todos_branco(double *luz_20_total, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b ){
    desenho_branco(luz_20_total, valor_led, pio, sm, r, g, b);
}

void cacto(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

double frame1[25] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0};
    
    double frame2[25] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0};
    
    double frame3[25] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 
        0.0, 1.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0};
    
    double frame4[25] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0};
    
    double frame5[25] = {
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0};
        
for (int frame = 0; frame < 5; frame++){
    double* frame_atual;

    if (frame==0) frame_atual = frame1;
    else if(frame == 1) frame_atual = frame2;
    else if(frame == 2) frame_atual = frame3;
    else if(frame== 3) frame_atual = frame4;
    else frame_atual = frame5;

    for (int16_t i = 0; i < NUM_PIXELS; i++){
    valor_led = matrix_rgb(0.0, 0.0, frame_atual[24-i]);
     pio_sm_put_blocking(pio, sm, valor_led);
    }
     sleep_ms(1000);
  }
}

void animation_multicolor(uint32_t valor_led, PIO pio, uint sm) {

    emitir_sinal_sonoro();


    double frames[5][25][3] = {
        // Frame 1
        {{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
         {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
         {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
         {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
         {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
        // Frame 2
        {{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
         {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
         {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
         {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0},
         {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}},
        // Frame 3
        {{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
         {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
         {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0},
         {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
         {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}},
        // Frame 4
        {{1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
         {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0},
         {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
         {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
         {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
        // Frame 5
        {{0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0},
         {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
         {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
         {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
         {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}}
    };

    for (int frame = 0; frame < 5; frame++) {
        for (int16_t i = 0; i < NUM_PIXELS; i++) {
            double r = frames[frame][i][0];
            double g = frames[frame][i][1];
            double b = frames[frame][i][2];
            valor_led = matrix_rgb(r, g, b);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(500); // Tempo de exibição de cada frame
    }
}

// Estrutura para definir um pixel RGB
typedef struct {
    uint8_t R, G, B;
} pixel_t;

// Buffer para armazenar o estado dos LEDs
pixel_t leds[NUM_PIXELS];

// Função para configurar um LED com valores RGB
void npSetLED(int index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= 0 && index < NUM_PIXELS) {
        leds[index].R = r;
        leds[index].G = g;
        leds[index].B = b;
    }
}

// Função para enviar os valores dos LEDs para a matriz
void npWrite(PIO pio, uint sm) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        uint32_t pixel_data = (leds[i].G << 24) | (leds[i].R << 16) | (leds[i].B << 8);
        pio_sm_put_blocking(pio, sm, pixel_data);
    }
    sleep_us(80); // Tempo de reset para o protocolo WS2812
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

// Função principal
int main() {
    PIO pio = pio0;
    uint offset, sm;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;

    // Colunas e linhas do teclado
    uint colunas[4] = {1,2, 3, 4}; // Ajuste os números dos pinos conforme necessário
    uint linhas[4] = { 5,6, 7, 8}; // Ajuste os números dos pinos conforme necessário

    // Inicializa clock, stdio e configurações
    stdio_init_all();
    inicializar_clock();
    configurar_teclado(colunas, linhas);
    configurar_pio(pio, &offset, &sm);

    printf("Sistema inicializado. Aguardando entrada...\n");

    while (true) {
        char tecla = leitura_teclado(); // Leitura do teclado
        if (tecla != 'n') { // Verifica se uma tecla foi pressionada
            printf("Tecla pressionada: %c\n", tecla);

        switch (tecla) {
            case '1': // Caso o usuário aperte "1" 
                animacao_coracao(valor_led, pio, sm, r, g, b);
                break;

            case '2': //caso aperte "2"
                animation2(valor_led, pio, sm, r, g, b);
                break;
                
            case '3': //caso aperte "3"
                animation_multicolor(valor_led, pio, sm);
                break;

            case '4': //caso aperte "4"
                cacto(valor_led, pio, sm, r, g, b);
                break;

            case '5': //caso aperte "5"
                animation3(valor_led, pio, sm, r, g, b);
                break;

            case 'A': //caso aperte "A"
                desenho_apagado_total(desenho_apagado, valor_led, pio, sm, r, g, b);
                break;

            case 'B': //caso aperte "B"
                todos_azul(luz_total, valor_led, pio, sm, r, g, b);
                break;

            case 'C': //caso aperte "C"
                todos_vermelho(luz_80_total, valor_led, pio, sm, r, g, b);
                break;

            case 'D': //caso aperte "D"
                todos_verde(luz_50_total, valor_led, pio, sm, r, g, b);
                break;

            case '#': //caso aperte "#"
                todos_branco(luz_20_total, valor_led, pio, sm, r, g, b);
                break;

            case '0': // Novo caso para a tecla "0"
                for (int linha = 0; linha < 2; linha++) { // Duas primeiras fileiras
                    for (int coluna = 0; coluna < 5; coluna++) { // 5 LEDs por linha
                    int posicao = linha * 5 + coluna; // Calcula a posição do LED
                    npSetLED(posicao, 0, 0, 255); // Azul: RGB = (0, 0, 255)
                    }
                }
                npWrite(pio, sm); // Atualiza os LEDs com o novo estado
                        break;

            case '*': //caso aperte *
                reset_usb_boot(0, 0);
                break;

            default: // Para outras teclas ou nenhuma tecla pressionada
                printf("Default acionado. Valor tecla: %c (ASCII: %d)\n", tecla, tecla);
                desenho_pio(desenho_apagado, valor_led, pio, sm, r, g, b);
                break;
            }
        }   
        sleep_ms(5); // Atraso para evitar múltiplas leituras
    }

    return 0;
}
