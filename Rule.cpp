#include <vector>

#include "Rule.h"

Rule::Rule(const Predicate& predicate) : head_(predicate) {}

Rule::~Rule() {
    predicates_.clear();
}

const Predicate& Rule::getHead() const {
    return head_;
}

std::vector<Predicate>& Rule::getPredicates() {
    return predicates_;
}

const std::vector<Predicate>& Rule::getPredicates() const {
    return predicates_;
}

std::string Rule::toString() const {
    std::string comma = "";
    std::stringstream ss;
    
    ss << head_.toString() << " :- ";
    
    for (auto predicate : predicates_) {
        ss << comma << predicate.toString();
        comma = ",";
    }
    
    return ss.str();
}