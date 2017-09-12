#ifndef SYNTAXERROR_H
#define SYNTAXERROR_H

#include <stdexcept>
#include <string>
#include <sstream>
#include "Token.h"

class SyntaxError : public std::runtime_error {
public:
    SyntaxError(const std::string& message);
    virtual ~SyntaxError();
    
    const std::string& message() const;

private:
    std::string message_;
};

#endif /* SYNTAXERROR_H */

