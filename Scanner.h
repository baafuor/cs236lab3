#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Token.h"
#include "IOException.h"
#include "SyntaxError.h"

class Scanner {
public:
    Scanner(const std::string& logFilename);
    ~Scanner();

    void tokenize();
    const std::vector<Token>& getTokens() const;

private:
    const std::string scanString(unsigned int line);
    const std::string scanIdentifier(char c, unsigned int line);
    const Token::Types checkKeyword(const std::string& identifier);
    void scanComment(unsigned int& line);
    void error(unsigned int line);
    void clearVector();
    
    std::ifstream log_;
    std::vector<Token> tokens_;
};

