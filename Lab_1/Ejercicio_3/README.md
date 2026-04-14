Desde una PowerShell con administrador:
PowerShell
usbipd list  # Busca el BUSID de tu dispositivo
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>

mete esto en wsl
ls /dev/ttyUSB*


source ~/.profile && source ~/.bashrc && get_esp32 && idf.py set-target esp32s3 && idf.py build && idf.py -p /dev/ttyUSB0 flash monitor

Para correr el codigo despues de cambiar la frecuencia
idf.py fullclean && idf.py build && idf.py -p /dev/ttyUSB0 flash monitor