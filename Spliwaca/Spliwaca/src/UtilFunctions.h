#pragma once
#include "Frontend/Lexer/LexicalError.h"
#include "Frontend/Parser/SyntaxError.h"
#include "Frontend/Parser/SemanticError.h"

int RegisterLexicalError(uint8_t errorCode, uint32_t lineNumber, uint32_t columnNumber, uint16_t columnSpan = 1);
int RegisterSyntaxError(Spliwaca::SyntaxErrorType type, std::shared_ptr<Spliwaca::Token> token);
int RegisterSyntaxError(Spliwaca::SyntaxErrorType errorCode, uint32_t lineNumber, uint32_t columnNumber, size_t columnSpan, Spliwaca::TokenType type);
int RegisterSemanticError(Spliwaca::SemanticErrorType errorCode, std::shared_ptr<Spliwaca::Token> token);
int RegisterSemanticError(Spliwaca::SemanticErrorType errorCode, uint32_t lineNumber, uint32_t columnNumber, size_t columnSpan, Spliwaca::TokenType type);

std::string mulString(std::string s, int i);

int numDigits(int32_t x);

bool charInStr(const std::string& s, char c);
//template<typename T>
//bool itemInVect(const std::vector<T>& v, T t);