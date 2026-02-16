cmake_minimum_required(VERSION 3.10)

# 定义静态库
add_library(gm_spi STATIC gm_spi.c)

# 添加头文件搜索路径
target_include_directories(gm_spi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
