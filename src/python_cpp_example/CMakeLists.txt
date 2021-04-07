# Compile c++ code to a static library (here header-only library)
set(TARGET "${PROJECT_NAME}_src")
add_library(${TARGET} INTERFACE)
target_include_directories(${TARGET} INTERFACE "cpp_src/")

# Generate python module
set(TARGET bindings)
pybind11_add_module(${TARGET} ${SOURCES} bindings.cpp)
target_link_libraries(${TARGET} PRIVATE "${PROJECT_NAME}_src")
addCompileOpt(${TARGET})
