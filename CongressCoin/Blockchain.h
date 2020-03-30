#pragma once

#include <vector>
#include <memory>
#include "TransactionData.h"
#include "Block.h"
#include <thread>
#include <mutex>
#include <map>

class Miner;

// Blockchain - will contain all blocks
class Blockchain
{
public:
    Blockchain(const std::string& name);
    ~Blockchain();

    void addBlock(const TransactionData& data);
    bool isChainValid() const;
    void printChain() const;
    void addMiners(std::vector<std::unique_ptr<Miner>>& miners);
    void doMining();
    void stopMining(size_t timeoutMs = 0);
    size_t getSize() const;
    std::string getName() const;

    // dangerous! allows direct memory access, but used to showcase
    // what would happen in case a block was actually hacked
    Block* getLatestBlock();
private:
    Block createGenesisBlock();

    std::vector<Block> m_chain;
    std::string m_name;
    std::thread m_miningThread;
    std::mutex m_mtxMiners;
    std::vector<std::unique_ptr<Miner>> m_miners;
    mutable std::mutex m_mtxChain;
    std::map<std::string, size_t> m_senderBlocks;
};