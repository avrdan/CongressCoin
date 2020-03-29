#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <deque>
#include "TransactionData.h"

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
    //std::cout << "Adding message at address:" << &msg  << " to queue." << std::endl;
    std::cout << "Adding message to queue." << std::endl;
    m_queue.emplace_back(std::move(msg));
    m_cond.notify_one();
}

class Miner
{
public:
    Miner(size_t difficulty, size_t nodes, const std::string& name, const std::string& blockchainName);
    ~Miner();

    void simulate();
    TransactionData waitForData();
private:
    void stop();
    void doWork();
    void fakeWork();

    size_t m_nrNodes = 1;
    size_t m_difficulty = 1;
    int m_workJobsDone = 0;
    std::vector<std::thread> m_nodes;

    // Hm...how do we get current block number???
    // Have blockchain in miner...OR
    // From blackchain, create miners and send regular updates when the chain changes size
    // or promise/future
    //Blockchain* m_blockchain = nullptr;

    // milliseconds
    const int WORK_TIME_MIN = 500;
    const int WORK_TIME_MAX = 5000;
    const int AMOUNT_FACTOR = 10;

    // need to somehow pass the blockchain

    // create block validator as a concurrent message queue??
    // push back mined blocks and discard invalid ones on receive?

    MessageQueue<TransactionData> m_queue;
    std::string m_name;
    std::string m_blockchainName;
    std::mutex m_mtx;
    bool m_isDone = false;
};