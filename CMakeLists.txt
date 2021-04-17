cmake_minimum_required(VERSION 3.3)
project(benkyo CXX)
AUX_SOURCE_DIRECTORY(./ exec) 
add_executable(benkyo ${exec})