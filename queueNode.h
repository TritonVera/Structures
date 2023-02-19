#ifndef __QUEUE_NODE_H
#define __QUEUE_NODE_H

template<class NodeType>
class QueueNode {
public:
    QueueNode(NodeType data);
    NodeType& data();
    QueueNode<NodeType>*& next();

private:
    NodeType innerData;
    QueueNode<NodeType>* nextNode;
};

template<class NodeType> QueueNode<NodeType>::QueueNode(NodeType data) {
    innerData = data;
    nextNode = nullptr;
}

template<class NodeType> NodeType& QueueNode<NodeType>::data() {
    return innerData;
}

template<class NodeType> QueueNode<NodeType>*& QueueNode<NodeType>::next() {
    return nextNode;
}

#endif  // __QUEUE_NODE_H