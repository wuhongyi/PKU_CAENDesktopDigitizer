#CMakeLists.txt --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 六 2月 22 19:39:08 2014 (+0800)
# Last-Updated: 一 9月  6 21:31:29 2021 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 119
# URL: http://wuhongyi.cn 

#----------------------------------------------------------------------------
#判断cmake版本是否符合，对工程命名
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#set(CODE_DIR /home/wuhongyi/CodeProject )   #设置工程文件路径
#set(CMAKE_C_COMPILER "/usr/local/bin/gcc")  #调用自己安装在local的gcc
#set(CMAKE_CXX_COMPILER "/usr/local/bin/g++") #调用自己安装在local的g++
set(PROJ_NAME pkuDigitizer)
project(${PROJ_NAME})
SET(CMAKE_BUILD_TYPE Debug) #编译调试版本

##C99  ROOT不支持C++11采用以下两行
set(CMAKE_CXX_FLAGS " -fPIC -W -Wall -s")
set(CMAKE_C_FLAGS " -fPIC -W -Wall -s")
##C++11  ROOT支持C++11采用以下两行
# set(CMAKE_CXX_FLAGS "-std=c++11  -fPIC -W -Wall -s")
# set(CMAKE_C_FLAGS "-std=c++11  -fPIC   -W -Wall -s")

#----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}//cmake) #设置 FindROOT.cmake 文件路径
# Find ROOT (required package)
#find_package(ROOT REQUIRED)
find_package( ROOT COMPONENTS ASImage EG FFTW FitPanel Ged Geom Gui GuiBld GuiHtml GX11 GX11TTF Hbook Hist Html MathMore Minuit Minuit2 MLP Proof RGL RooFit RooFitCore Smatrix Spectrum Table TMVA TreePlayer TreeViewer )#COMPONENTS可选 ，REQUIRED COMPONENTS必须有
#----------------------------------------------------------------------------
#指出所需hh，cc，so，a文件位置路径，并用变量命名
include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS} )
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*)
file(GLOB libs ${PROJECT_SOURCE_DIR}/lib/* )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build) #生成可执行文件放在bin文件夹中


set(headerfiles ${PROJECT_SOURCE_DIR}/include/*.hh )
set(linkdefs ${PROJECT_SOURCE_DIR}/include/MainLinkdef.h )

add_custom_command(OUTPUT MainDict.cxx 
                   PRE_BUILD
                   COMMAND rootcint -f  MainDict.cxx -c  ${headerfiles} ${linkdefs} )#COMMENT echo "executing a command -> rootcint "
add_library(MainDict SHARED MainDict.cxx)#生成动态链接库
#----------------------------------------------------------------------------
add_executable(${PROJ_NAME} main.cc ${sources} ${headers}) 

target_link_libraries(${PROJ_NAME} MainDict ${libs} ${ROOT_LIBRARIES} CAENComm CAENDigitizer -pthread m dl -rdynamic ncurses c)

#----------------------------------------------------------------------------
#将所需数据文件复制到bin文件夹
set(CopyFile
    rootlogon.C rootlogoff.C
  )

foreach(copyfile ${CopyFile})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${copyfile}
    ${PROJECT_SOURCE_DIR}/build/${copyfile}
    COPYONLY
    )
endforeach()

###################################
