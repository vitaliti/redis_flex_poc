cmake_minimum_required(VERSION 3.20)

project(MySpeedbApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SPEEDB_DIR "C:/Users/Administrator/Desktop/Projects/speedb")

include_directories(include)

add_executable(MyApp 
    main.cpp
    src/db_controller.cpp
)

target_include_directories(MyApp PRIVATE
    ${SPEEDB_DIR}/include
)

target_link_libraries(MyApp PRIVATE
    ${SPEEDB_DIR}/build/Release/speedb.lib
)

target_link_libraries(MyApp PRIVATE
    Rpcrt4
    Shlwapi
)