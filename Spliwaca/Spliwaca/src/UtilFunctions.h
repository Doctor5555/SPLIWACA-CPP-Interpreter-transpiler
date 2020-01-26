#pragma once
#include "Frontend/Lexer/LexicalError.h"
#include "Frontend/Parser/SyntaxError.h"

int RegisterLexicalError(Spliwaca::LexicalError e);
int RegisterSyntaxError(Spliwaca::SyntaxError e);
int RegisterMissingVariable(uint32_t lineNumber, uint32_t columnNumber);
std::string mulString(std::string s, int i);

int numDigits(int32_t x);

bool charInStr(const std::string& s, char c);
//template<typename T>
//bool itemInVect(const std::vector<T>& v, T t);