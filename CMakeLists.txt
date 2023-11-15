cmake_minimum_required(VERSION 3.8)
project(my_project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# Change path from /src if needed, or add more directories
file(GLOB_RECURSE sources
        "${CMAKE_SOURCE_DIR}/*.c"
        "${CMAKE_SOURCE_DIR}/*.cpp"
        )

add_executable(cmpc ${sources})

target_compile_features(cmpc PRIVATE cxx_std_17)
# target_include_directories(cmpc PUBLIC "E:\\Dev\\llvm-project-llvmorg-17.0.2\\llvm\\include")
