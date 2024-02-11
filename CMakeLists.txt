
##
## earthling Project Details
##
## (c) 2023 David Rowbotham (thedaver64@gmail.com)
##
##
cmake_minimum_required(VERSION 3.0...3.22)
project("dfrc")
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "64 bit compiler required.")
endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

set(CMAKE_CXX_STANDARD 20)
set(Boost_NO_WARN_NEW_VERSIONS 1)

##
## MSVC Compile Flags
##
if(MSVC)
set(Boost_INCLUDE_DIR "C:\\Users\\daver\\LocalApps\\boost_1_84_0\\")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "/std:c++20 -I../CPPINCLUDE /EHsc /Qpar /GS- /O2 /D NDEBUG")
endif(MSVC)


##
## Linux Compile Flags
##
if(UNIX)
set(CMAKE_CXX_FLAGS "-std=c++20 -I../CPPINCLUDE -O2")
endif(UNIX)


##
## MINGW. Not supported atm.
##
if(MINGW)
set(CMAKE_CXX_FLAGS "-std=c++20 -I../CPPINCLUDE -O2")
endif(MINGW)


if(POLICY CMP0148) 
cmake_policy(SET CMP0148 OLD) 
endif()
##
## Project Build.
##
if(MSVC)
add_executable(dfrc
    CPPSOURCE/main.cpp
)
endif(MSVC)

if(UNIX)
add_executable(dfrc
    CPPSOURCE/main.cpp
)
endif(UNIX)


##
## MSVC Platform Link Options
##
if(MSVC)
set_target_properties(dfrc PROPERTIES LINK_FLAGS "/LIBPATH:../lib/win64 //ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE")
#target_link_libraries(earthling)
endif(MSVC)

##
## Linux Platform Link Options
##
if(UNIX)
target_link_libraries(dfrc)
endif(UNIX)