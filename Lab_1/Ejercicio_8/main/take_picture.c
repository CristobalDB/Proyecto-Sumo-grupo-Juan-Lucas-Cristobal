#include <stdio.h>
#include <stdlib.h>
#include "esp_camera.h"
#include "equalize.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// cd ~/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_8 && ./flash_and_dump.sh 
// Terminado el proceso, salir del portal con: Ctrl T + X . para ver el resultado 0x completo en la terminal.
#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27
#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

static camera_config_t camera_config = {
    .pin_pwdn  = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk  = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,
    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href  = CAM_PIN_HREF,
    .pin_pclk  = CAM_PIN_PCLK,
    .xclk_freq_hz = 20000000,
    .ledc_timer   = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_GRAYSCALE,
    .frame_size   = FRAMESIZE_96X96,
    .fb_location  = CAMERA_FB_IN_DRAM,
    .jpeg_quality = 12,
    .fb_count     = 1,
    .grab_mode    = CAMERA_GRAB_WHEN_EMPTY,
};

void app_main(void) {
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        printf("Error iniciando camara: 0x%x\n", err);
        return;
    }

    vTaskDelay(pdMS_TO_TICKS(500));

    camera_fb_t *pic = esp_camera_fb_get();
    if (!pic) {
        printf("Fallo captura\n");
        return;
    }

    // Esperar a que el monitor serial este conectado antes de transmitir
    vTaskDelay(pdMS_TO_TICKS(5000));

    printf("BEGIN_IMAGE_HEX len=%u\n", (unsigned)pic->len);
    fflush(stdout);

    // 16 bytes por linea con delay entre lineas para no saturar el UART
    char line[16 * 6 + 2];
    for (size_t i = 0; i < pic->len; i += 16) {
        size_t chunk = (pic->len - i > 16) ? 16 : (pic->len - i);
        int pos = 0;
        for (size_t j = 0; j < chunk; j++) {
            pos += snprintf(&line[pos], sizeof(line) - (size_t)pos, "0x%02X,", pic->buf[i + j]);
        }
        line[pos++] = '\n';
        line[pos]   = '\0';
        printf("%s", line);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    printf("END_IMAGE_HEX\n");
    fflush(stdout);

    // Ecualizar y transmitir
    uint8_t *eq_buf = malloc(pic->len);
    if (eq_buf) {
        equalize_histogram(pic->buf, eq_buf, pic->len);

        printf("BEGIN_EQUALIZED_HEX len=%u\n", (unsigned)pic->len);
        fflush(stdout);

        for (size_t i = 0; i < pic->len; i += 16) {
            size_t chunk = (pic->len - i > 16) ? 16 : (pic->len - i);
            int pos = 0;
            for (size_t j = 0; j < chunk; j++) {
                pos += snprintf(&line[pos], sizeof(line) - (size_t)pos, "0x%02X,", eq_buf[i + j]);
            }
            line[pos++] = '\n';
            line[pos]   = '\0';
            printf("%s", line);
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        printf("END_EQUALIZED_HEX\n");
        fflush(stdout);
        free(eq_buf);
    }

    esp_camera_fb_return(pic);
}
