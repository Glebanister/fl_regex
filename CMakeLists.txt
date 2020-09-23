cmake_minimum_required(VERSION 3.0.0)
project(regex_c VERSION 0.1.0)

set(EXE regex_c)

add_executable(${EXE} src/main.c
                      src/regex.c)

target_include_directories(${EXE} PUBLIC include)
target_include_directories(${EXE} PUBLIC ctest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
