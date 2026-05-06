#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_dsp.h"
#include "esp_log.h"
#include "esp_task_wdt.h"


// --- Configuración de Pines ---
#define MOTOR_A_ENA 20
#define MOTOR_A_IN1 19
#define MOTOR_A_IN2 18
#define MOTOR_B_ENB 5
#define MOTOR_B_IN3 7
#define MOTOR_B_IN4 6
#define MIC_ADC_CHANNEL ADC_CHANNEL_9 // GPIO 10


// --- Parámetros de Procesamiento ---
#define N_SAMPLES 1024       // N para la FFT
#define SAMPLING_FREQ 10000  // fs = 10kHz (Resolución = 9.7Hz)
#define UMBRAL_RUIDO 400.0   // Ajustar según el entorno


// Buffers de procesamiento (deben estar alineados para DSP)
float fft_input[N_SAMPLES * 2]; // Formato complejo (Re, Im, Re, Im...)
float window[N_SAMPLES];


adc_oneshot_unit_handle_t adc1_handle;


void init_hardware() {
    // Configurar Motores como salida [cite: 185]
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL<<MOTOR_A_IN1) | (1ULL<<MOTOR_A_IN2) |
                        (1ULL<<MOTOR_B_IN3) | (1ULL<<MOTOR_B_IN4) |
                        (1ULL<<MOTOR_A_ENA) | (1ULL<<MOTOR_B_ENB),
    };
    gpio_config(&io_conf);
   
    // Habilitar drivers (velocidad máxima por ahora)
    gpio_set_level(MOTOR_A_ENA, 1);
    gpio_set_level(MOTOR_B_ENB, 1);


    // Inicializar ADC Unit 1 (Nuevo driver v5.x) [cite: 167]
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);


    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11,
    };
    adc_oneshot_config_channel(adc1_handle, MIC_ADC_CHANNEL, &config);


    // Inicializar funciones DSP [cite: 87]
    dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    dsps_wind_hann_f32(window, N_SAMPLES); // Generar ventana de Hann
}


void mover(int in1, int in2, int in3, int in4) {
    gpio_set_level(MOTOR_A_IN1, in1); gpio_set_level(MOTOR_A_IN2, in2);
    gpio_set_level(MOTOR_B_IN3, in3); gpio_set_level(MOTOR_B_IN4, in4);
}


void app_main(void) {
    init_hardware();
    int adc_raw;
    const char* TAG = "LAB1_AUDIO";


    while (1) {
        float mean = 0;


        // 1. Captura de muestras con remoción de offset DC
        for (int i = 0; i < N_SAMPLES; i++) {
            adc_oneshot_read(adc1_handle, MIC_ADC_CHANNEL, &adc_raw);
            fft_input[i * 2] = (float)adc_raw;
            fft_input[i * 2 + 1] = 0;
            mean += fft_input[i * 2];
            esp_rom_delay_us(1000000 / SAMPLING_FREQ);
        }
        mean /= N_SAMPLES;


        // 2. Aplicar offset y ventana de Hann
        for (int i = 0; i < N_SAMPLES; i++) {
            fft_input[i * 2] = (fft_input[i * 2] - mean) * window[i];
        }


        // 3. Ejecutar FFT [cite: 169]
        dsps_fft2r_fc32(fft_input, N_SAMPLES);
        dsps_bit_rev_fc32(fft_input, N_SAMPLES);
        dsps_cplx2re_fc32(fft_input, N_SAMPLES);


        // 4. Encontrar frecuencia dominante (ignorar bins bajos de ruido DC)
        float max_mag = 0;
        int max_idx = 0;
        for (int i = 5; i < N_SAMPLES / 2; i++) {
            float mag = sqrtf(fft_input[i*2]*fft_input[i*2] + fft_input[i*2+1]*fft_input[i*2+1]);
            if (mag > max_mag) {
                max_mag = mag;
                max_idx = i;
            }
        }


        float freq_detectada = max_idx * (float)SAMPLING_FREQ / N_SAMPLES; [cite: 173, 175]


        // 5. Lógica de Control de Movimiento [cite: 180]
        if (max_mag > UMBRAL_RUIDO) {
            ESP_LOGI(TAG, "Freq: %.1f Hz | Mag: %.1f", freq_detectada, max_mag);


            if (freq_detectada > 900 && freq_detectada < 1100) {
                mover(1, 0, 1, 0); // Adelante [cite: 181]
            } else if (freq_detectada > 1900 && freq_detectada < 2100) {
                mover(0, 1, 0, 1); // Atrás [cite: 182]
            } else if (freq_detectada > 2900 && freq_detectada < 3100) {
                mover(0, 1, 1, 0); // Izquierda [cite: 183]
            } else if (freq_detectada > 3900 && freq_detectada < 4100) {
                mover(1, 0, 0, 1); // Derecha [cite: 184]
            } else {
                mover(0, 0, 0, 0);
            }
        } else {
            mover(0, 0, 0, 0);
        }


        // Evitar que el Watchdog se queje y dar tiempo al sistema [cite: 237]
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
