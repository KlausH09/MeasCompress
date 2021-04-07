cmake_minimum_required(VERSION 3.10.0)
project(python_cpp_example) # FIXME must be equal with the py module name!!

add_subdirectory(extern/pybind11)
add_subdirectory(extern/catch2)

function(addCompileOpt TARGET)
    target_compile_options(${TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>)
    if (CMAKE_BUILD_TYPE EQUAL "Release")
        target_compile_options(${TARGET} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/Ox>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Ofast>)
    endif()
endfunction()

add_subdirectory("src/${PROJECT_NAME}")
add_subdirectory(tests/cpp)
