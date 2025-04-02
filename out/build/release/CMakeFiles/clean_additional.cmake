# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\label2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\label2_autogen.dir\\ParseCache.txt"
  "label2_autogen"
  )
endif()
