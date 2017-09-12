#ifndef RULE_H
#define RULE_H

#include "Predicate.h"

class Rule {
public:
    Rule(const Predicate& predicate);
    virtual ~Rule();
    
    const Predicate& getHead() const;
    std::vector<Predicate>& getPredicates();
    const std::vector<Predicate>& getPredicates() const;
    std::string toString() const;

private:
    Predicate head_;
    std::vector<Predicate> predicates_;
};

#endif /* RULE_H */

