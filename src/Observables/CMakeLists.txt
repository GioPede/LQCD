# Observables for the core library
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/observable.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/superobs.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/energydensity.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/plaquette.cpp")
target_sources(LatticeQCD PRIVATE "${CMAKE_CURRENT_LIST_DIR}/topologicalcharge.cpp")
