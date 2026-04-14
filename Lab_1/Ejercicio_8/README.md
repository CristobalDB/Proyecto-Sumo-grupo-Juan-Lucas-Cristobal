
take_picture.c
Programa principal que corre en el ESP32. Contiene:
● camera_config — estructura con la configuración del hardware: pines, resolución
96×96, escala de grises, buffer en DRAM
● app_main() — función de entrada, ejecuta todo en orden:
● Inicializa la cámara
● Espera 500ms para estabilizar el sensor
● Captura un frame con esp_camera_fb_get()
● Espera 5000ms para que el monitor serial esté conectado
● Imprime la imagen original en hex (16 bytes por línea, 10ms entre líneas para no
saturar el UART), entre tags BEGIN_IMAGE_HEX / END_IMAGE_HEX
● Llama a equalize_histogram(), imprime el resultado entre tags
BEGIN_EQUALIZED_HEX / END_EQUALIZED_HEX
● Libera el buffer de la cámara
equalize.c
Módulo de procesamiento de imagen. Contiene:
● equalize_histogram(input, output, len) — toma la imagen cruda y produce una
ecualizada:
● Calcula el histograma: cuenta cuántos píxeles tienen cada valor (0–255)
● Calcula la CDF: suma acumulada del histograma
● Encuentra cdf_min: primer valor no cero de la CDF
● Construye una tabla de transformación que redistribuye los valores para maximizar
el contraste
● Aplica la tabla a cada píxel del buffer de salida
equalize.h
Interfaz pública de equalize.c. Declara la firma de equalize_histogram() para que
take_picture.c pueda usarla sin conocer su implementación.
CMakeLists.txt
Archivo de build del proyecto. Le dice al sistema de compilación:
● Qué archivos compilar: take_picture.c y equalize.c
● Dónde buscar headers: directorio actual (.)
● Qué librería externa necesita: esp32-camera
flash_and_dump.sh
Script que orquesta todo desde la PC:
● Activa el entorno ESP-IDF
● Limpia /tmp/serial_output.txt para evitar datos viejos
● Flashea el ESP32 y abre el monitor con script, que muestra el output en pantalla y lo
guarda en el archivo simultáneamente
● Al salir con Ctrl+T X, extrae automáticamente con sed los bloques hex de cada
imagen y los imprime listos para copiar al Colab.
Para tomar la foto hay que seguir los siguientes pasos:
1. cd ~/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_8 &&
./flash_and_dump.sh : con este comando flasheamos el ESP32 y abrimos el monitor
serial para ver la salida.
2. Ctrl T+X : Con este comando cerramos el monitor y se muestran los datos x0
completos de las imágenes, en la salida de la terminal.
3. Por último copiamos la salida para la imagen original y la imagen ecualizada, donde
ambas están en la salida de la terminal, correctamente diferenciadas.