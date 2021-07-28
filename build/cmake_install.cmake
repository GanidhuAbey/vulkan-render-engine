<<<<<<< HEAD
# Install script for directory: /home/ganidhu/Desktop/vulkan-render-engine
=======
# Install script for directory: /home/ganidhu/Desktop/old_engine/vulkan-render-engine
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9

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
<<<<<<< HEAD
    set(CMAKE_INSTALL_CONFIG_NAME "")
=======
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
<<<<<<< HEAD
file(WRITE "/home/ganidhu/Desktop/vulkan-render-engine/build/${CMAKE_INSTALL_MANIFEST}"
=======
file(WRITE "/home/ganidhu/Desktop/old_engine/vulkan-render-engine/build/${CMAKE_INSTALL_MANIFEST}"
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
