// Inclusão das bibliotecas necessárias
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "matriz_rgb.h"

// Definição de portas e endereços do display OLED e do joystick
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDR 0x3C
#define JOYSTICK_X_PIN 26
#define BUTTON_A 5
#define BUTTON_B 6
#define LED_R 13
#define LED_B 12
#define BUZZER 21

// Tabela de frequências para as notas musicais
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784

ssd1306_t ssd;

// Variáveis globais de estado do ajuste
bool ajustando_nivel = false;
bool ajustando_temperatura = false;
bool piscou_nivel = false;
bool piscou_temperatura = false;
int nivel = 0;
int temperatura = 0;
const int nivel_min = 0;
const int nivel_max = 100;
const int temp_min = 0;
const int temp_max = 50;
int y_center = 2000;
uint pwm_b, pwm_r, pwm_buzzer;

// Callback dos botões
void botao_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A) {
        ajustando_nivel = !ajustando_nivel;
        if (ajustando_nivel) {
            ajustando_temperatura = false;
            pwm_set_gpio_level(LED_R, 0); // Apaga o LED vermelho
        }
    } else if (gpio == BUTTON_B) {
        ajustando_temperatura = !ajustando_temperatura;
        if (ajustando_temperatura) {
            ajustando_nivel = false;
            pwm_set_gpio_level(LED_B, 0); // Apaga o LED azul
        }
    }
}

// Estrutura para armazenar a melodia
int melodia[] = { NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_C5 };
int duracoes[] = { 300, 300, 300, 300, 300 };

// Função para tocar a melodia no buzzer
void alerta_buzzer() {
    for (int i = 0; i < sizeof(melodia) / sizeof(melodia[0]); i++) {
        pwm_set_gpio_level(BUZZER, 2048);  // Liga o buzzer com metade do ciclo de trabalho
        pwm_set_wrap(pwm_buzzer, 125000000 / melodia[i]); // Define a frequência da nota
        pwm_set_enabled(pwm_buzzer, true);
        
        sleep_ms(duracoes[i]);  // Mantém a nota pelo tempo determinado
        
        pwm_set_enabled(pwm_buzzer, false); // Desliga o buzzer
        sleep_ms(50);  // Pequena pausa entre as notas
    }
}


// Ajusta o nível com base na leitura do joystick
void ajustar_nivel() {
    if (ajustando_nivel) {
        adc_select_input(1);
        uint16_t y = adc_read();
        int ajuste = (y - y_center) / 500;
        nivel += ajuste;

        if (nivel < nivel_min) {
            nivel = nivel_min;
            alerta_buzzer();
        }
        if (nivel > nivel_max) {
            nivel = nivel_max;
            alerta_buzzer();
        }

        pwm_set_gpio_level(LED_B, (nivel * 4095) / nivel_max);
    }
}

// Ajusta a temperatura com base na leitura do joystick
void ajustar_temperatura() {
    if (ajustando_temperatura) {
        adc_select_input(1);
        uint16_t y = adc_read();
        int ajuste = (y - y_center) / 500;
        temperatura += ajuste;

        if (temperatura < temp_min) {
            temperatura = temp_min;
            alerta_buzzer();
        }
        if (temperatura > temp_max) {
            temperatura = temp_max;
            alerta_buzzer();
        }

        pwm_set_gpio_level(LED_R, (temperatura * 4095) / temp_max);
    }
}

// Inicializa um pino PWM e retorna o número do slice
uint pwm_init_gpio(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, 4095);
    pwm_set_enabled(slice_num, true);
    return slice_num;
}

// Inicializa os sinais PWM para LEDs e buzzer
void inicializar_pwm() {
    pwm_b = pwm_init_gpio(LED_B);
    pwm_r = pwm_init_gpio(LED_R);
    pwm_buzzer = pwm_init_gpio(BUZZER);
}

// Atualiza o display OLED com as barras de nível e temperatura
void atualizar_display() {
    ssd1306_fill(&ssd, false);
    int barra_nivel = (nivel * 128) / nivel_max;
    int barra_temp = (temperatura * 128) / temp_max;

    ssd1306_rect(&ssd, 3, 5, barra_nivel, 10, true, true);
    ssd1306_rect(&ssd, 35, 5, barra_temp, 10, true, true);

    char nivel_str[16], temp_str[16];
    sprintf(nivel_str, "Nivel: %d", nivel);
    sprintf(temp_str, "Temp:  %d", temperatura);

    ssd1306_draw_string(&ssd, nivel_str, 5, 21);
    ssd1306_draw_string(&ssd, temp_str, 5, 53);

    ssd1306_send_data(&ssd);
}

// Atualiza a matriz de LEDs com base no nível e temperatura
void atualizar_matriz_leds(int nivel, int temperatura) {
    npClear();
    int leds_ativos_nivel = (nivel * LED_COUNT) / nivel_max;
    int leds_ativos_temp = (temperatura * LED_COUNT) / temp_max;

    if (ajustando_nivel) {
        for (int i = 0; i < leds_ativos_nivel; i++) {
            int intensidade_azul = (i + 1) * (255 / LED_COUNT);
            npSetLED(i, 0, 0, intensidade_azul);
        }
        if (nivel >= nivel_max && !piscou_nivel) {
            for (int j = 0; j < 2; j++) {
                npClear();
                npWrite();
                sleep_ms(200);
                for (int i = 0; i < leds_ativos_nivel; i++) {
                    int intensidade_azul = (i + 1) * (255 / LED_COUNT);
                    npSetLED(i, 0, 0, intensidade_azul);
                }
                npWrite();
                sleep_ms(200);
            }
            piscou_nivel = true;  // Evita que pisque de novo
        } else if (nivel < nivel_max) {
            piscou_nivel = false;  // Reseta para permitir piscar novamente no futuro
        }
    }

    if (ajustando_temperatura) {
        for (int i = 0; i < leds_ativos_temp; i++) {
            int intensidade_vermelho = (i + 1) * (255 / LED_COUNT);
            npSetLED(i, intensidade_vermelho, 0, 0);
        }
        if (temperatura >= temp_max && !piscou_temperatura) {
            for (int j = 0; j < 2; j++) {
                npClear();
                npWrite();
                sleep_ms(200);
                for (int i = 0; i < leds_ativos_temp; i++) {
                    int intensidade_vermelho = (i + 1) * (255 / LED_COUNT);
                    npSetLED(i, intensidade_vermelho, 0, 0);
                }
                npWrite();
                sleep_ms(200);
            }
            piscou_temperatura = true;  // Evita que pisque de novo
        } else if (temperatura < temp_max) {
            piscou_temperatura = false;  // Reseta para permitir piscar novamente no futuro
        }
    }

    npWrite();
}

// Função principal do programa
int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    
    gpio_set_irq_enabled(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_callback(&botao_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
    
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_put(BUZZER, 0);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, 128, 64, false, OLED_ADDR, I2C_PORT);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    inicializar_pwm();
    npInit(LED_PIN);
    npClear();
    npWrite();

    
    while (true) {
        ajustar_nivel();
        ajustar_temperatura();
        atualizar_matriz_leds(nivel, temperatura);
        atualizar_display();
        printf("Nivel: %d, Temperatura: %d\n", nivel, temperatura);
        sleep_ms(100);
    }
}