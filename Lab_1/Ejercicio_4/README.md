powershell:
usbipd list
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>

Para Flashear por primera vez
source ~/.profile && source ~/.bashrc && get_esp32 && idf.py build && idf.py flash -p /dev/ttyUSB0 monitor

Para hacer fullclean
source ~/.profile && source ~/.bashrc && get_esp32 && idf.py fullclean && idf.py build && idf.py flash -p /dev/ttyUSB0 monitor

CONFIGURACION:
idf.py menuconfig -> Serial flasher config ->Flash SPI mode (QIO)

Component config -> ESP PSRAM -> activated
        Mode (QUAD/OCT) of SPI RAM chip in use (Octal Mode PSRAM)  --->
        Type of SPIRAM chip in use (Auto-detect)  --->
        [ ] Enable Executable in place from (XiP) from PSRAM feature (default value)
        [ ] Move Instructions in Flash to PSRAM (default value)
        [ ] Move Read-Only Data in Flash to PSRAM (default value)
            Set RAM clock speed (40Mhz clock speed)  --->
        [ ] Enable SPI RAM ECC (default value)
        [*] Initialise the PSRAM related hardware (default value)
        [*]     Initialize PSRAM during startup, including the hardware and memory related configurations (default value)
        [*] Pre-configure memory protection for PSRAM (default value)
        [ ] Ignore PSRAM when not found (default value)
            SPI RAM access method (Make RAM allocatable using heap_caps_malloc(..., MALLOC_CAP_SPIRAM))  --->
        [*] Run memory test on SPI RAM initialization
        [ ] Try to allocate memories of WiFi and LWIP in SPIRAM firstly. If failed, allocate internal memory (default value)
        [ ] Allow .bss segment placed in external memory (default value)
        [ ] Allow .noinit segment placed in external memory (default value)


(Top) → Component config → ESP System Settings
    CPU frequency (240 MHz)  --->
    Cache config  --->
    Memory  --->
    Trace memory  --->
    Panic handler behaviour (Print registers and reboot)  --->
(15) Panic reboot delay (Seconds)
[*] Enable RTC fast memory for dynamic allocations (default value)
[ ] Enable memory protection
(32) System event queue size (default value)
(2304) Event loop task stack size (default value)
(3584) Main task stack size (default value)
    Main task core affinity (CPU0)  --->
(2048) Minimal allowed size for shared stack (default value)
[ ] Interrupt watchdog
[ ] Enable Task Watchdog Timer
[ ] Place panic handler code in IRAM (default value)
[ ] OpenOCD debug stubs (default value)
[*] Make exception and panic handlers JTAG/OCD aware (default value)
    Interrupt level to use for Interrupt Watchdog and other system checks (Level 4 interrupt)  --->
[*] Re-calibration BBPLL at startup (default value)