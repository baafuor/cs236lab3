#ifndef SCHEME_H
#define SCHEME_H
#include <string>
#include <vector>
#include "Parameter.h"

class Scheme {
public:
    Scheme();
    Scheme(const Scheme& other);
    Scheme(const std::vector<Parameter>& parameters);
    virtual ~Scheme();
    
    void addName(std::string name);
    const std::vector<std::string>& getNames() const;

private:
    std::vector<std::string> attrib_names_;
};

#endif /* SCHEME_H */

