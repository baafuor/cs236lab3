#include "Graph.h"

Graph::Graph(const std::vector<Rule>& rules) : rules_(rules) {
    const int size = rules.size();
    dependencyGraph_ = std::vector<Node>(size);
    revDependencyGraph_ = std::vector<Node>(size);
    stronglyConnectedComponents_ = std::vector<std::set<int>>(size);
}

Graph::~Graph() {
    dependencyGraph_.clear();
    revDependencyGraph_.clear();
    stronglyConnectedComponents_.clear();
}

void Graph::buildDependencyGraphs() {
    int index1(0);

    for (Rule rule1 : rules_) {
        int index2(0);

        for (Rule rule2 : rules_) {
            for (Predicate predicate : rule2.getPredicates()) {
                if (rule1.getHead().getName() == predicate.getName()) {
                    dependencyGraph_[index2].addAdjacentNode(index1);
                    revDependencyGraph_[index1].addAdjacentNode(index2);
                    break;
                }
            }

            index2++;
        }

        index1++;
    }
}

void Graph::dfs() {
    int postOrderNumber = 1;
    
    for (Node& node : revDependencyGraph_) {
        if (!node.isVisited()) {
            postOrderNumber = dfsFromNode(node, postOrderNumber);
        }
    }
}

int Graph::dfsFromNode(Node& node, int postOrderNumber) {
    node.setVisited(true);
    
    for (int index : node.getAdjacentNodes()) {
        Node& adjNode = revDependencyGraph_[index];
        if (!adjNode.isVisited()) {
            postOrderNumber = dfsFromNode(adjNode, postOrderNumber);
        }
    }

    node.setPostOrderNumber(postOrderNumber);
    return ++postOrderNumber;
}

void Graph::sccdfs() {
    std::vector<int> order = sortByPostOrderNumber();

    for (int index : order) {
        std::set<int> scc;

        if (!dependencyGraph_[index].isVisited()) {
            sccdfsFromNode(index, scc);
        }

        stronglyConnectedComponents_[index] = scc;
    }
}

void Graph::sccdfsFromNode(int index, std::set<int>& scc) {
    Node& node = dependencyGraph_[index];
    
    node.setVisited(true);

    for (int index : node.getAdjacentNodes()) {
        if (!dependencyGraph_[index].isVisited()) {
            sccdfsFromNode(index, scc);
        }
    }

    scc.insert(index);
}

std::vector<int> Graph::sortByPostOrderNumber() const {
    int size = rules_.size();
    std::vector<int> sorted(size);

    for (int i = 0; i < size; i++) {
        sorted[size - revDependencyGraph_[i].getPostOrderNumber()] = i;
    }

    return sorted;
}

const std::vector<std::set<int>>& Graph::getSCCs() const {
    return stronglyConnectedComponents_;
}

const std::string Graph::toString() const {
    std::stringstream ss;

    ss << graphToString(dependencyGraph_, "Dependency Graph");
    ss << graphToString(revDependencyGraph_, "Reverse Graph");
    ss << postOrderNumbersToString();
    ss << SCCSearchOrderToString();

    return ss.str();
}

const std::string Graph::graphToString(const std::vector<Node>& graph, const std::string& title) const {
    std::stringstream ss;

    ss << "\n" << title;

    for (int i = 0; i < graph.size(); i++) {
        ss << "\n  R" << i << ":";
        for (int j : graph[i].getAdjacentNodes()) {
            ss << " R" << j;
        }
    }

    ss << "\n";

    return ss.str();
}

const std::string Graph::postOrderNumbersToString() const {
    std::stringstream ss;

    ss << "\nPostorder Numbers";
    for (int i = 0; i < rules_.size(); i++) {
        ss << "\n  R" << i << ": " << revDependencyGraph_[i].getPostOrderNumber();
    }
    ss << "\n";

    return ss.str();
}

const std::string Graph::SCCSearchOrderToString() const {
    std::stringstream ss;

    ss << "\nSCC Search Order";
    std::vector<int> search_order = sortByPostOrderNumber();
    for (auto index : search_order) {
        ss << "\n  R" << index;
    }
    ss << "\n";

    return ss.str();
}
