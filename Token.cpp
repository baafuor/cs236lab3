#include "Token.h"

const std::string Token::TypeNames[] = {
    "UNDEFINED",
    "COMMA", 
    "PERIOD", 
    "Q_MARK", 
    "LEFT_PAREN", 
    "RIGHT_PAREN", 
    "COLON", 
    "COLON_DASH", 
    "SCHEMES", 
    "FACTS", 
    "RULES", 
    "QUERIES", 
    "ID", 
    "STRING", 
    "EOF"
};

Token::Token(Types type, const std::string& value, unsigned int line) : type_(type), value_(value), line_(line) {
}

Token::Token(Types type, unsigned int line) : type_(type), line_(line) {
}

//Token::Token(unsigned int line) : Token(Types::UNDEFINED, line) {
//}

Token::~Token() {
}

Token::Types Token::getTypeId() const {
    return type_;
}

const std::string& Token::getType() const {
    return Token::TypeNames[static_cast<int>(type_)];
}

void Token::setType(Types type) {
    type_ = type;
}

const std::string& Token::get() const {
    return value_;
}

void Token::set(const std::string value) {
    value_ = value;
}

unsigned int Token::getLine() const {
    return line_;
}

std::string Token::toString() const {
    std::stringstream ss;

    ss << "(" << getType() << ",\"" << value_ << "\"," << line_ << ")";

    return ss.str();
}
