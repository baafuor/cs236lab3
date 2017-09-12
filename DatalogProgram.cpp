#include <iostream>

#include "DatalogProgram.h"

DatalogProgram::DatalogProgram(const std::vector<Token>& tokens) : tokens_(tokens), currentIndex_(0) {
}

DatalogProgram::~DatalogProgram() {
    clearVectors();
}

void DatalogProgram::parse() {
    test(Token::Types::SCHEMES);
    test(Token::Types::COLON);
    scheme();
    schemeList();
    
    test(Token::Types::FACTS);
    test(Token::Types::COLON);
    factList();
    
    test(Token::Types::RULES);
    test(Token::Types::COLON);
    ruleList();
    
    test(Token::Types::QUERIES);
    test(Token::Types::COLON);
    query();
    queryList();
    
    test(Token::Types::END_OF_FILE);
}

 const std::vector<Predicate>& DatalogProgram::getSchemes() const {
    return schemes_;
}

const std::vector<Predicate>& DatalogProgram::getFacts() const {
    return facts_;
}

const std::vector<Rule>& DatalogProgram::getRules() const {
    return rules_;
}

const std::vector<Predicate>& DatalogProgram::getQueries() const {
    return queries_;
}

const std::set<std::string>& DatalogProgram::getDomain() const {
    return domain_;
}

std::string DatalogProgram::toString() const {
    std::stringstream ss;
    
    ss << "Success!\n";

    ss << "Schemes(" << schemes_.size() << "):\n";
    for (Predicate scheme : schemes_) {
        ss << "  " << scheme.toString() << "\n";
    }

    ss << "Facts(" << facts_.size() << "):\n";
    for (Predicate fact : facts_) {
        ss << "  " << fact.toString() << "\n";
    }

    ss << "Rules(" << rules_.size() << "):\n";
    for (Rule rule : rules_) {
        ss << "  " << rule.toString() << "\n";
    }

    ss << "Queries(" << queries_.size() << "):\n";
    for (Predicate query : queries_) {
        ss << "  " << query.toString() << "\n";
    }

    ss << "Domain(" << domain_.size() << "):\n";
    for (std::string domain : domain_) {
        ss << "  " << domain << "\n";
    }
    
    return ss.str();
}

void DatalogProgram::error() {
    std::stringstream ss;
    
    ss << "Failure!\n  (" << currentToken().getType() << ",\"" 
            << currentToken().get() << "\"," 
            << currentToken().getLine() << ")\n";
    
    throw SyntaxError(ss.str());
}

void DatalogProgram::test(Token::Types type) {
    if (peek() == type)
        currentIndex_++;
    else
        error();
}

Token::Types DatalogProgram::peek() {
    return currentToken().getTypeId();
}

const Token& DatalogProgram::currentToken() const {
    return tokens_.at(currentIndex_);
}

void DatalogProgram::scheme() {
    predicate(schemes_);
}

void DatalogProgram::schemeList() {
    if (peek() == Token::Types::ID) {
        scheme();
        schemeList();
    }
}

void DatalogProgram::fact() {
    predicate(facts_);
    test(Token::Types::PERIOD);
}

void DatalogProgram::factList() {
    if (peek() == Token::Types::ID) {
        fact();
        factList();
    }
}

void DatalogProgram::rule() {
    Rule rule(predicate());
    test(Token::Types::COLON_DASH);
    std::vector<Predicate>& predicates = rule.getPredicates();
    predicate(predicates);
    predicateList(predicates);
    test(Token::Types::PERIOD);

    rules_.push_back(rule);
}

void DatalogProgram::ruleList() {
    if (peek() == Token::Types::ID) {
        rule();
        ruleList();
    }
}

void DatalogProgram::query() {
    predicate(queries_);
    test(Token::Types::Q_MARK);
}

void DatalogProgram::queryList() {
    if (peek() == Token::Types::ID) {
        query();
        queryList();
    }
}

//this is used only for the rule head
Predicate DatalogProgram::predicate() {
    Predicate pred(currentToken().get());

    test(Token::Types::ID);
    test(Token::Types::LEFT_PAREN);
    parameter(pred);
    parameterList(pred);
    test(Token::Types::RIGHT_PAREN);
    
    return pred;
}

void DatalogProgram::predicate(std::vector<Predicate>& predicates) {
    Predicate pred(currentToken().get());

    test(Token::Types::ID);
    test(Token::Types::LEFT_PAREN);
    parameter(pred);
    parameterList(pred);
    test(Token::Types::RIGHT_PAREN);
    
    predicates.push_back(pred);
}

void DatalogProgram::predicateList(std::vector<Predicate>& predicates) {
    if (peek() == Token::Types::COMMA) {
        currentIndex_++;
        predicate(predicates);
        predicateList(predicates);
    }
}

void DatalogProgram::parameter(Predicate& pred) {
    switch (currentToken().getTypeId()) {
        case Token::Types::STRING:
            pred.addParameter(Parameter(true, currentToken().get()));
            domain_.insert(currentToken().get());
            currentIndex_++;
            break;
        case Token::Types::ID:
            pred.addParameter(Parameter(false, currentToken().get()));
            currentIndex_++;
            break;
        default: //syntax error, an exception must be thrown
            error();
    }
}

void DatalogProgram::parameterList(Predicate& pred) {
    if (peek() == Token::Types::COMMA) {
        currentIndex_++;
        parameter(pred);
        parameterList(pred);
    }
}

void DatalogProgram::clearVectors() {
    schemes_.clear();
    facts_.clear();
    rules_.clear();
    queries_.clear();
    domain_.clear();
}