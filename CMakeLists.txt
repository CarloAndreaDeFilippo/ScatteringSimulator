cmake_minimum_required(VERSION 3.18)
project(
    scatteringSimulator
    LANGUAGES CXX
    )

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif()

find_package(Boost REQUIRED CONFIG COMPONENTS filesystem system)

option(USE_OPENMP "Enable OpenMP support" OFF)
option(USE_CUDA "Enable CUDA support" OFF)

file(GLOB ALL_CPP CONFIGURE_DEPENDS "src/*.cpp")

set(CUDA_SOURCES "")

if(USE_CUDA)

    enable_language(CUDA OPTIONAL)

    if(NOT CMAKE_CUDA_COMPILER)
        message(FATAL_ERROR "CUDA compiler not found.")
    endif()

    set(CUDA_SOURCES
    src/cuda/Rho1D_cuda.cu
    src/cuda/Rho2D_cuda.cu
    src/cuda/rho1d_kernel.cu
    src/cuda/rho2d_kernel.cu
    src/cuda/projection_kernel.cu
    )

    
    set(CMAKE_CUDA_ARCHITECTURES native)    

endif()

add_executable(${PROJECT_NAME}
    ${ALL_CPP}
    ${CUDA_SOURCES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>:/W4>
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:-Wall -Wextra>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    _USE_MATH_DEFINES
    $<$<BOOL:${USE_CUDA}>:USE_CUDA>
    $<$<BOOL:${USE_OPENMP}>:USE_OPENMP>
)

if(USE_CUDA)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        CUDA_STANDARD 11
        CUDA_STANDARD_REQUIRED ON
        CUDA_SEPARABLE_COMPILATION ON
    )
    
endif()


if(USE_OPENMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::filesystem
    Boost::system
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Libraries)
