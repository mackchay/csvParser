cmake_minimum_required(VERSION 3.24)
project(CSVparser)

set(CMAKE_CXX_STANDARD 23)

add_executable(CSVparser main.cpp Print/print.cpp Print/print.h CSVParser/CSVParser.cpp CSVParser/CSVParser.h TupleUtils/TupleUtils.cpp TupleUtils/TupleUtils.h Read/Read.cpp Read/Read.h)
