//
// Created by user on 9/17/20.
//

#ifndef DATA_STRUCTURES_STACK_H
#define DATA_STRUCTURES_STACK_H

#include <memory>

template <class T>
class Stack {
private:
    struct Node{
        T Data;
        Node *NextNode = nullptr;
    };
    int _length = 0;
    Node *Top = nullptr;

public:
    int GetLength() {
        return _length;
    }

    ~Stack(){
        while (GetLength() > 0) {
            Pop();
        }
    }

    void Push(T newData){
        if (_length <= 0) {
            Top = new Node {
                .Data = newData
            };
        }
        else {
            Node* newTopNode = new Node {
                .Data = newData,
                .NextNode = Top
            };
            Top = newTopNode;
        }
        _length++;
    }

    T Pop(){
        Node *nodeForDeleting = Top;
        if (nodeForDeleting == nullptr) {
            throw std::out_of_range("Unsuccessful attempt to pop an element from an empty stack");
        } else {
            T result = Top -> Data;
            Top = nodeForDeleting -> NextNode;
            _length--;
            delete nodeForDeleting;
            return result;
        }
    }

    T Peek() {
        return Top -> Data;
    }

    T* ToArray() {
        if (_length > 0) {
            T *result = new T[GetLength()];
            Node *currNode = Top;
            for (int i = 0; i < _length; ++i) {
                result[i] = currNode->Data;
                currNode = currNode->NextNode;
            }
            return result;
        } else {
            return nullptr;
        }
    }
};
#endif //DATA_STRUCTURES_STACK_H
