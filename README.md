# S

A simple interpreted programming language.

S is a programming language designed for creating and running scripts. It provides features for defining variables, control flow structures like if statements and loops, and basic input/output operations. This language can be used for scripting tasks or as a learning tool for understanding compiler and interpreter design.

## Tech Stack

* C++20
* CMake

## Quick Start

1. **Build the project:**
 ```bash
 cmake -S . -B build
 cmake --build build
 ```
2. **Run the interpreter:**
 * To run an interactive prompt:
 ```bash
 ./build/s
 ```
 * To run a script file:
 ```bash
 ./build/s <script_path>
 ```


