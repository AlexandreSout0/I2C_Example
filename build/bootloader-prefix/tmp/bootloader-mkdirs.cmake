# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/alexandre/esp/esp-idf/components/bootloader/subproject"
  "/Users/alexandre/Documents/GitHub/I2C_Example/build/bootloader"
  "/Users/alexandre/Documents/GitHub/I2C_Example/build/bootloader-prefix"
  "/Users/alexandre/Documents/GitHub/I2C_Example/build/bootloader-prefix/tmp"
  "/Users/alexandre/Documents/GitHub/I2C_Example/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/alexandre/Documents/GitHub/I2C_Example/build/bootloader-prefix/src"
  "/Users/alexandre/Documents/GitHub/I2C_Example/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/alexandre/Documents/GitHub/I2C_Example/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/alexandre/Documents/GitHub/I2C_Example/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
