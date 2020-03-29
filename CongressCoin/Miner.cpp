#include "Miner.h"

Miner::Miner(size_t difficulty, size_t nodes, const std::string& name, const std::string& blockchainName)
    : m_nrNodes(nodes)
    , m_difficulty(difficulty)
    , m_name(name)
    , m_blockchainName(blockchainName)
{
}

Miner::~Miner()
{
    stop();
}

void Miner::simulate()
{
    for (int i = 0; i < m_nrNodes; ++i)
    {
        m_nodes.emplace_back(std::thread(&Miner::doWork, this));
    }
}

TransactionData Miner::waitForData()
{
    return m_queue.receive();
}

void Miner::stop()
{
    m_isDone = true;
    // send empty message which returns it from receive and terminates the call, thus closing queue after it becomes empty
    m_queue.send({});
    while (!m_queue.isEmpty());

    // queue closed, safe to destroy after all mining threads are finished
    for (auto& node : m_nodes)
    {
        node.join();
    }
}

void Miner::doWork()
{
    while (!m_isDone)
    {
        // add work job
        fakeWork();

        // when threshold reached, add block to blockchain
        std::lock_guard<std::mutex> lock(m_mtx);
        if (m_workJobsDone >= std::pow(2, m_difficulty))
        {
            // send transaction data that will be used to create a block
            double amount = (double)rand() / RAND_MAX * AMOUNT_FACTOR * m_difficulty;
            std::string keySender = m_name;
            std::string keyReceiver = m_blockchainName;
            m_queue.send({ amount, keySender, keyReceiver, time(nullptr) });
            m_workJobsDone = 0;
            ++m_difficulty;
        }
    }
}

void Miner::fakeWork()
{
    std::chrono::milliseconds workTime = std::chrono::duration<unsigned int, std::milli>(rand() % WORK_TIME_MAX + WORK_TIME_MIN);
    std::this_thread::sleep_for(workTime);
    std::lock_guard<std::mutex> lock(m_mtx);
    ++m_workJobsDone;
}

