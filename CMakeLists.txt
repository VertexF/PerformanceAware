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
set(PART1_HOMEWORK_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/computer_enhance/perfaware/part1")

set(8086DECODER_SOURCE 
src/8086Decoder/main.cpp
src/8086Decoder/CPUData.hpp
src/8086Decoder/GroupInstructions.hpp
src/8086Decoder/GroupInstructions.cpp
src/8086Decoder/MoveInstruction.hpp
src/8086Decoder/MoveInstruction.cpp
src/8086Decoder/AddInstruction.hpp
src/8086Decoder/AddInstruction.cpp
src/8086Decoder/SubInstruction.hpp
src/8086Decoder/SubInstruction.cpp
src/8086Decoder/CmpInstruction.hpp
src/8086Decoder/CmpInstruction.cpp
src/8086Decoder/JnzInstruction.hpp
src/8086Decoder/JnzInstruction.cpp)

add_executable(8086Decoder ${8086DECODER_SOURCE})

if (WIN32)
    target_compile_definitions(8086Decoder PRIVATE
                               _CRT_SECURE_NO_WARNINGS
                               WIN32_LEAN_AND_MEAN
                               NOMINMAX)
endif()

target_include_directories(8086Decoder SYSTEM PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${SRC_INCLUDE}
                                               ${RAPID_HASH_DIR}
                                               ${TLSF_INCLUDE_DIR}
                                               ${VENDER_INCLUDE}
                                               ${PART1_HOMEWORK_INCLUDE})

target_compile_definitions(8086Decoder PUBLIC BaseFilePath="${CMAKE_CURRENT_SOURCE_DIR}")

set(CPUSIM_SOURCE 
src/CPUSimulator/main.cpp)

add_executable(CPUSimulator ${CPUSIM_SOURCE})

if (WIN32)
    target_compile_definitions(CPUSimulator PRIVATE
                               _CRT_SECURE_NO_WARNINGS
                               WIN32_LEAN_AND_MEAN
                               NOMINMAX)
endif()

target_include_directories(CPUSimulator SYSTEM PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${SRC_INCLUDE}
                                               ${RAPID_HASH_DIR}
                                               ${TLSF_INCLUDE_DIR}
                                               ${VENDER_INCLUDE}
                                               ${PART1_HOMEWORK_INCLUDE})

target_compile_definitions(CPUSimulator PUBLIC BaseFilePath="${CMAKE_CURRENT_SOURCE_DIR}")

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
    target_link_libraries(8086Decoder PRIVATE
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

target_link_libraries(8086Decoder PRIVATE Foundation External)
target_link_libraries(CPUSimulator PRIVATE Foundation External)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${8086DECODER_SOURCE} ${CPUSIM_SOURCE} ${FOUNDATION_SOURCE})

target_compile_options(8086Decoder PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4>
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_compile_options(CPUSimulator PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4>
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 8086Decoder)
endif()