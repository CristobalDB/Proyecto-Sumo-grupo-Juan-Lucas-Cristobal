Para poder correr el codigo
powershell:
usbipd list
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>

Para Flashear
source ~/.profile && source ~/.bashrc && get_esp32 && idf.py build && idf.py flash -p /dev/ttyUSB0 monitor


