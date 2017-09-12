#include "Scheme.h"

Scheme::Scheme() {}

Scheme::Scheme(const Scheme& other) {
    attrib_names_ = other.attrib_names_;
}

Scheme::Scheme(const std::vector<Parameter>& parameters) {
    for (Parameter prm : parameters) {
        attrib_names_.push_back(prm.getValue());
    }
}

Scheme::~Scheme() {
}

void Scheme::addName(std::string name) {
    attrib_names_.push_back(name);
}

const std::vector<std::string>& Scheme::getNames() const {
    return attrib_names_;
}