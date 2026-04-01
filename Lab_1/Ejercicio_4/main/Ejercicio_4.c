#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_heap_caps.h"
#include "esp_attr.h"
#include "esp_cpu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define VECTOR_SIZE 1024 

// Memorias compatibles con lectura/escritura directa
DRAM_ATTR static int v_dram_st[VECTOR_SIZE];
RTC_DATA_ATTR static int v_rtc_st[VECTOR_SIZE];

void IRAM_ATTR multiply_vector(int *vec, int size) {
    if (!vec) return;
    for (int i = 0; i < size; i++) vec[i] = vec[i] * 3;
}

void measure_mem(const char* name, int* ptr) {
    if (!ptr) {
        printf("%-15s | %-12s | %-15s\n", name, "ERROR", "NULL");
        return;
    }
    // Inicializar
    for(int i = 0; i < VECTOR_SIZE; i++) ptr[i] = i;

    uint32_t start = esp_cpu_get_cycle_count();
    multiply_vector(ptr, VECTOR_SIZE);
    uint32_t end = esp_cpu_get_cycle_count();
    
    uint32_t diff = end - start;
    float per_byte = (float)diff / (VECTOR_SIZE * 4);
    printf("%-20s | %12lu | %12.2f\n", name, (unsigned long)diff, per_byte);
}

void app_main(void) {
    vTaskDelay(pdMS_TO_TICKS(2000));

    printf("\n--- RESULTADOS FINALES EJERCICIO 4 ---\n\n");
    printf("%-20s | %-12s | %-12s\n", "Tipo Memoria", "Ciclos Totales", "Ciclos/Byte");
    printf("------------------------------------------------------------\n");

    // Reservar dinámicas
    int *d_dram = (int *)heap_caps_malloc(VECTOR_SIZE * sizeof(int), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    int *d_psram = (int *)heap_caps_malloc(VECTOR_SIZE * sizeof(int), MALLOC_CAP_SPIRAM);

    // Medir solo lo que no rompe la caché
    measure_mem("DRAM Estatica",  v_dram_st);
    measure_mem("RTC Estatica",   v_rtc_st);
    measure_mem("DRAM Dinamica",  d_dram);
    
    if (d_psram) {
        measure_mem("PSRAM Dinamica (OCT)", d_psram);
    } else {
        printf("PSRAM Dinamica      |    [ERROR]   |  No asignada\n");
    }

    printf("------------------------------------------------------------\n");
    printf("Proceso finalizado. Use estos datos para su output.pdf\n");

    if(d_dram) free(d_dram);
    if(d_psram) free(d_psram);

    while(1) vTaskDelay(1000); 
}