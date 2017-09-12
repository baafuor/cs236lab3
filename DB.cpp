#include "DB.h"
#include "Rule.h"
#include "Graph.h"

DB::DB() {
}

DB::~DB() {
}

std::string DB::processSchemes(const std::vector<Predicate>& schemes) {
    for (const Predicate& scheme : schemes) {
        relations_.insert(MapType(scheme.getName(), Relation(scheme)));
    }

    return "Scheme Evaluation\n\n";
}

std::string DB::processFacts(const std::vector<Predicate>& facts) {
    std::stringstream ss;

    ss << "Fact Evaluation\n\n";

    for (const Predicate& fact : facts) {
        relations_[fact.getName()].addTuple(Tuple(fact.getParameters()));
    }

    relationsToString(ss);
    
    return ss.str();
}

std::string DB::optimizeRuleEval(const std::vector<Rule>& rules) {
    Graph graph(rules);
    std::stringstream ss;
    
    graph.buildDependencyGraphs();
    graph.dfs();
    graph.sccdfs();
    
    ss << "Rule Evaluation\n" << graph.toString();

    std::vector<int> order = graph.sortByPostOrderNumber();
    std::vector<std::set<int>> sccs = graph.getSCCs();
    
    for (int index : order) {
        std::vector<Rule> sccsRules;
        
        if (sccs[index].size() > 0) {
            ss << "\nSCC:";
            for (int i : sccs[index]) {
                ss << " R" << i;
                sccsRules.push_back(rules[i]);
            }
    
            ss << "\n" << evalRules(sccsRules);
        }
    }
    
    ss << "\nRule Evaluation Complete\n\n";
    
    relationsToString(ss);
    
    return ss.str();
}

std::string DB::evalRules(const std::vector<Rule>& rules) {
    std::stringstream ss;
    int iterations = 0, totalCount;
    
    bool mustRepeat = mustRepeatEvaluation(rules);
    
    do {
        totalCount = 0;
        iterations++;
        
        for (auto rule : rules) {
            std::vector<Relation> relations;
            const Predicate& head = rule.getHead();

            ss << rule.toString() << "\n";
            
            for (Predicate predicate : rule.getPredicates()) {
                const Relation& relation = relations_[predicate.getName()];
                const Relation& select = relation.select(predicate);

                std::vector<int> positions;
                getUniqueColumns(predicate, positions);

                const Relation& project = select.project(positions);
                relations.push_back(project.rename(predicate, positions));
            }

            Relation join = relations[0];
            for (auto i = std::next(relations.begin(), 1); i < relations.end(); i++) {
                join = join.joinWith(*i);
            }

            std::array<std::vector<int>, 2> positions;
            projectRule(join.getScheme(), head, positions);

            const Relation& project = join.project(positions[0]);
            const Relation& rename = project.rename(head, positions[1]);
            
            Relation& relation = relations_[head.getName()];
            totalCount += relation.unionWith(rename, ss);
        }
    } while (totalCount > 0 && mustRepeat);
    
    return ss.str();
}

bool DB::mustRepeatEvaluation(const std::vector<Rule>& rules) {
    if (rules.size() == 1) {
        std::string headName = rules[0].getHead().getName();

        for (auto predicate : rules[0].getPredicates()) {
            if (predicate.getName() == headName) {
                return true;
            }
        }
    } else
        return true;
    
    return false;
}

std::string DB::evalQueries(const std::vector<Predicate>& queries) {
    std::stringstream ss;

    ss << "Query Evaluation\n\n";

    for (const Predicate& query : queries) {
        const Relation& relation = relations_.at(query.getName());
        const Relation& select = relation.select(query);

        std::vector<int> positions;
        getUniqueColumns(query, positions);

        const Relation& project = select.project(positions);
        const Relation& rename = project.rename(query, positions);

        int size = select.getTuples().size();
        ss << query.toString() << "? ";
        if (size > 0) {
            ss << "Yes(" << size << ")\n";
            ss << "select\n" << select.toString();
            ss << "project\n" << project.toString();
            ss << "rename\n" << rename.toString();
        } else
            ss << "No\n";

        ss << "\n";
    }

    return ss.str();
}

void DB::getUniqueColumns(const Predicate& query, std::vector<int>& positions) {
    for (int i = 0; i < query.getParameters().size(); i++) {
        const Parameter& prm = query.getParameters()[i];
        if (!prm.isString()) {
            bool isDuplicated = false;
            //see if its duplicated
            for (int j = 0; j < i; j++) {
                if (prm.getValue() == query.getParameters()[j].getValue()) {
                    isDuplicated = true;
                    break;
                }
            }
            if (!isDuplicated) {
                positions.push_back(i);
            }
        }
    }
}

void DB::projectRule(const Scheme& scheme, const Predicate& head, std::array<std::vector<int>, 2>& positions) {
    int i = 0;
    
    for (const auto& prm : head.getParameters()) {
        //we find the parameter in the scheme to get its position
        for (int j = 0; j < scheme.getNames().size(); j++) {
            if (scheme.getNames()[j] == prm.getValue()) {
                positions[0].push_back(j);
            }
        }
        positions[1].push_back(i++);
    }
}

void DB::relationsToString(std::stringstream& ss) {
    for (auto it : relations_) {
        Relation relation = it.second;
        ss << relation.getName() << "\n" << relation.toString() << "\n";
    }
}