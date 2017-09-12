#include "SyntaxError.h"

SyntaxError::SyntaxError(const std::string& message) : std::runtime_error("Syntax error"), message_(message) {
}

SyntaxError::~SyntaxError() {
}

const std::string& SyntaxError::message() const {
    return message_;
}