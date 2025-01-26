#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define C1 5
#define C2 6
#define C3 7
#define C4 8

#define RLED_PIN 11
#define GLED_PIN 12
#define BLED_PIN 13

//arquivo .pio
#include "matriz_led.pio.h"

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 13

//botão de interupção
const uint BTNA = 26;
const uint BTNB = 27;

//vetor para criar imagem na matriz de led - 1
double desenho[25] =   {0.0, 0.3, 0.3, 0.3, 0.0,
                        0.0, 0.3, 0.0, 0.3, 0.0, 
                        0.0, 0.3, 0.3, 0.3, 0.0,
                        0.0, 0.3, 0.0, 0.3, 0.0,
                        0.0, 0.3, 0.3, 0.3, 0.0};

//vetor para criar imagem na matriz de led - 2
double desenho2[25] =   {1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0};

double frame_null[25] =     {0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0};

double frame_full[25] ={1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0};


//rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events){
    printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
    printf("HABILITANDO O MODO GRAVAÇÃO");
	reset_usb_boot(0,0); //habilita o modo de gravação do microcontrolador
}

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
        if (i%2==0)
        {
            valor_led = matrix_rgb(desenho[24-i], r=0.0, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);

        }else{
            valor_led = matrix_rgb(b=0.0, desenho[24-i], g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
}

void animation1(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

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

// ----------------------------------------------------------

void init_buttons() {
//inicializar o botão de interrupção - GPIO5
    gpio_init(BTNA);
    gpio_set_dir(BTNA, GPIO_IN);
    gpio_pull_up(BTNA);

    //inicializar o botão de interrupção - GPIO5
    gpio_init(BTNB);
    gpio_set_dir(BTNB, GPIO_IN);
    gpio_pull_up(BTNB);

    //interrupção da gpio habilitada
    gpio_set_irq_enabled_with_callback(BTNA, GPIO_IRQ_EDGE_FALL, 1, & gpio_irq_handler);
}

void init_clock() {
    bool ok;

    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    printf("iniciando a transmissão PIO");
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));
}

// Mapeamento das teclas em uma matriz 4x4
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void init_teclado() {
    for (int i = 0; i < 4; i++){

      gpio_init(R1 + i);
      gpio_set_dir(R1 + i, GPIO_OUT);
      gpio_put(R1 + i, 0);

      gpio_init(C1 + i);
      gpio_set_dir(C1 + i, GPIO_IN);
      gpio_pull_down(C1 + i);

    }}

char leitura_teclado() {
    for (int row = 0; row < 4; row++) {
        gpio_put(R1 + row, 1);

        for (int col = 0; col < 4; col++) {
            if (gpio_get(C1 + col)) {
                sleep_ms(100);
                while (gpio_get(C1 + col)){
                  gpio_put(R1 + row, 0);
                }
                return teclas[row][col];
            }
        }
        gpio_put(R1 + row, 0);
    }
    return 0;
}

int main() {

    stdio_init_all();
    init_teclado();
    init_buttons();

    PIO pio = pio0; 
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;

    //configurações da PIO
    uint offset = pio_add_program(pio, &matriz_led_program);
    uint sm = pio_claim_unused_sm(pio, true);
    matriz_led_program_init(pio, sm, offset, OUT_PIN);
    
    while (true) {
    
        char key = leitura_teclado();

        if(!gpio_get(BTNB)) //botão em nível alto
        {
            //rotina para escrever na matriz de leds com o emprego de PIO - desenho 2
            desenho_pio(desenho, valor_led, pio, sm, r, g, b);
        }
        else if (key == 'A'){
            desenho_pio(frame_null, valor_led, pio, sm, r, g, b);
            sleep_ms(100);
        }
        else if (key == 'B'){
            animation1(valor_led, pio, sm, r, g, b);
            //desenho_pio(frame_full, valor_led, pio, sm, 0.0, 0.0, 1.0);
            //sleep_ms(100);
        }
        else
        {
            //rotina para escrever na matriz de leds com o emprego de PIO - desenho 1
            desenho_pio(desenho2, valor_led, pio, sm, r, g, b);
        }

        sleep_ms(500);
        printf("\nfrequeência de clock %ld\r\n", clock_get_hz(clk_sys));
    }
}