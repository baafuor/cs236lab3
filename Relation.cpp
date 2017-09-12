#include "Relation.h"

Relation::Relation() {}

Relation::Relation(const Predicate& scheme) : Relation(scheme.getName(), scheme.getParameters()) {}

Relation::Relation(const std::string& name, const Scheme& scheme) : name_(name), scheme_(scheme) {}

Relation::Relation(const std::string& name, const Scheme& scheme, const std::set<Tuple>& tuples) : name_(name), scheme_(scheme), tuples_(tuples) {}

Relation::~Relation() {
}

Relation Relation::select(const Predicate& query) const {
    Relation select = Relation(name_, scheme_);

    for (const Tuple& tuple : tuples_) {
        if (checkValues(query, tuple)) {
            select.addTuple(tuple);
        }
    }

    return select;
}

Relation Relation::project(const std::vector<int>& positions) const {
    Scheme scheme;
    std::vector<std::string> values;

    for (auto pos : positions) {
        scheme.addName(scheme_.getNames()[pos]);
    }
    
    Relation project = Relation(name_, scheme);

    for (const Tuple& tuple : tuples_) {
        for (auto pos : positions) {
            values.push_back(tuple.getValues()[pos]);
        }
        
        if (values.size() > 0) {
            project.addTuple(Tuple(values));
            values.clear();
        }
    }
    
    return project;
}

Relation Relation::rename(const Predicate& query, const std::vector<int>& positions) const {
    Scheme scheme;

    for (const auto& pos : positions) {
        scheme.addName(query.getParameters()[pos].getValue());
    }
    
    return Relation(name_, scheme, tuples_);
}

Relation Relation::joinWith(const Relation& relation) {
    std::vector<std::pair<int, int>> sourcePos;
    Relation temp("temp", combineSchemes(relation.scheme_, sourcePos));
    
    for (const Tuple& t1 : tuples_) {
        for (const Tuple& t2 : relation.tuples_) {
            if (canJoin(t1, t2, sourcePos))
                temp.performJoin(t1, t2, sourcePos);
        }
    }
    
    return temp;
}

int Relation::unionWith(const Relation& relation, std::stringstream& ss) {
    int initialSize = tuples_.size();
    
    for (Tuple t : relation.tuples_) {
        if (addTuple(t)) {
            int i = 0;
            ss << "  ";

            std::string space;
            for (const std::string& str : t.getValues()) {
                ss << space << scheme_.getNames()[i++] << "=" << str;
                space = " ";
            }

            ss << "\n";
        }
    }
    
    return tuples_.size() - initialSize;
}

Scheme Relation::combineSchemes(const Scheme& scheme, std::vector<std::pair<int, int>>& sourcePos) {
    //we copy-construct a new scheme with the scheme of this relation
    Scheme s(scheme_);

    int i = 0;
    sourcePos.resize(scheme_.getNames().size());
    std::generate(sourcePos.begin(), sourcePos.end(), [&i]{return std::make_pair(i++, -1);});
    
    //we add only the names from the other scheme that don't exist already
    for (int i = 0; i < scheme.getNames().size(); i++) {
        bool found = false;
        
        for (int j = 0; j < scheme_.getNames().size(); j++) {
            if (scheme.getNames().at(i) == scheme_.getNames().at(j)) {
                sourcePos.at(j) = std::make_pair(j, i);
                found = true;
            } 
        }
        
        if (!found) {
            s.addName(scheme.getNames().at(i));
            sourcePos.push_back(std::make_pair(-1, i));
        }
    }
    
    return s;
}

bool Relation::canJoin(const Tuple& t1, const Tuple& t2, const std::vector<std::pair<int, int>>& sourcePos) {
    for (auto pos : sourcePos) {
        if (pos.first != -1 && pos.second != -1) {
            if (t1.getValues().at(pos.first) != t2.getValues().at(pos.second)) {
                //we can't join these tuples, so we return false
                return false;
            }
        }
    }
    
    //these tuples are "joinable"
    return true;
}

void Relation::performJoin(const Tuple& t1, const Tuple& t2, const std::vector<std::pair<int, int>>& sourcePos) {
    //we create the new tuple into the relation product of the join
    std::vector<std::string> params;

    for (auto pos : sourcePos) {
        params.push_back(pos.first != -1 ? t1.getValues().at(pos.first) : t2.getValues().at(pos.second));
    }

    addTuple(Tuple(params));
}

bool Relation::addTuple(const Tuple& tuple) {
    std::pair<TupleIt, bool> result = tuples_.insert(tuple);
    
    //returns true if a new tuple was added to the set, false otherwise
    return result.second;
}

const std::string& Relation::getName() const {
    return name_;
}

const Scheme& Relation::getScheme() const {
    return scheme_;
}

const std::set<Tuple>& Relation::getTuples() const {
    return tuples_;
}

std::string Relation::toString() const {
    std::stringstream ss;

    for (const Tuple& tuple : tuples_) {
        int i = 0;
        ss << "  ";

        std::string space;
        for (const std::string& str : tuple.getValues()) {
            ss << space << scheme_.getNames()[i++] << "=" << str;
            space = " ";
        }

        ss << "\n";
    }

    return ss.str();
}

bool Relation::checkValues(const Predicate& query, const Tuple& tuple) const {
    bool equal = true;
    int index = 0;
    const std::vector<Parameter>& parameters = query.getParameters();
    const std::vector<std::string>& values = tuple.getValues();
    VarMap vmap;

    do {
        if (parameters[index].isString())
            equal = parameters[index].getValue() == values[index];
        else {
            VarMapIterator it = vmap.find(parameters[index].getValue());
            if (it != vmap.end()) { //exists
                equal = it->second == values[index];
            }
            else
                vmap.insert(VarMapType(parameters[index].getValue(), values[index]));
        }
    } while (equal && ++index < parameters.size());
    
    return equal;
}
