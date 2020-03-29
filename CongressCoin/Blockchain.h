#pragma once

#include <vector>
#include <memory>
#include "TransactionData.h"
#include "Block.h"
#include "Miner.h"

// Blockchain - will contain all blocks
class Blockchain
{
public:
    Blockchain(const std::string& name);
    ~Blockchain() = default;

    void addBlock(const TransactionData& data);
    bool isChainValid() const;
    void printChain() const;
    void addMiners(std::vector<std::unique_ptr<Miner>>& miners);
    void doMining();
    void stopMining(size_t timeoutMs = 0);
    size_t getSize() const;

    // dangerous! allows direct memory access, but used to showcase
    // what would happen in case a block was actually hacked
    Block* getLatestBlock();
private:
    Block createGenesisBlock();

    std::vector<Block> m_chain;
    std::string m_name;
    std::vector<std::unique_ptr<Miner>> m_miners;
    std::thread m_miningThread;
    std::mutex m_mtx;
};