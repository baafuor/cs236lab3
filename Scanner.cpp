#include "Scanner.h"

Scanner::Scanner(const std::string& logFilename) {
    log_.open(logFilename);

    if (!log_.good())
        throw IOException("File does not exist\n");
}

Scanner::~Scanner() {
    clearVector();
    log_.close();
}

void Scanner::tokenize() {
    char c;
    unsigned int line = 1;

    while (log_.get(c)) {
        switch (c) {
            case ',':
                tokens_.push_back(Token(Token::Types::COMMA, ",", line));
                break;
            case '.':
                tokens_.push_back(Token(Token::Types::PERIOD, ".", line));
                break;
            case '?':
                tokens_.push_back(Token(Token::Types::Q_MARK, "?", line));
                break;
            case '(':
                tokens_.push_back(Token(Token::Types::LEFT_PAREN, "(", line));
                break;
            case ')':
                tokens_.push_back(Token(Token::Types::RIGHT_PAREN, ")", line));
                break;
            case ':':
                if (log_.peek() == '-') {
                    tokens_.push_back(Token(Token::Types::COLON_DASH, ":-", line));
                    log_.get();
                } else {
                    tokens_.push_back(Token(Token::Types::COLON, ":", line));
                }
                break;
            case '\'':
                tokens_.push_back(Token(Token::Types::STRING, scanString(line), line));
                break;
            case '#':
                scanComment(line);
                break;
            case '\n': //increments line counter
                line++;
                break;
            default:
                if (isspace(c))
                    break;

                std::string value = scanIdentifier(c, line);
                Token::Types type = checkKeyword(value);
                tokens_.push_back(Token(type, value, line));
                break;
        }
    }

    tokens_.push_back(Token(Token::Types::END_OF_FILE, line));
}

const std::string Scanner::scanString(unsigned int line) {
    char c;
    std::stringstream ss;

    ss << '\'';

    while (log_.get(c)) {
        if (c == '\n') //we reached end of line before string was terminated, this is an error
            error(line);

        ss << c;

        if (c == '\'') //end of string
            break;
    }
    
    return ss.str();
}

const std::string Scanner::scanIdentifier(char c, unsigned int line) {
    std::stringstream ss;

    //check if first letter is not a digit
    if (!isalpha(c))
        error(line);

    ss << c;

    while (isalnum(log_.peek())) {
        log_.get(c);
        ss << c;
    }
    
    return ss.str();
}

const Token::Types Scanner::checkKeyword(const std::string& identifier) {
    //check if the value is a keyword
    if (identifier == "Schemes")
        return Token::Types::SCHEMES;
    else if (identifier == "Facts")
        return Token::Types::FACTS;
    else if (identifier == "Rules")
        return Token::Types::RULES;
    else if (identifier == "Queries")
        return Token::Types::QUERIES;
    else //it's not a keyword
        return Token::Types::ID;
}


void Scanner::scanComment(unsigned int& line) {
    char c;

    while (log_.get(c)) {
        if (c == '\n') {
            line++;
            break;
        }
    }
}

void Scanner::error(unsigned int line) {
    std::stringstream ss;
    
    ss << "Input Error on line " << line << "\n";
    throw SyntaxError(ss.str());
}

const std::vector<Token>& Scanner::getTokens() const {
    return tokens_;
}

void Scanner::clearVector() {
    tokens_.clear();
}