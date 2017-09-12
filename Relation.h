#ifndef RELATION_H
#define RELATION_H
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include "Predicate.h"
#include "Parameter.h"
#include "Scheme.h"
#include "Tuple.h"

typedef std::map<std::string, std::string> VarMap;
typedef VarMap::value_type VarMapType;
typedef VarMap::const_iterator VarMapIterator;
typedef std::set<Tuple>::iterator TupleIt;

class Relation {
public:
    Relation();
    Relation(const Predicate& scheme);
    Relation(const std::string& name, const Scheme& scheme);
    Relation(const std::string& name, const Scheme& scheme, const std::set<Tuple>& tuples);
    virtual ~Relation();
    
    Relation select(const Predicate& query) const;
    Relation project(const std::vector<int>& positions) const;
    Relation rename(const Predicate& query, const std::vector<int>& positions) const;
    Relation joinWith(const Relation& relation);
    int unionWith(const Relation& relation, std::stringstream& ss);
    
    bool addTuple(const Tuple& tuple);
    const std::string& getName() const;
    const Scheme& getScheme() const;
    const std::set<Tuple>& getTuples() const;

    std::string toString() const;

private:
    //checks if the values of a tuple pass the specified query
    bool checkValues(const Predicate& query, const Tuple& tuple) const;
    //combines a scheme with the scheme of this relation
    Scheme combineSchemes(const Scheme& scheme, std::vector<std::pair<int, int>>& sourcePos);
    bool canJoin(const Tuple& t1, const Tuple& t2, const std::vector<std::pair<int, int>>& sourcePos);
    void performJoin(const Tuple& t1, const Tuple& t2, const std::vector<std::pair<int, int>>& sourcePos);

    std::string name_;
    Scheme scheme_;
    std::set<Tuple> tuples_;
};

#endif /* RELATION_H */

