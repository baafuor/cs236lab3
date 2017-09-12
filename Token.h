#pragma once
#include <string>
#include <fstream>
#include <sstream>

class Token {
public:

    enum class Types : char {
        UNDEFINED,
        COMMA,
        PERIOD,
        Q_MARK,
        LEFT_PAREN,
        RIGHT_PAREN,
        COLON,
        COLON_DASH,
        SCHEMES,
        FACTS,
        RULES,
        QUERIES,
        ID,
        STRING,
        END_OF_FILE
    };

    Token(Types type, const std::string& value, unsigned int line);
    Token(Types type, unsigned int line);
    //Token(unsigned int line);
    ~Token();

    static const std::string TypeNames[];

    Types getTypeId() const;
    const std::string& getType() const;
    void setType(Types type);
    const std::string& get() const;
    void set(const std::string value);
    unsigned int getLine() const;
    std::string toString() const;

private:
    Types type_;
    std::string value_;
    unsigned int line_;
};

