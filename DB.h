#ifndef DB_H
#define DB_H
#include <map>
#include <array>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Relation.h"
#include "Rule.h"
#include "Predicate.h"

typedef std::map<std::string, Relation> Map;
typedef Map::value_type MapType;

class DB {
public:
    DB();
    virtual ~DB();
    
    std::string processSchemes(const std::vector<Predicate>& schemes);
    std::string processFacts(const std::vector<Predicate>& facts);
    std::string optimizeRuleEval(const std::vector<Rule>& rules);
    std::string evalRules(const std::vector<Rule>& rules);
    std::string evalQueries(const std::vector<Predicate>& queries);

private:
    void getUniqueColumns(const Predicate& query, std::vector<int>& positions);
    void projectRule(const Scheme& scheme, const Predicate& head, std::array<std::vector<int>, 2>& positions);
    void relationsToString(std::stringstream& ss);
    bool mustRepeatEvaluation(const std::vector<Rule>& rules);
    
    Map relations_;
};

#endif /* DB_H */

