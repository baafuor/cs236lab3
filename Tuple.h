#ifndef TUPLE_H
#define TUPLE_H
#include <string>
#include <vector>
#include "Parameter.h"

class Tuple {
public:
    Tuple(const std::vector<Parameter>& parameters);
    Tuple(const std::vector<std::string>& parameters);
    virtual ~Tuple();

    const std::vector<std::string>& getValues() const;
    bool operator<(const Tuple& other) const;

private:
    std::vector<std::string> attrib_values_;
};

#endif /* TUPLE_H */

