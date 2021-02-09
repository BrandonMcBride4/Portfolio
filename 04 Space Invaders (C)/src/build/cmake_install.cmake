# Install script for directory: /home/xilinx/lab6

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/xilinx/lab6/build/intc/cmake_install.cmake")
  include("/home/xilinx/lab6/build/btns/cmake_install.cmake")
  include("/home/xilinx/lab6/build/fit/cmake_install.cmake")
  include("/home/xilinx/lab6/build/switches/cmake_install.cmake")
  include("/home/xilinx/lab6/build/image_render/cmake_install.cmake")
  include("/home/xilinx/lab6/build/video/cmake_install.cmake")
  include("/home/xilinx/lab6/build/tank/cmake_install.cmake")
  include("/home/xilinx/lab6/build/saucer/cmake_install.cmake")
  include("/home/xilinx/lab6/build/globals/cmake_install.cmake")
  include("/home/xilinx/lab6/build/aliens/cmake_install.cmake")
  include("/home/xilinx/lab6/build/control/cmake_install.cmake")
  include("/home/xilinx/lab6/build/tank_bullet/cmake_install.cmake")
  include("/home/xilinx/lab6/build/alien_bullet/cmake_install.cmake")
  include("/home/xilinx/lab6/build/bunkers/cmake_install.cmake")
  include("/home/xilinx/lab6/build/user_audio/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/xilinx/lab6/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
