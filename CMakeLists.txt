cmake_minimum_required(VERSION 3.8)
 
project(HelloGL VERSION 0.1.0)
 
# 使用 C++ 11 标准
set(CMAKE_CXX_STANDARD 11)
 
# 添加头文件
set(THIRD_PARTY_H ${PROJECT_SOURCE_DIR}/include)
include_directories(${THIRD_PARTY_H})
 
# 添加目标链接
set(LIB_PATH ${PROJECT_SOURCE_DIR}/lib)
file(GLOB_RECURSE LIB_FILES
	"${LIB_PATH}/*.lib"
)
message("............ LIB_PATH: ${LIB_PATH}")
message("............ LIB_FILES: ${LIB_FILES}")
link_directories(${LIB_PATH})
 
aux_source_directory(./src DIR_SRCS)
add_executable(HelloGL ${DIR_SRCS})
target_link_libraries(HelloGL ${LIB_FILES})