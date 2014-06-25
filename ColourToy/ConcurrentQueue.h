#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>

template<typename T>
class ConcurrentQueue
{
public:
    ConcurrentQueue(unsigned size)
    {
        m_MaxSize = size;
    }

    void push(const T& item)
    {
#ifdef LOG
        std::cout << "Push on " << std::this_thread::get_id() << std::endl;
#endif
        std::unique_lock<std::mutex> mlock(m_Mutex);
        while (m_Queue.size() >= m_MaxSize)
        {
#ifdef LOG
            std::cout << "Waiting on pop" << std::endl;
#endif
            m_PopCond.wait(mlock);
        }
        m_Queue.push(item);
        mlock.unlock();
        m_PushCond.notify_one();
    }

    void push(T&& item)
    {
#ifdef LOG
        std::cout << "Push on " << std::this_thread::get_id() << std::endl;
#endif
        std::unique_lock<std::mutex> mlock(m_Mutex);
        while (m_Queue.size() >= m_MaxSize)
        {
#ifdef LOG
            std::cout << "Waiting on pop" << std::endl;
#endif
            m_PopCond.wait(mlock);
        }
        m_Queue.push(std::move(item));
        mlock.unlock();

        m_PushCond.notify_one();
    }

    T pop()
    {
#ifdef LOG
        std::cout << "Pop on " << std::this_thread::get_id() << std::endl;
#endif
        std::unique_lock<std::mutex> mlock(m_Mutex);
        while (m_Queue.empty())
        {
#ifdef LOG
            std::cout << "Waiting on push" << std::endl;
#endif
            m_PushCond.wait(mlock);
        }
        auto item = m_Queue.front();
        m_Queue.pop();
        m_PopCond.notify_one();
        return item;
    }

    bool empty()
    {
        std::unique_lock < std::mutex > mlock(m_Mutex);
        return m_Queue.empty();
    }

    ConcurrentQueue(const ConcurrentQueue&) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;

private:
    unsigned m_MaxSize;
    std::queue<T> m_Queue;
    std::mutex m_Mutex;
    std::condition_variable m_PushCond;
    std::condition_variable m_PopCond;
};

