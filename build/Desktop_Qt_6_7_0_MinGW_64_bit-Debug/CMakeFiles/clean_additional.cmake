# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GraphEditorPlus_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GraphEditorPlus_autogen.dir\\ParseCache.txt"
  "GraphEditorPlus_autogen"
  )
endif()
