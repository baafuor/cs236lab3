#include "Tuple.h"

Tuple::Tuple(const std::vector<Parameter>& parameters) {
    for (Parameter prm : parameters) {
        attrib_values_.push_back(prm.getValue());
    }
}

Tuple::Tuple(const std::vector<std::string>& parameters) {
    attrib_values_ = parameters;
}

Tuple::~Tuple() {
}

bool Tuple::operator<(const Tuple& other) const {
    for (int i = 0; i < attrib_values_.size(); i++) {
        int cmp = attrib_values_[i].compare(other.attrib_values_[i]);
        if (cmp < 0)
            return true;
        else if (cmp > 0)
            return false;
    }
    
    return false;
}

const std::vector<std::string>& Tuple::getValues() const {
    return attrib_values_;
}