# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Users/Administrator/esp/v5.1.3/esp-idf/components/bootloader/subproject"
  "F:/c3_lcd_win/knob_panel/build/bootloader"
  "F:/c3_lcd_win/knob_panel/build/bootloader-prefix"
  "F:/c3_lcd_win/knob_panel/build/bootloader-prefix/tmp"
  "F:/c3_lcd_win/knob_panel/build/bootloader-prefix/src/bootloader-stamp"
  "F:/c3_lcd_win/knob_panel/build/bootloader-prefix/src"
  "F:/c3_lcd_win/knob_panel/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/c3_lcd_win/knob_panel/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/c3_lcd_win/knob_panel/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
