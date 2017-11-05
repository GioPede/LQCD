# User extensions source directory

# Include sources for json validation
target_sources(LatticeQCDUserExtensions PRIVATE 
    "${CMAKE_CURRENT_LIST_DIR}/src/Actions/JsonInput/jsonuseractions.cpp" 
    "${CMAKE_CURRENT_LIST_DIR}/src/Observables/JsonInput/jsonuserobservables.cpp" 
)

# Include user-defined actions
#target_sources(jsonParserUser PRIVATE 
#    "${CMAKE_CURRENT_LIST_DIR}/src/Actions/....)


# Include user-defined actions
#target_sources(jsonParserUser PRIVATE 
#    "${CMAKE_CURRENT_LIST_DIR}/src/Observables/....)


