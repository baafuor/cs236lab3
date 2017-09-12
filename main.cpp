#include <iostream>
#include "Scanner.h"
#include "DatalogProgram.h"
#include "DB.h"
#include "IOException.h"
#include "SyntaxError.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Invalid arguments\n";
        return 0;
    }
    
    char* intputFile = argv[1];
    char* outputFile = argv[2];

    std::cout << "Opening Datalog file " << intputFile << "\n";
    try {
        Scanner scanner(intputFile);

        std::cout << "Getting tokens...\n";
        scanner.tokenize();

        DatalogProgram prog(scanner.getTokens());

        std::cout << "Parsing...\n";
        prog.parse();
        
        DB db;
        std::ofstream of;
        
        of.open(outputFile);
        
        std::cout << "Processing schemes\n";
        of << db.processSchemes(prog.getSchemes());
        
        std::cout << "Processing facts\n";
        of << db.processFacts(prog.getFacts());
        
        std::cout << "Evaluating rules\n";
        of << db.optimizeRuleEval(prog.getRules());
        //of << db.evalRules(prog.getRules());

        std::cout << "Evaluating queries\n";
        of << db.evalQueries(prog.getQueries());

        std::cout << "Ready.\n";
        
        of.close();
    } catch (IOException ex) {
        std::cout << ex.message();
    } catch (SyntaxError ex) {
        std::cout << ex.message();
    }

    return 0;
}

