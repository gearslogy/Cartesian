cmake_minimum_required(VERSION 3.16)
project("CartesianLuajitStdPlugin")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/CartesianPlugins/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/CartesianPlugins/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/CartesianPlugins/bin)


# CGAL
INCLUDE_DIRECTORIES(D:/plugin_dev/libs/CGAL-5.1-beta1/include)
INCLUDE_DIRECTORIES(D:./plugin_dev/libs/CGAL-5.1-beta1/auxiliary/gmp/include)


# GLM
include_directories(D:/plugin_dev/libs/GLM_include)


## ---------------------------- Boost include --------------------------------------
# https://cmake.org/cmake/help/v3.15/module/FindBoost.html
set(BOOST_ROOT D:/plugin_dev/libs/boost_1_74_0_b1_rc1)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs

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
link_directories(${CMAKE_SOURCE_DIR}/CartesianLuajitCore/bin)
# ----------------------------- MyCore --------------------------------------------
FILE(GLOB SRC
        CGAL_Bind/*.cpp
        GLM_Bind/*.cpp)

add_library(stdlib SHARED ${SRC})
target_link_libraries(stdlib CartesianLuaJIT)

