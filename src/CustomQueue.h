#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H

#include <iostream>

struct Device; 

struct QueueNode {
    Device* data;
    QueueNode* next;
};

class CustomQueue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;
public:
    CustomQueue() : frontNode(nullptr), rearNode(nullptr) {}

    bool isEmpty() {
        return frontNode == nullptr;
    }

    void enqueue(Device* d) {
        QueueNode* newNode = new QueueNode{d, nullptr};
        if (rearNode) {
            rearNode->next = newNode;
            rearNode = newNode;
        } else {
            frontNode = rearNode = newNode;
        }
    }

    Device* dequeue() {
        if (!frontNode) return nullptr;
        QueueNode* temp = frontNode;
        Device* data = temp->data;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete temp;
        return data;
    }
    
    ~CustomQueue() {
        while (frontNode) dequeue();
    }
};

#endif