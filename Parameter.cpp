#include "Parameter.h"

Parameter::Parameter(bool isString, std::string value) : value_(value), isString_(isString) {
}

Parameter::~Parameter() {
}

const bool& Parameter::isString() const {
    return isString_;
}

const std::string& Parameter::getValue() const {
    return value_;
}