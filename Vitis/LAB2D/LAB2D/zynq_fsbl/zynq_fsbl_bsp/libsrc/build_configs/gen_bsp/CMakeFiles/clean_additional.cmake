# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\diskio.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\ff.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\ffconf.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\sleep.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\xilffs.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\xilffs_config.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\xilrsa.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\xiltimer.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\include\\xtimer_config.h"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\lib\\libxilffs.a"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\lib\\libxilrsa.a"
  "D:\\Vivado_Project\\Vitis\\LAB2D\\LAB2D\\zynq_fsbl\\zynq_fsbl_bsp\\lib\\libxiltimer.a"
  )
endif()
