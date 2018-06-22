#ifndef __EROBOT_BLOCKING_QUEUE_H__
#define __EROBOT_BLOCKING_QUEUE_H__

#pragma once

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <deque>
#include <utility>

template <typename T>
class ERobotBlockingQueue
{
private:
	boost::mutex              d_mutex;
	boost::condition_variable d_condition;
    std::deque<T>             d_queue;
public:
    void push(T const& value) {
        {
        	boost::mutex::scoped_lock lock(d_mutex);
            d_queue.push_front(value);
        }
        this->d_condition.notify_one();
    }
    T pop() {
        boost::mutex::scoped_lock lock(d_mutex);
        this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
        T rc(std::move(this->d_queue.back()));
        this->d_queue.pop_back();
        return rc;
    }
//    bool tryPop (T & v, chrono::milliseconds dur) {
//    	boost::mutex::scoped_lock lock(d_mutex);
//        if (!this->d_condition.wait_for(lock, dur, [=]{ return !this->d_queue.empty(); })) {
//            return false;
//        }
//        v = std::move (this->d_queue.back());
//        this->d_queue.pop_back();
//        return true;
//    }
    T back() {
    	return d_queue.front();
    }

    void setBack(T const& value) {
    	d_queue.front() = value;
    }

    int size() {
        return d_queue.size();
    }

    void clear() {
    	d_queue.clear();
    }
};


#endif /* __EROBOT_BLOCKING_QUEUE_H__ */
