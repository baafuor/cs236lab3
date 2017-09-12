#include "IOException.h"

IOException::IOException(const std::string& message) : runtime_error("I/O exception"), message_(message) {
}

IOException::~IOException() {
}

const std::string& IOException::message() const {
    return message_;
}