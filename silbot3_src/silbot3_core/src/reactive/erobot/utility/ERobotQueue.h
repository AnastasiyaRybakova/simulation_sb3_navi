#ifndef __EROBOT_QUEUE_H__
#define __EROBOT_QUEUE_H__

#pragma once

#include <deque>
#include <utility>

template <typename T>
class ERobotQueue
{
private:
    std::deque<T>             d_queue;
public:
    void push(T const& value) {
        {
            d_queue.push_front(value);
        }
    }
    T pop() {
        T rc(std::move(this->d_queue.back()));
        this->d_queue.pop_back();
        return rc;
    }
    int size() {
        return d_queue.size();
    }
    void clear() {
    	d_queue.clear();
    }
};


#endif /* __EROBOT_QUEUE_H__ */
