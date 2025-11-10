#[[VScode 自动配置运行cmake_build时使用的指令
E:\Cmake\bin\cmake.EXE -DCMAKE_BUILD_TYPE:STRING=Debug 
-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE 
-DCMAKE_C_COMPILER:FILEPATH=E:\msys64\ucrt64\bin\gcc.exe 
-DCMAKE_CXX_COMPILER:FILEPATH=E:\msys64\ucrt64\bin\g++.exe --no-warn-unused-cli 
-Smake E:/workspace/testspace/C++/helloworld 
-B e:/workspace/testspace/C++/helloworld/build 
-G "MinGW Makefiles"

E:\Cmake\bin\cmake.EXE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=E:\msys64\ucrt64\bin\gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=E:\msys64\ucrt64\bin\g++.exe --no-warn-unused-cli -Smake E:/workspace/math/C++ -B e:/workspace/math/C++/build -G "MinGW Makefiles"

E:\Cmake\bin\cmake.EXE -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=E:\msys64\ucrt64\bin\gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=E:\msys64\ucrt64\bin\g++.exe --no-warn-unused-cli -Smake E:/workspace/math/C++ -B e:/workspace/math/C++/build -G "MinGW Makefiles"

E:\Cmake\bin\cmake.EXE -B e:/workspace/math/C++/build -G "MinGW Makefiles"
]]

#[[cmake的编译选项 --no-warn-unused-cli
    对于定义但是未使用的变量不进行警告
]]


#[[
CMAKE_BUILD_TYPE：用于控制构建类型
    Debug：调试模式，完全不优化程序、生成调试信息
    Release：发布模式，优化程度最高，性能最佳，但是编译比Debug慢
    MinSizeRel：最小体积发布，生成的文件比Release更小，不完全优化，减少二进制体积
    RelWithDebInfo：带调试信息发布，生成的文件比Release更大

]]
#若未指定CMAKE_BUILD_TYPE，则设定其默认值为Release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#[[
CMAKE_EXPORT_COMPILE_COMMANDS：用于控制指令文本的保存开关
    OFF：默认选项
    ON：将生成文件 compile_commands.json
]]
if (NOT CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

if (NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER "E:/msys64/ucrt64/bin/gcc.exe")
endif()

if (NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER "E:/msys64/ucrt64/bin/g++.exe")
endif()

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()

cmake_minimum_required(VERSION 3.10)
#set(CMAKE_C_COMPILER "E:/msys64/ucrt64/bin/gcc")
#set(CMAKE_CXX_COMPILER "E:/msys64/ucrt64/bin/g++")
project(math_project)
message(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
message(${SRC_LIST})
add_executable(app main.cpp ${SRC_LIST})