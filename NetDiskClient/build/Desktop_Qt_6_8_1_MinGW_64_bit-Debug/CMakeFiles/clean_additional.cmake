# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\NetDiskClient_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\NetDiskClient_autogen.dir\\ParseCache.txt"
  "NetDiskClient_autogen"
  )
endif()
