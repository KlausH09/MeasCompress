set(TARGET "${PROJECT_NAME}_test")
add_executable(${TARGET} 
    test_math.cpp
)
target_link_libraries(${TARGET} 
    PRIVATE Catch2::Catch2WithMain
    PRIVATE "${PROJECT_NAME}_src"
)
addCompileOpt(${TARGET})

set_target_properties(${TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)
