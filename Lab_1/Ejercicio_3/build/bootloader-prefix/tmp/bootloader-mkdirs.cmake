# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/diazb/esp/esp-idf/components/bootloader/subproject"
  "/home/diazb/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_3/build/bootloader"
  "/home/diazb/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_3/build/bootloader-prefix"
  "/home/diazb/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_3/build/bootloader-prefix/tmp"
  "/home/diazb/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_3/build/bootloader-prefix/src/bootloader-stamp"
  "/home/diazb/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_3/build/bootloader-prefix/src"
  "/home/diazb/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_3/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/diazb/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_3/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/diazb/Proyecto-Sumo-grupo-Juan-Lucas-Cristobal/Lab_1/Ejercicio_3/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
