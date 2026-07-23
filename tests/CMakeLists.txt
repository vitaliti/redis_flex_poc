# cmake_minimum_required(VERSION 3.20)

# project(MySpeedbApp)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(SPEEDB_DIR "C:/Users/Administrator/Desktop/Projects/speedb")

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(UnitTests
    test_main.cpp
    lfu_eviction_policy_ut.cpp
    # test_memory_controller.cpp

    # ../src/ram_cache.cpp
    # ../src/memory_controller.cpp
    # ../src/lru_eviction_policy.cpp
)

target_include_directories(UnitTests PRIVATE
    ../include
)

target_link_libraries(UnitTests
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    redis_core
)

include(GoogleTest)

gtest_discover_tests(UnitTests)