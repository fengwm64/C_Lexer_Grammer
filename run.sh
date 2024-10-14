#!/bin/bash

clear

echo "==================== 词法分析 ===================="
# 编译 main 程序
g++ -std=c++11 -o ./bin/lex ./src/Lexer/main.cpp ./src/Lexer/myLex.cpp

# 运行 main 程序
./bin/lex
echo

# 编译 myScannerCode 程序
gcc -o ./bin/myScanner ./src/Lexer/myScannerCode.c -w

# 运行 myScannerCode 程序，并提供测试文件作为参数
./bin/myScanner ./src/testCode/test_1.c

echo "词法分析完毕，请查看token文件"

echo

echo "==================== 语法分析 ===================="
g++ -o ./bin/grammer ./src/Grammer/main.cpp ./src/Grammer/Grammer.cpp ./src/Grammer/Lexer.cpp -w
./bin/grammer
echo "语法分析完毕，请查看 语法分析结果.gr 文件"