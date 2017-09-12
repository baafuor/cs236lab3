#ifndef PREDICATE_H
#define PREDICATE_H

#include <vector>
#include <sstream>
#include "Parameter.h"

class Predicate {
public:
    Predicate(std::string name);
    virtual ~Predicate();
    
    const std::string& getName() const;
    const std::vector<Parameter>& getParameters() const;
    void addParameter(const Parameter param);
    std::string toString() const;
    
private:
    void clearVectors();
    
    std::string name_;
    std::vector<Parameter> parameters_;
};

#endif /* PREDICATE_H */

