/**
 * @file circular_buffer.cpp
 * @author Arnaud Kalthoff
 * @brief https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "application/circular_buffer.h"

explicit circular_buffer::circular_buffer(size_t size) : buf_(std::unique_ptr<T[]>(new T[size])),
                                                         max_size_(size)
{
    // empty constructor
}

void circular_buffer::reset()
{
    std::lock_guard<std::mutex> lock(mutex_);
    head_ = tail_;
    full_ = false;
}

bool circular_buffer::empty() const
{
    // if head and tail are equal, we are empty
    return (!full_ && (head_ == tail_));
}

bool circular_buffer::full() const
{
    // If tail is ahead the head by 1, we are full
    return full_;
}

size_t circular_buffer::capacity() const
{
    return max_size_;
}

size_t circular_buffer::size() const
{
    size_t size = max_size_;

    if (!full_)
    {
        if (head_ >= tail_)
        {
            size = head_ - tail_;
        }
        else
        {
            size = max_size_ + head_ - tail_;
        }
    }

    return size;
}

void circular_buffer::put(T item)
{
    std::lock_guard<std::mutex> lock(mutex_);

    buf_[head_] = item;

    if (full_)
    {
        tail_ = (tail_ + 1) % max_size_;
    }

    head_ = (head_ + 1) % max_size_;

    full_ = head_ == tail_;
}

T circular_buffer::get()
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (empty())
    {
        return T();
    }

    // Read data and advance the tail (we now have a free space)
    auto val = buf_[tail_];
    full_ = false;
    tail_ = (tail_ + 1) % max_size_;

    return val;
}