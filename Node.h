#ifndef NODE_H
#define NODE_H

#include <set>

class Node {
public:
    Node();
    virtual ~Node();
    
    void addAdjacentNode(int index);
    const std::set<int>& getAdjacentNodes() const;

    int getPostOrderNumber() const;
    void setPostOrderNumber(int postorderNumber);
    bool isVisited() const;
    void setVisited(bool visited);
    
private:
    bool visited_;
    int postOrderNumber_;
    std::set<int> adjacentNodes_;
};

#endif /* NODE_H */

