#include "Blockchain.h"
#include "Miner.h"
#include <future>

// Congress coin
// Add an svg inside each miner, have a Republican miner and a Democraft miner, and a libertarian miner

Blockchain::Blockchain(const std::string& name)
    : m_name(name)
{
    m_chain.emplace_back(std::move(createGenesisBlock()));
}

void Blockchain::addBlock(const TransactionData& data)
{
    std::cout << "Adding 1 block.." << std::endl;
    size_t index = m_chain.size();
    m_chain.emplace_back(static_cast<int>(index), data, index > 0 ? getLatestBlock()->getHash() : 0);
}

bool Blockchain::isChainValid() const
{
    const Block* prevBlock = nullptr;
    for (const auto& block : m_chain)
    {
        if (!block.isHashValid())
            return false;

        if (prevBlock)
        {
            if (block.getPrevHash() != prevBlock->getHash())
            {
                return false;
            }
        }
        prevBlock = &block;
    }

    return true;
}

void Blockchain::printChain() const
{
    std::cout << std::endl << std::endl << "================= PRINTING BLOCKCHAIN: " << m_name << " =================" << std::endl;
    for (const auto& block : m_chain)
    {
        std::cout << std::endl << "Block =================" << std::endl;
        std::cout << "Index: " << block.getIndex() << std::endl;
        std::cout << "Hash: " << block.getHash() << std::endl;
        std::cout << "Amount: " << block.getData().amount << std::endl;
        std::cout << "Sender: " << block.getData().keySender << std::endl;
        std::cout << "Receiver: " << block.getData().keyReceiver << std::endl;
        std::cout << "Timestamp: " << block.getData().timestamp << std::endl;
        std::cout << "Previous Hash: " << block.getPrevHash() << std::endl;
        std::cout << "Block Valid? " << block.isHashValid() << std::endl;
    }
}

void Blockchain::addMiners(std::vector<std::unique_ptr<Miner>>& miners)
{
    for (auto& miner : miners)
    {
        m_miners.emplace_back(std::move(miner));
    }
}

Block* Blockchain::getLatestBlock()
{
    return &m_chain.back();
}

Block Blockchain::createGenesisBlock()
{
    return { 0, { 0, "Genesis", "Genesis", time(nullptr) }, 0 };
}

void Blockchain::doMining()
{

    m_miningThread = std::thread([this]() {

        for (auto& miner : m_miners)
        {
            miner->simulate();
        }
        std::vector<std::future<TransactionData>> futures;
        while (true)
        {
            // receive blocks from message queue
            // TODO: pass difficulty to waitForData??
            for (auto& miner : m_miners)
            {
                std::future<TransactionData> ftr = std::async(std::launch::async, &Miner::waitForData, miner.get());
                futures.emplace_back(std::move(ftr));
            }

            for (auto& f : futures)
            {
                // grow chain by calling addBlock if we have valid data
                TransactionData data = f.get();
                if (data.isValid())
                {
                    addBlock(data);
                }
                else
                {
                    std::cout << "Invalid block.." << std::endl;
                }
            }
            futures.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::lock_guard<std::mutex> lock(m_mtx);
            if (m_miners.empty())
            {
                break;
            }
        }

        });
}

void Blockchain::stopMining(size_t timeoutMs /*= 0*/)
{
    if (timeoutMs > 0)
    {
      std::chrono::milliseconds workTime = std::chrono::duration<unsigned int, std::milli>(20000);
      std::this_thread::sleep_for(workTime);
    }

    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_miners.clear();
    }
    
    m_miningThread.join();
}

size_t Blockchain::getSize() const
{
    return m_chain.size();
}

