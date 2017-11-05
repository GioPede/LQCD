# Input and Output classes for the core library
add_subdirectory(JsonInput)
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/inputconf.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/inputparser.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/outputconf.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/outputobs.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/outputterm.cpp")
