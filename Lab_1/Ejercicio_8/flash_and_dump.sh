#!/bin/bash
source /home/jukal/esp/idf/esp-idf/export.sh > /dev/null

echo "Salir con Ctrl+T luego X cuando veas: Returned from app_main()"
echo ""

> /tmp/serial_output.txt
script -q -c "idf.py -p /dev/ttyUSB0 flash monitor" /tmp/serial_output.txt

echo ""
echo "====== IMAGEN ORIGINAL ======"
sed -n '/BEGIN_IMAGE_HEX/,/END_IMAGE_HEX/p' /tmp/serial_output.txt \
    | grep -v "BEGIN_IMAGE_HEX\|END_IMAGE_HEX" \
    | tr -d '\r\n '
echo ""

echo ""
echo "====== IMAGEN ECUALIZADA ======"
sed -n '/BEGIN_EQUALIZED_HEX/,/END_EQUALIZED_HEX/p' /tmp/serial_output.txt \
    | grep -v "BEGIN_EQUALIZED_HEX\|END_EQUALIZED_HEX" \
    | tr -d '\r\n '
echo ""
