cmake_minimum_required(VERSION 3.15)
project(IRGenerator)

set(CMAKE_CXX_STANDARD 17)

# Поиск SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Простые настройки LLVM
set(LLVM_INCLUDE_DIRS "/usr/include/llvm-14" "/usr/include/llvm-c-14")
set(LLVM_LIBRARIES "-lLLVM-14")

include_directories(${LLVM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

add_executable(app_ir_gen 
    app_ir_gen.cpp
    sim.c
)

target_link_libraries(app_ir_gen 
    ${LLVM_LIBRARIES}
    ${SDL2_LIBRARIES}
)