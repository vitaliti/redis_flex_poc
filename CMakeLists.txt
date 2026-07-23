cmake_minimum_required(VERSION 3.20)

project(MySpeedbApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SPEEDB_DIR "C:/Users/Administrator/Desktop/Projects/speedb")

add_library(redis_core
    src/memory_controller.cpp
    src/speedb_flash_storage.cpp
    src/ram_cache.cpp
    src/lru_eviction_policy.cpp
    src/lfu_eviction_policy.cpp
)

target_include_directories(redis_core PUBLIC
    include
    ${SPEEDB_DIR}/include
)

target_link_libraries(redis_core PUBLIC
    ${SPEEDB_DIR}/build/Release/speedb.lib
    Rpcrt4
    Shlwapi
)

add_executable(MyApp
    main.cpp
)

target_link_libraries(MyApp PRIVATE
    redis_core
)

enable_testing()

add_subdirectory(tests)