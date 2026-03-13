cmake_minimum_required(VERSION 3.26.0)

project(Homework
        VERSION 0.0.0
        LANGUAGES CXX C
        DESCRIPTION "Performance aware programming homework")

#C++ is only "required" to remove warnings for libraries
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TLSF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vender/tlsf")
set(STACK_WALKER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vender/StackWalker/Main/StackWalker")
set(RAPID_HASH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vender/rapidhash")
set(VENDER_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/vender")
#Set the location of the engine source.
set(SRC_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(HOMEWORK_SOURCE src/main.cpp)

add_executable(Homework ${HOMEWORK_SOURCE})

if (WIN32)
    target_compile_definitions(Homework PRIVATE
                               _CRT_SECURE_NO_WARNINGS
                               WIN32_LEAN_AND_MEAN
                               NOMINMAX)
endif()

target_include_directories(Homework SYSTEM PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${SRC_INCLUDE}
                                               ${RAPID_HASH_DIR}
                                               ${TLSF_INCLUDE_DIR}
                                               ${VENDER_INCLUDE}
                                               ${CGLTF_INCLUDE})

set(FOUNDATION_SOURCE src/Foundation/Array.hpp
                      src/Foundation/Assert.hpp
                      src/Foundation/Bit.cpp
                      src/Foundation/Bit.hpp
                      src/Foundation/BlobSerialisation.cpp
                      src/Foundation/BlobSerialisation.hpp
                      src/Foundation/Blob.hpp
                      src/Foundation/Colour.cpp
                      src/Foundation/Colour.hpp
                      src/Foundation/File.cpp
                      src/Foundation/File.hpp
                      src/Foundation/HashMap.hpp
                      src/Foundation/HashMap.cpp
                      src/Foundation/Log.hpp
                      src/Foundation/Memory.cpp
                      src/Foundation/Memory.hpp
                      src/Foundation/Numerics.cpp
                      src/Foundation/Numerics.hpp
                      src/Foundation/Platform.hpp
                      src/Foundation/Process.cpp
                      src/Foundation/Process.hpp
                      src/Foundation/RelativeDataStructures.hpp
                      src/Foundation/ResourcePool.cpp
                      src/Foundation/ResourcePool.hpp
                      src/Foundation/ResourceManager.cpp
                      src/Foundation/ResourceManager.hpp
                      src/Foundation/String.cpp
                      src/Foundation/String.hpp
                      src/Foundation/Time.cpp
                      src/Foundation/Time.hpp
                      src/Foundation/WindowsDeclarations.hpp
)
add_library(Foundation STATIC ${FOUNDATION_SOURCE})

target_compile_definitions(Foundation PRIVATE
                                      _CRT_SECURE_NO_WARNINGS)

if(WIN32)
    target_compile_definitions(Foundation PRIVATE
                                          WIN32_LEAN_AND_MEAN
                                          NOMINMAX)
endif()


target_include_directories(Foundation SYSTEM PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${TLSF_INCLUDE_DIR}
                           ${STACK_WALKER_DIR}
                           ${RAPID_HASH_DIR}
                           ${SRC_INCLUDE}
                           ${VENDOR_INCLUDE})

if (UNIX)
    target_link_libraries(Homework PRIVATE
                          dl
                          pthread)
endif()

add_library(External STATIC
    vender/tlsf/tlsf.c
    vender/tlsf/tlsf.h)

set_target_properties(External PROPERTIES SYSTEM ON)

if (WIN32)
    target_sources(External PRIVATE
                   vender/StackWalker/Main/StackWalker/StackWalker.cpp
                   vender/StackWalker/Main/StackWalker/StackWalker.h)
endif()

if (UNIX)
    target_link_libraries(External PRIVATE
                          dl)
endif()

target_link_libraries(Homework PRIVATE Foundation External)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HOMEWORK_SOURCE} ${FOUNDATION_SOURCE})

target_compile_options(Homework PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4>
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Homework)
endif()