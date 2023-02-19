#ifndef __DICTIONARY_H
#define __DICTIONARY_H

#include <cmath>
#include <stdexcept>

template<typename KeyType, typename DataType>
class DictionaryNode {
public:
    DataType data;
    KeyType key;
    int height;
    DictionaryNode* left;
    DictionaryNode* right;

    DictionaryNode(KeyType inKey, DataType inData = DataType()) {
        key = inKey;
        data = inData;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

template<class KeyType, class DataType>
class Dictionary {
private:
    DictionaryNode<KeyType, DataType>* root = nullptr;
    size_t dictSize;

public:
    void insert(KeyType inKey, DataType inData = 0) {
        root = insertUtil(root, inKey, inData);
    }

    void remove(KeyType inKey) {
        root = removeUtil(root, inKey);
    }

    DataType& at(KeyType inKey) {
        auto findNode = search(inKey);
        return (findNode ? findNode->data : throw(std::out_of_range("Can't find element")));
    }

    DataType& operator[](KeyType inKey) {
        auto findNode = search(inKey);
        if (findNode) return findNode->data;
        insert(inKey);
        return search(inKey)->data;
    }

    size_t size() const {
        return dictSize;
    }

    bool empty() const {
        return (dictSize == 0);
    }

private:
    int height(DictionaryNode<KeyType, DataType>* head) const {
        return (head ? head->height : 0);
    }

    DictionaryNode<KeyType, DataType>* search(KeyType inKey) const {
        return searchUtil(root, inKey);
    }

    DictionaryNode<KeyType, DataType>* rightRotation(DictionaryNode<KeyType, DataType>* headNode) {
        auto newHead = headNode->left;
        headNode->left = newHead->right;
        newHead->right = headNode;
        headNode->height = 1 + std::max(height(headNode->left), height(headNode->right));
        newHead->height = 1 + std::max(height(newHead->left), height(newHead->right));
        return newHead;
    }

    DictionaryNode<KeyType, DataType>* leftRotation(DictionaryNode<KeyType, DataType>* headNode) {
        auto newHead = headNode->right;
        headNode->right = newHead->left;
        newHead->left = headNode;
        headNode->height = 1 + std::max(height(headNode->left), height(headNode->right));
        newHead->height = 1 + std::max(height(newHead->left), height(newHead->right));
        return newHead;
    }

    DictionaryNode<KeyType, DataType>* insertUtil(DictionaryNode<KeyType, DataType>* headNode, KeyType inKey, DataType inData) {
        // Добавление элемента
        if (headNode == nullptr) {
            dictSize += 1;
            DictionaryNode<KeyType, DataType>* temp = new DictionaryNode<KeyType, DataType>(inKey, inData);
            return temp;
        }
        if (inKey < headNode->key)
            headNode->left = insertUtil(headNode->left, inKey, inData);
        else if (inKey > headNode->key)
            headNode->right = insertUtil(headNode->right, inKey, inData);
        headNode->height = 1 + std::max(height(headNode->left), height(headNode->right));

        // Балансировка дерева
        auto balanceFactor = height(headNode->left) - height(headNode->right);
        if (balanceFactor > 1) {
            if (inKey < headNode->left->key) {
                return rightRotation(headNode);
            }
            else {
                headNode->left = leftRotation(headNode->left);
                return rightRotation(headNode);
            }
        }
        else if (balanceFactor < -1) {
            if (inKey > headNode->right->key) {
                return leftRotation(headNode);
            }
            else {
                headNode->right = rightRotation(headNode->right);
                return leftRotation(headNode);
            }
        }
        return headNode;
    }

    DictionaryNode<KeyType, DataType>* removeUtil(DictionaryNode<KeyType, DataType>* headNode, KeyType inKey) {
        // Удаление элемента
        if (headNode == nullptr) return nullptr;
        if (inKey < headNode->key) {
            headNode->left = removeUtil(headNode->left, inKey);
        }
        else if (inKey > headNode->key) {
            headNode->right = removeUtil(headNode->right, inKey);
        }
        else {
            auto rightNode = headNode->right;
            if (headNode->right == nullptr) {
                auto leftNode = headNode->left;
                delete(headNode);
                headNode = leftNode;
            }
            else if (headNode->left == nullptr) {
                delete(headNode);
                headNode = rightNode;
            }
            else {
                while (rightNode->left) rightNode = rightNode->left;
                headNode->key = rightNode->key;
                headNode->right = removeUtil(headNode->right, rightNode->key);
            }
        }
        if (headNode == nullptr) return headNode;
        headNode->height = 1 + std::max(height(headNode->left), height(headNode->right));

        // Балансировка дерева
        auto balanceFactor = height(headNode->left) - height(headNode->right);
        if (balanceFactor > 1) {
            if (height(headNode->left) >= height(headNode->right)) {
                return rightRotation(headNode);
            }
            else {
                headNode->left = leftRotation(headNode->left);
                return rightRotation(headNode);
            }
        }
        else if (balanceFactor < -1) {
            if (height(headNode->right) >= height(headNode->left)) {
                return leftRotation(headNode);
            }
            else {
                headNode->right = rightRotation(headNode->right);
                return leftRotation(headNode);
            }
        }
        return headNode;
    }

    DictionaryNode<KeyType, DataType>* searchUtil(DictionaryNode<KeyType, DataType>* headNode, KeyType inKey) const {
        if (headNode == nullptr)
            return nullptr;
        auto headNodeKey = headNode->key;
        if (headNodeKey == inKey)
            return headNode;
        else if (headNodeKey > inKey)
            return searchUtil(headNode->left, inKey);
        else    // if (headNodeKey < inKey)
            return searchUtil(headNode->right, inKey);
    }
};

#endif  // __DICTIONARY_H
