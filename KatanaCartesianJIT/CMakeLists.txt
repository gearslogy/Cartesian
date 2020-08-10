cmake_minimum_required (VERSION 3.2)
project(KatanaProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(KATANA_ROOT "D:/Program Files/Katana3.6v1")
list(INSERT CMAKE_MODULE_PATH 0 "${KATANA_ROOT}/plugins/Src/cmake")

find_package(Katana PATHS "${KATANA_ROOT}/plugin_apis/cmake" REQUIRED)
# Ensure we don't compile DSOs with undefined symbols.
if (CMAKE_SYSTEM_NAME MATCHES Linux)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-undefined")
endif ()

if (NOT MSVC)
    add_compile_options(-Wall)
endif ()
if (MSVC)
    # Prevent Boost auto-linking.
    add_definitions(-DBOOST_ALL_NO_LIB)
endif ()

#add_compile_definitions(__cplusplus=201103L)

file(GLOB SRC
        src/*.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/KatanaCartesianJIT/bin/Libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/KatanaCartesianJIT/bin/Libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/KatanaCartesianJIT/bin/Libs)


# -------------------------- Lua jit -----------------------------------------
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
# -------------------------- Lua jit -----------------------------------------


# -------------------------- BOOST AND CGAL ---------------------------------------
INCLUDE_DIRECTORIES(D:/plugin_dev/libs/CGAL-5.1-beta1/include)
INCLUDE_DIRECTORIES(D:./plugin_dev/libs/CGAL-5.1-beta1/auxiliary/gmp/include)

set(BOOST_ROOT D:/plugin_dev/libs/boost_1_74_0_b1_rc1)

find_package(Boost 1.74 COMPONENTS regex system REQUIRED)  # Boost_LIBRARIES 就是这2个库

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
endif()
# -------------------------- BOOST AND CGAL ---------------------------------------

# ------------------ spdlog ------------------------
include_directories(D:/plugin_dev/libs/spdlog-1.7.0/include)

# ----------------------------- MyCore --------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/CartesianLuajitCore/include)
link_directories(${CMAKE_SOURCE_DIR}/CartesianLuajitCore/bin)
# ----------------------------- MyCore --------------------------------------------


add_library(CartesianScript MODULE ${SRC})


target_link_libraries(CartesianScript
        PRIVATE
        Katana::FnAttribute
        Katana::FnGeolibOpPlugin
        Katana::FnGeolibServices
        Katana::pystring
        CartesianLuaJIT
        )
