cmake_minimum_required(VERSION 3.15)

project(S LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(s 
    src/S.cc
    src/Token.cc
    src/Scanner.cc
    src/AstPrinter.cc
)
