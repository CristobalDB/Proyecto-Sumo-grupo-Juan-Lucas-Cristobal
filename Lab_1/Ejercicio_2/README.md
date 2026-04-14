# Ejercicio 2 - Procesamiento de Imagen (Sobel)

Este ejercicio realiza el procesamiento de una imagen de 96x96 píxeles aplicando operadores Sobel directamente en el MCU.

Para correr el código es necesario hacer los siguientes pasos:

PowerShell
usbipd list  # Busca el BUSID de tu dispositivo
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>

Terminal wsl
source ~/.profile && source ~/.bashrc && get_esp32 && idf.py 
set-target esp32s3 && idf.py build && idf.py -p /dev/ttyUSB0 
flash monitor
