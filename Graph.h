#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "Node.h"
#include "Rule.h"

class Graph {
public:
    Graph(const std::vector<Rule>& rules);
    virtual ~Graph();
    
    void buildDependencyGraphs();
    void dfs();
    void sccdfs();
    std::vector<int> sortByPostOrderNumber() const;
    const std::vector<std::set<int>>& getSCCs() const;
    const std::string toString() const;

private:
    int dfsFromNode(Node& node, int postOrderNumber);
    void sccdfsFromNode(int index, std::set<int>& scc);
    const std::string graphToString(const std::vector<Node>& graph, const std::string& title) const;
    const std::string postOrderNumbersToString() const;
    const std::string SCCSearchOrderToString() const;
    const std::string SCCEvaluationToString() const;
    
    const std::vector<Rule>& rules_;
    std::vector<Node> dependencyGraph_;
    std::vector<Node> revDependencyGraph_;
    std::vector<std::set<int>> stronglyConnectedComponents_;
};

#endif /* GRAPH_H */

