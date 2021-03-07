cmake_minimum_required(VERSION 3.0)
project(autodiff)

set(CMAKE_CXX_STANDARD 20)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_compile_options(
        -O3
        -g0
)

file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

if (SRC_FILES)
    message("LibType: static")
    add_library(autodiff STATIC ${SRC_FILES})
    target_include_directories(autodiff PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
else ()
    message("LibType: interface")
    add_library(autodiff INTERFACE)
    target_include_directories(autodiff INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
endif ()


file(GLOB TEST_FILES ${CMAKE_CURRENT_LIST_DIR}/test/*.cpp)
foreach (TEST_FILE ${TEST_FILES})
    get_filename_component(NAME ${TEST_FILE} NAME_WE)
    add_executable(${NAME} ${TEST_FILE})
    target_link_libraries(${NAME} autodiff)
endforeach ()
