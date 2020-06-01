#!/bin/bash
cd Spliwaca/Spliwaca/src
clang++ -std=c++17 -stdlib=libc++ Main.cpp Transpiler.cpp Log.cpp Frontend/Lexer/Lexer.cpp Frontend/Parser/Parser.cpp Frontend/Parser/IdentNode.cpp Backend/Code\ Generation/Generator.cpp -I . -I ../../vendor/spdlog/include --define-macro SPLW_DEBUG -o ../../../transpiler
cd -
