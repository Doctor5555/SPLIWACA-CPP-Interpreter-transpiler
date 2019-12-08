#pragma once
#include "Frontend/Lexer/LexicalError.h"
#include "Frontend/Parser/SyntaxError.h"

int RegisterLexicalError(Spliwaca::LexicalError e);
int RegisterSyntaxError(Spliwaca::SyntaxError e);
std::string mulString(std::string s, int i);

int numDigits(int32_t x);
