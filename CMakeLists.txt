cmake_minimum_required(VERSION 3.24)
set(PROJECT_LANGUAGES "CXX")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_FORTRAN_STANDARD 90)
#set(CMAKE_FORTRAN_STANDARD_REQUIRED ON)
# fixed form, C preprocessed, all REAL as double
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -ffixed-form -cpp -fdefault-real-8")
set(CMAKE_BUILD_TYPE ${BUILD_TYPE} CACHE STRING "Build type")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG" CACHE STRING "Release build flags" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-Ofast -DNDEBUG" CACHE STRING "Release build flags" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" CACHE STRING "Debug build flags" FORCE)
set(CMAKE_C_FLAGS_DEBUG "-O0 -g" CACHE STRING "Debug build flags" FORCE)
set(PROJECT_FAMILY_NAME "Kiwi")
set(PROJECT_VERSION "0.0.2")
set(PROJECT_VERSION_NAME "RitzCracker")
set(PROJECT_VERSION_MONTH "August")
set(PROJECT_VERSION_YEAR "2026")
set(PROJECT_WORKING_RDIRS "_T" "_I")
set(ACCEL "") # "Cuda", "Metal", "" (none)
set(ACCEL_DEPENDENCIES "") # cuBLAS, cuRAND, ...
set(ACCEL_INSTALL_DIR "/usr/local") # /usr/local, $ENV{CUDA_HOME}, $ENV{METAL_HOME}, ...
set(DEPENDENCY_LIB_TYPE "s") # shared (s) or static (a)
set(DGCALC_DEPENDENCY_TEST "")
set(DGCALC_DEPENDENCY_SRC ${DGCALC_DEPENDENCY_TEST} "Base" "Types" "Kyte" "Scalar" )
set(DGCALC_DEPENDENCY_LIB "")
set(GROUPS "THIRD")
set(THIRD_DEPENDENCY_TEST "Catch2")
set(THIRD_DEPENDENCY_SRC ${THIRD_DEPENDENCY_TEST})
set(THIRD_DEPENDENCY_LIB "")

# set to enable MPI. the value does not matter if running via another script.
 set(NUM_PROCESSES 2)

set(OUTPUT_STEM "zout")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

# set(LOCAL_ROOT "")
if (LOCAL_ROOT)
 include(${LOCAL_ROOT}/src/dgcalc.1.cmake)
else()
 execute_process(COMMAND dgcalc source 1 OUTPUT_VARIABLE SOURCE1_CODE)
 cmake_language(EVAL CODE "${SOURCE1_CODE}")
endif()
set(PROJECT_NAME "${PROJECT_FAMILY_NAME}${PROJECT_VERSION}${PROJECT_VERSION_NAME}_${PROJECT_VERSION_MONTH}${PROJECT_VERSION_YEAR}")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES ${PROJECT_LANGUAGES})
if (LOCAL_ROOT)
 include(${LOCAL_ROOT}/src/dgcalc.2.cmake)
else()
 execute_process(COMMAND dgcalc source 2 OUTPUT_VARIABLE SOURCE2_CODE)
 cmake_language(EVAL CODE "${SOURCE2_CODE}")
endif()




