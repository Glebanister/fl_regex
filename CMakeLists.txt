cmake_minimum_required(VERSION 3.0.0)
project(regex_c VERSION 0.1.0)

set(EXE test_regex)
set(STRESS stress_regex)

add_executable(${EXE} src/regex.c
                      src/test.c)

add_executable(${STRESS} src/stress.c
                         src/regex.c)

target_include_directories(${EXE} PUBLIC include)
target_include_directories(${EXE} PUBLIC ctest)

target_include_directories(${STRESS} PUBLIC include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

enable_testing()
add_test(NAME test_regex
         COMMAND ./test_regex)
