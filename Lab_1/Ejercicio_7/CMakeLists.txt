cmake_minimum_required(VERSION 3.16)

# Esto le dice a ESP-IDF que busque componentes en la carpeta local 'components'
set(EXTRA_COMPONENT_DIRS "components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Ejercicio_7)