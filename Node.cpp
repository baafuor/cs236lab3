#include "Node.h"

Node::Node() : visited_(false), postOrderNumber_(0) {
}

Node::~Node() {
    adjacentNodes_.clear();
}

const std::set<int>& Node::getAdjacentNodes() const {
    return adjacentNodes_;
}

int Node::getPostOrderNumber() const {
    return postOrderNumber_;
}

void Node::setPostOrderNumber(int postorderNumber) {
    postOrderNumber_ = postorderNumber;
}

bool Node::isVisited() const {
    return visited_;
}

void Node::setVisited(bool visited) {
    visited_ = visited;
}

void Node::addAdjacentNode(int index) {
    adjacentNodes_.insert(index);
}