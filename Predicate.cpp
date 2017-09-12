#include "Predicate.h"

Predicate::Predicate(std::string name) : name_(name) {
}

Predicate::~Predicate() {
    clearVectors();
}

const std::string& Predicate::getName() const {
    return name_;
}

const std::vector<Parameter>& Predicate::getParameters() const {
    return parameters_;
}

void Predicate::addParameter(const Parameter param) {
    parameters_.push_back(param);
}

std::string Predicate::toString() const {
    std::string comma = "";
    std::stringstream ss;
    
    ss << name_ << "(";
    
    for (Parameter parameter : parameters_) {
        ss << comma << parameter.getValue();
        comma = ",";
    }
    
    ss << ")";
    
    return ss.str();
}

void Predicate::clearVectors() {
    parameters_.clear();
}