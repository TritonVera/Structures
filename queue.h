#ifndef __QUEUE_H
#define __QUEUE_H

#include <cstdint>
#include "queueNode.h"

template<class QueueType>
class Queue {
public:
    Queue(uint32_t inMaxCount = 0xFFFFFFFF);
    ~Queue();
    void add(QueueType data);
    QueueType remove();
    QueueType first();
    QueueType last();
    uint32_t size();
    bool empty();
    bool full();
    void clear();
    bool contains(QueueType data);

private:
    QueueNode<QueueType>* firstNode;
    QueueNode<QueueType>* lastNode;
    uint32_t count;
    uint32_t maxCount;
};

template<class QueueType> Queue<QueueType>::Queue(uint32_t inMaxCount) {
    firstNode = nullptr;
    lastNode = nullptr;
    count = 0;
    maxCount = inMaxCount;
}

template<class QueueType> Queue<QueueType>::~Queue() {
    while (firstNode != nullptr) remove();
    lastNode = nullptr;
}

template<class QueueType> void Queue<QueueType>::add(QueueType data) {
    if (count == maxCount) return;
    QueueNode<QueueType>* node = new QueueNode<QueueType>(data);
    QueueNode<QueueType>* tempNode = lastNode;
    lastNode = node;
    if (count == 0) firstNode = lastNode;
    else tempNode->next() = lastNode;
    count++;
}

template<class QueueType> QueueType Queue<QueueType>::remove() {
    if (count == 0) return 0;
    QueueType out = firstNode->data();
    QueueNode<QueueType>* tempNode = firstNode;
    firstNode = firstNode->next();
    delete tempNode;
    count--;
    return out;
}

template<class QueueType> QueueType Queue<QueueType>::first() {
    if (count == 0) return 0;
    else return firstNode->data();
}

template<class QueueType> QueueType Queue<QueueType>::last() {
    if (count == 0) return 0;
    else return lastNode->data();
}

template<class QueueType> uint32_t Queue<QueueType>::size() {
    return count;
}

template<class QueueType> bool Queue<QueueType>::empty() {
    return count == 0;
}

template<class QueueType> bool Queue<QueueType>::full() {
    return count == maxCount;
}

template<class QueueType> void Queue<QueueType>::clear() {
    while (firstNode != nullptr) remove();
    lastNode = nullptr;
}

template<class QueueType> bool Queue<QueueType>::contains(QueueType data) {
    QueueNode<QueueType>* current = firstNode;
    while (current != nullptr) {
        if (current->data() == data) return true;
        current = current->next();
    }
    return false;
}

#endif  // __QUEUE_H