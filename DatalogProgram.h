#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include "Rule.h"
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "SyntaxError.h"

class DatalogProgram {
public:
    DatalogProgram(const std::vector<Token>& tokens);
    virtual ~DatalogProgram();
    
    void parse();

    const std::vector<Predicate>& getSchemes() const;
    const std::vector<Predicate>& getFacts() const;
    const std::vector<Rule>& getRules() const;
    const std::vector<Predicate>& getQueries() const;
    const std::set<std::string>& getDomain() const;
    
    std::string toString() const;

private:
    //tests if the current token matches the requested type, if it fails
    //an exception is thrown
    void test(Token::Types type);
    
    //peeks the next token without advancing the vector's current pointer 
    //and returns its type
    Token::Types peek();
    
    //returns the token from the tokens vector located at currentIndex_ position
    const Token& currentToken() const;

    //parsers
    void scheme();
    void schemeList();
    void fact();
    void factList();
    void rule();
    void ruleList();
    void query();
    void queryList();
    
    //parsers
    Predicate predicate();
    void predicate(std::vector<Predicate>& predicates);
    void predicateList(std::vector<Predicate>& predicates);
    void parameter(Predicate& pPredicate);
    void parameterList(Predicate& pPredicate);
    
    void error();

    //when this object is destroyed, this function destroys all objects stored
    //in the vector classes
    void clearVectors();
    
    //the tokens we get from our scanner
    const std::vector<Token>& tokens_;
    
    //this variable will be our pointer inside the token vector
    unsigned int currentIndex_;
    
    //the lists where all elements will be stored
    std::vector<Predicate> schemes_;
    std::vector<Predicate> facts_;
    std::vector<Rule> rules_;
    std::vector<Predicate> queries_;
    std::set<std::string> domain_;
};

#endif /* DATALOGPROGRAM_H */

