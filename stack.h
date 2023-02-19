#ifndef __STACK_H
#define __STACK_H

#include <cstdint>
#include "stackNode.h"

template<class KeyType>
class Stack {
public:
    Stack(uint32_t inMaxCount = 0xFFFFFFFF);
    ~Stack();
    void add(KeyType data);
    KeyType remove();
    KeyType first();
    KeyType last();
    uint32_t size();
    bool empty();
    bool full();
    void clear();
    bool contains(KeyType data);

private:
    StackNode<KeyType>* firstNode;
    StackNode<KeyType>* lastNode;
    uint32_t count;
    uint32_t maxCount;
};

template<class KeyType> Stack<KeyType>::Stack(uint32_t inMaxCount) {
    firstNode = nullptr;
    lastNode = nullptr;
    count = 0;
    maxCount = inMaxCount;
}

template<class KeyType> Stack<KeyType>::~Stack() {
    while (firstNode != nullptr) remove();
    lastNode = nullptr;
}

template<class KeyType> void Stack<KeyType>::add(KeyType data) {
    if (count == maxCount) return;
    StackNode<KeyType>* tempNode = lastNode;
    StackNode<KeyType>* node = new StackNode<KeyType>(data, tempNode);
    lastNode = node;
    if (count == 0) firstNode = lastNode;
    count++;
}

template<class KeyType> KeyType Stack<KeyType>::remove() {
    if (count == 0) return 0;
    StackNode<KeyType>* tempNode = lastNode;
    KeyType out = tempNode->data();
    lastNode = tempNode->prev();
    delete tempNode;
    count--;
    return out;
}

template<class KeyType> KeyType Stack<KeyType>::first() {
    if (count == 0) return KeyType();
    else return firstNode->data();
}

template<class KeyType> KeyType Stack<KeyType>::last() {
    if (count == 0) return KeyType();
    else return lastNode->data();
}

template<class KeyType> uint32_t Stack<KeyType>::size() {
    return count;
}

template<class KeyType> bool Stack<KeyType>::empty() {
    return count == 0;
}

template<class KeyType> bool Stack<KeyType>::full() {
    return count == maxCount;
}

template<class KeyType> void Stack<KeyType>::clear() {
    while (firstNode != nullptr) remove();
    lastNode = nullptr;
}

template<class KeyType> bool Stack<KeyType>::contains(KeyType data) {
    StackNode<KeyType>* current = lastNode;
    while (current != nullptr) {
        if (current->data() == data) return true;
        current = current->prev();
    }
    return false;
}

#endif  // __STACK_H