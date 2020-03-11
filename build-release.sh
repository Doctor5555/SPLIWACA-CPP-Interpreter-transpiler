#!/bin/bash
cd Spliwaca/Spliwaca/src
clang++ -std=c++11 -stdlib=libc++ -O3 Main.cpp Transpiler.cpp Log.cpp Frontend/Lexer/Lexer.cpp Frontend/Parser/Parser.cpp Frontend/Parser/IdentNode.cpp Backend/Code\ Generation/Generator.cpp -I . -I ../../vendor/spdlog/include --define-macro SPLW_DIST -o ../../../transpiler
cd -
