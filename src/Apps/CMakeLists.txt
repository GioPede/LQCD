# Main classes for core library
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/app.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/gaugefieldfactory.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/gaugefieldreader.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/wilsonflow.cpp")
