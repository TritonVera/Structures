#ifndef __STACK_NODE_H
#define __STACK_NODE_H

template<class NodeType>
class StackNode {
public:
    StackNode(NodeType data, StackNode<NodeType>* prev);
    NodeType& data();
    StackNode<NodeType>* prev();

private:
    NodeType innerData;
    StackNode<NodeType>* prevNode;
};

template<class NodeType> StackNode<NodeType>::StackNode(NodeType data, StackNode<NodeType>* prev) {
    innerData = data;
    prevNode = prev;
}

template<class NodeType> NodeType& StackNode<NodeType>::data() {
    return innerData;
}

template<class NodeType> StackNode<NodeType>* StackNode<NodeType>::prev() {
    return prevNode;
}

#endif  // __STACK_NODE_H