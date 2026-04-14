#include <stdio.h>
#include <inttypes.h>
#include "esp_timer.h"
#include "esp_cpu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
    volatile int var_1 = 233;
    volatile int var_2 = 128;
    volatile int result_0, result_1, result_2, result_3, result_4;
    
    int X = 1000; 

    uint64_t start_time = esp_timer_get_time(); 
    uint32_t start_cycles = esp_cpu_get_cycle_count();

    for (int i = 0; i < X; i++) {
        result_0 = var_1 + var_2;
        //result_1 = var_1 + 10;
        result_2 = var_1 % var_2;
        result_3 = var_1 * var_2;
        result_4 = var_1 / var_2;
    }

    uint32_t end_cycles = esp_cpu_get_cycle_count();
    uint64_t end_time = esp_timer_get_time();

    uint32_t total_cycles = end_cycles - start_cycles;
    uint64_t total_time_us = end_time - start_time;
    
    float cpi = (float)total_cycles / (X * 5); 

    uint32_t cpu_freq_hz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000;
    float time_by_cycles = (float)total_cycles / cpu_freq_hz;

    printf("\n--- RESULTADOS EJERCICIO 3 ---\n");
    printf("Iteraciones (X): %d\n", X);
    printf("Frecuencia CPU: %lu MHz\n", (unsigned long)CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ);
    printf("Tiempo medido (esp_timer): %llu us\n", total_time_us);
    printf("Tiempo calculado (ciclos): %.6f s\n", time_by_cycles);
    printf("Ciclos totales: %" PRIu32 "\n", total_cycles);
    printf("CPI estimado: %.4f\n", cpi);
    
    if (result_4 > 0 || result_0 > 0) {
        printf("Check Results: PASS\n");
    }

    printf("------------------------------\n");
}