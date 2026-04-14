#include <stdio.h>
#include <stdlib.h>
#include "esp_heap_caps.h"
#include "esp_attr.h"
#include "esp_cpu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define VECTOR_SIZE 20

// --- 1. MEMORIAS ESTÁTICAS ---
DRAM_ATTR static int v_dram_st[VECTOR_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
RTC_DATA_ATTR static int v_rtc_st[VECTOR_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
const __attribute__((section(".rodata"))) int v_flash_st[VECTOR_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

// Función de procesamiento (multiplicación)
void IRAM_ATTR multiply_vector_scalar(int *vec, int scalar) {
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vec[i] = vec[i] * scalar;
    }
}

// Función de medición de desempeño
void measure_mem(const char* name, int* ptr, bool readonly) {
    if (!ptr) {
        printf("%-20s | ERROR: No asignada (SPIRAM descativada)\n", name);
        fflush(stdout);
        return;
    }

    uint32_t start = esp_cpu_get_cycle_count();
    
    if (readonly) {
        // Para Flash, simulamos lectura sumando valores (volatile para evitar que el compilador lo borre)
        volatile int dummy = 0;
        for(int i=0; i<VECTOR_SIZE; i++) dummy += ptr[i];
    } else {
        multiply_vector_scalar(ptr, 5);
    }
    
    uint32_t end = esp_cpu_get_cycle_count();
    uint32_t diff = end - start;
    float cycles_per_byte = (float)diff / (VECTOR_SIZE * sizeof(int));

    printf("%-20s | %12lu | %12.2f\n", name, (unsigned long)diff, cycles_per_byte);
    fflush(stdout); // Fuerza la salida al monitor serial
}

void app_main(void) {
    // 1. ESPERA CRÍTICA: Da tiempo al monitor serial para conectar tras el arranque/tuning
    vTaskDelay(pdMS_TO_TICKS(5000));

    printf("\n--- RESULTADOS EJERCICIO 4: DESEMPEÑO DE MEMORIA ---\n\n");
    printf("%-20s | %-12s | %-12s\n", "Tipo Memoria", "Ciclos", "Ciclos/Byte");
    printf("------------------------------------------------------------\n");
    fflush(stdout);

    // --- 2. ASIGNACIÓN DE MEMORIAS DINÁMICAS ---
    int *d_dram = (int *)malloc(VECTOR_SIZE * sizeof(int));
    
    // IRAM Dinámica
    int *d_iram = (int *)heap_caps_malloc(VECTOR_SIZE * sizeof(int), MALLOC_CAP_EXEC);
    
    // PSRAM Dinámica (Solo funcionará si está activa en menuconfig)
    int *d_psram = (int *)heap_caps_malloc(VECTOR_SIZE * sizeof(int), MALLOC_CAP_SPIRAM);

    // Inicialización de datos
    for(int i=0; i<VECTOR_SIZE; i++) {
        if(d_dram) d_dram[i] = i + 1;
        if(d_iram) d_iram[i] = i + 1;
        if(d_psram) d_psram[i] = i + 1;
    }

    // --- 3. EJECUCIÓN DE MEDICIONES ---
    measure_mem("DRAM Estatica", v_dram_st, false);
    measure_mem("RTC Estatica", v_rtc_st, false);
    measure_mem("Flash (ROData)", (int*)v_flash_st, true);
    measure_mem("DRAM Dinamica", d_dram, false);
    measure_mem("IRAM Dinamica", d_iram, false);
    measure_mem("PSRAM Dinamica", d_psram, false);

    // --- 4. LIMPIEZA ---
    if(d_dram) free(d_dram);
    if(d_iram) free(d_iram);
    if(d_psram) free(d_psram);

    printf("------------------------------------------------------------\n");
    printf("Fin del ejercicio. Analice los tiempos de acceso.\n");
    fflush(stdout);

    // Bucle infinito para evitar que la tarea principal muera y se reinicie
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}