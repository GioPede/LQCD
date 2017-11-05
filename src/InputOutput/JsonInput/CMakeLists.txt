# Main source directory for the core library
add_subdirectory(JsonAction)
add_subdirectory(JsonGeneric)
add_subdirectory(JsonObservable)
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/jsoninput.cpp")
