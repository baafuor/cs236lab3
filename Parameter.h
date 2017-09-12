#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

class Parameter {
public:
    Parameter(bool isString, std::string value);
    virtual ~Parameter();
    
    const bool& isString() const;
    const std::string& getValue() const;

private:
    std::string value_;
    bool isString_;
};

#endif /* PARAMETER_H */

