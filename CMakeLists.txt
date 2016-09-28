cmake_minimum_required(VERSION 3.0)
project(lc2ksim)

aux_source_directory(src SRC_LIST)

add_executable(lc2ksim ${SRC_LIST})
