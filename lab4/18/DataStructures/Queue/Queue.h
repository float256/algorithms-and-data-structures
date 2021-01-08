//
// Created by user on 9/20/20.
//

#ifndef DATA_STRUCTURES_QUEUE_H
#define DATA_STRUCTURES_QUEUE_H

#include <stdexcept>

template <class T>
class Queue {

private:
    struct Node{
        T Data;
        Node *NextNode = nullptr;
    };
    int _length = 0;
    Node *Top = nullptr;
    Node *Bottom = nullptr;
    bool IsDeleteQueueEntities;

public:
    Queue(bool isDeleteQueueEntities = true) {
        IsDeleteQueueEntities = isDeleteQueueEntities;
    }

    int GetLength() {
        return _length;
    }

    ~Queue(){
        while (GetLength() > 0) {
            Dequeue();
        }
    }

    void Enqueue(T newData) {
        Node* newQueueNode = new Node{};
        newQueueNode->Data = newData;
        if (Bottom == nullptr) {
            Top = Bottom = newQueueNode;
        } else {
            Bottom -> NextNode = newQueueNode;
            Bottom = Bottom -> NextNode;
        }
        _length++;
    }

    T Peek() {
        return Top -> Data;
    }

    T Dequeue() {
        Node *nodeForDeleting = Top;
        if (nodeForDeleting != nullptr) {
            T result = Top -> Data;
            Top = nodeForDeleting -> NextNode;
            _length--;
            if (IsDeleteQueueEntities) {
                delete nodeForDeleting;
            }
            return result;
        } else {
            throw std::out_of_range("Unsuccessful attempt to dequeue from empty queue");
        }
    }

    T* ToArray() {
        if (_length > 0) {
            T *result = new T[_length];
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


#endif //DATA_STRUCTURES_QUEUE_H
