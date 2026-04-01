#include <stdio.h>
#include <inttypes.h>
#include "esp_timer.h"
#include "esp_cpu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
    // Definición de variables según el lab
    volatile int var_1 = 233;
    volatile int var_2 = 128;
    volatile int result_0, result_1, result_2, result_3, result_4;
    
    int X = 1000; // Puedes cambiar este valor para el análisis de CPI

    // Inicio de mediciones
    uint64_t start_time = esp_timer_get_time();
    uint32_t start_cycles = esp_cpu_get_cycle_count();

    // Bucle de operaciones solicitado
    for (int i = 0; i < X; i++) {
        result_0 = var_1 + var_2;
        result_1 = var_1 + 10;
        result_2 = var_1 % var_2;
        result_3 = var_1 * var_2;
        result_4 = var_1 / var_2;
    }

    // Fin de mediciones
    uint32_t end_cycles = esp_cpu_get_cycle_count();
    uint64_t end_time = esp_timer_get_time();

    // Cálculos de desempeño
    uint32_t total_cycles = end_cycles - start_cycles;
    uint64_t total_time_us = end_time - start_time;
    
    // El CPI se calcula dividiendo ciclos totales por instrucciones (aprox X * 5)
    float cpi = (float)total_cycles / (X * 5);

    // Impresión de resultados
    printf("\n--- RESULTADOS EJERCICIO 3 ---\n");
    printf("Iteraciones (X): %d\n", X);
    printf("Tiempo total: %llu us\n", total_time_us);
    printf("Ciclos totales: %" PRIu32 "\n", total_cycles);
    printf("CPI estimado: %.2f\n", cpi);
    
    // Verificación de resultados para que el compilador no borre las variables
    if (result_4 > 0 || result_0 > 0) {
        printf("Check Results: PASS\n");
    }

    printf("------------------------------\n");
}