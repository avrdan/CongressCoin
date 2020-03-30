#include "Miner.h"
#include "Blockchain.h"

MessageQueue<TransactionData> Miner::s_queue;

Miner::Miner(Blockchain* chain, size_t nodes, const std::string& name)
    : m_chain(chain)
    , m_nrNodes(nodes)
    , m_name(name)
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

    m_minerThread = std::thread(&Miner::waitForData, this);
}

void Miner::waitForData()
{
    while (!m_isDone)
    {
        TransactionData data = s_queue.receive();
        if (data.isValid())
        {
            m_chain->addBlock(data);
        }
        else
        {
            std::cout << "Invalid block.." << std::endl;
        }
    }
}

void Miner::stop()
{
    m_isDone = true;
    // send empty message which returns it from receive and terminates the call, thus closing queue after it becomes empty
    s_queue.send({});
    while (!s_queue.isEmpty());
    m_minerThread.join();
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
        size_t difficulty = m_chain->getSize();
        if (m_workJobsDone >= difficulty)// std::pow(2, difficulty))
        {
            // send transaction data that will be used to create a block
            double amount = (double)rand() / RAND_MAX * AMOUNT_FACTOR * difficulty;
            std::string keySender = m_name;
            std::string keyReceiver = m_chain->getName();
            s_queue.send({ amount, keySender, keyReceiver, time(nullptr) });
            m_workJobsDone = 0;
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

