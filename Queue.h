#ifndef QUEUE_H
#define QUEUE_H

#include "stdafx.h"
#include "LinkedList.h"

template<typename T>
class Queue {
    private: 
        LinkedList<T> queue;
    public:
        Queue() = default;

    void push(const T& value) {
        queue.push_back(value);
    }

    void pop() {
        if (empty()) throw std::runtime_error("queue is empty");
        queue.pop_front();
    }

    T& front() {
        if (empty()) throw std::runtime_error("queue is empty");
        return queue.front();
    }

    const T& front() const {
        if (empty()) throw std::runtime_error("queue is empty");
        return queue.front();
    }

    bool empty() const { return queue.empty(); }
    size_t size() const { return queue.get_size(); }

    void clear() {
        queue.clear();
    }
};

#endif