cmake_minimum_required(VERSION 3.10)

# 定义静态库
add_library(icm20608 STATIC icm20608.c)

# 添加头文件搜索路径
target_include_directories(icm20608 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 添加链接库
target_link_libraries(icm20608 gm_spi)
