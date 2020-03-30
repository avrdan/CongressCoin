#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <deque>
#include "TransactionData.h"

class Blockchain;

// maybe use some block instead of T and have something like Block and DataBlock/InfoBlock

template<typename T>
class MessageQueue
{
public:
    T receive();
    void send(T&& msg);
    bool isEmpty() { return m_queue.empty(); }
private:
    std::deque<T> m_queue;
    std::mutex m_mtx;
    std::condition_variable m_cond;
};

template<typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cond.wait(lock, [this]() { return !m_queue.empty(); });
    
    T msg = std::move(m_queue.front());
    m_queue.pop_front();
    return msg;
}

template<typename T>
void MessageQueue<T>::send(T&& msg)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    //std::cout << "Adding message to queue." << std::endl;
    m_queue.emplace_back(std::move(msg));
    m_cond.notify_one();
}

class Miner
{
public:
    Miner(Blockchain* chain, size_t nodes, const std::string& name);
    ~Miner();

    void simulate();
    void waitForData();
private:
    void stop();
    void doWork();
    void fakeWork();

    size_t m_nrNodes = 1;
    int m_workJobsDone = 0;
    std::vector<std::thread> m_nodes;
    std::thread m_minerThread;

    // milliseconds
    const int WORK_TIME_MIN = 1;
    const int WORK_TIME_MAX = 5;
    const int AMOUNT_FACTOR = 10;

    // need to somehow pass the blockchain
    Blockchain* m_chain = nullptr;

    static MessageQueue<TransactionData> s_queue;
    std::string m_name;
    std::string m_blockchainName;
    std::mutex m_mtx;
    bool m_isDone = false;
};