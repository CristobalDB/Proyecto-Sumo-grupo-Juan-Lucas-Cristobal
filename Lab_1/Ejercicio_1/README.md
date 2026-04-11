# Ejercicio 1 - Hello World Personalizado

Para ejecutar este proyecto, asegúrese de tener el toolchain de ESP-IDF instalado y el dispositivo conectado a WSL mediante `usbipd`.

## Comando de ejecución:
source ~/.profile && source ~/.bashrc && get_esp32 && idf.py set-target esp32s3 && idf.py build && idf.py -p /dev/ttyUSB0 flash monitor