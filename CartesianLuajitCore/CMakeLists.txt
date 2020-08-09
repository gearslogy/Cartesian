cmake_minimum_required(VERSION 3.16)
project("CartesianLuaJIT")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/CartesianLuajitCore/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/CartesianLuajitCore/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/CartesianLuajitCore/bin)

add_definitions(-DCARTESIAN_BUILDING_DLL)
## ---------------------------- Boost include --------------------------------------
# https://cmake.org/cmake/help/v3.15/module/FindBoost.html
set(BOOST_ROOT D:/plugin_dev/libs/boost_1_74_0_b1_rc1)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_RELEASE_LIBS       ON)

find_package(Boost 1.74 COMPONENTS filesystem REQUIRED)  # 这里只link这2个库
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
    MESSAGE( STATUS "Boost_LIBRARY_DIRS = ${Boost_LIBRARY_DIRS}.")
    link_directories(${Boost_LIBRARY_DIRS})
endif()

# ------------------ spdlog ------------------------
include_directories(D:/plugin_dev/libs/spdlog-1.7.0/include)




include_directories(D:/plugin_dev/libs/sol2)
set(USE_LUAJIT 1)
if(USE_LUAJIT)
    include_directories(D:/plugin_dev/libs/luaJit2.1_Win65_vc16_lib/include)
    link_directories(D:/plugin_dev/libs/luaJit2.1_Win65_vc16_lib)
    message("[USE JIT MODE FOR COMPILE]")
else()
    message("[USE LUA MODE FOR COMPILE]")
    include_directories(D:/plugin_dev/libs/lua-5.3.5_Win64_vc16_lib_static/include)
    link_directories(D:/plugin_dev/libs/lua-5.3.5_Win64_vc16_lib_static)
endif()


# ----------------------------- MyCore --------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/CartesianLuajitCore/include)
file(GLOB CARTESIAN_CORE ${CMAKE_SOURCE_DIR}/CartesianLuajitCore/src/*.cpp)
# ----------------------------- MyCore --------------------------------------------

add_library(CartesianLuaJIT SHARED ${CARTESIAN_CORE})

## ---------------------------------- Lua libs settings ------------------------------------
if(USE_LUAJIT)
    target_link_libraries(CartesianLuaJIT luajit lua51 minilua buildvm)
else()
    target_link_libraries(CartesianLuaJIT lua53)
endif()
## ---------------------------------- Lua libs settings ------------------------------------

target_link_libraries(CartesianLuaJIT ${Boost_LIBRARIES})

