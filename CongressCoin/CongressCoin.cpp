// MyBlockchain.cpp : Defines the entry point for the application.
//

#include <ctime>
#include <vector>
#include <random>
#include "CongressCoin.h"
#include "Blockchain.h"

const std::string bcName = "CongressCoin";

int main()
{
    // TODO: store svgs inside a binary blobk in transaction data
    // each minor has an svg which they set in their respective transaction data

    // seed the random generator
    std::srand(static_cast<unsigned int>(time(nullptr)));

    Blockchain congressCoin(bcName);

    congressCoin.addBlock({1.5, "Jack", "Sara", time(nullptr)});
    congressCoin.addBlock({ 0.00013322, "Maria", "Frank", time(nullptr) });

    std::cout << "Is chain valid? " << congressCoin.isChainValid() << std::endl;

    // :)
    Block* hackBlock = congressCoin.getLatestBlock();
    char* p = reinterpret_cast<char*>(hackBlock);
    p += 3 * sizeof(size_t);
    TransactionData* data = (reinterpret_cast<TransactionData*>(p));
    data->amount = 10000;

    std::cout << "Is chain STILL valid? " << congressCoin.isChainValid() << std::endl;

    congressCoin.printChain();

    data->amount = 0.00013322; // make the block valid again

    std::vector<std::unique_ptr<Miner>> miners;
    std::unique_ptr<Miner> minerR = std::make_unique<Miner>(congressCoin.getSize(), 7, "RepublicanMiner", bcName);
    std::unique_ptr<Miner> minerD = std::make_unique<Miner>(congressCoin.getSize(), 3, "DemocratMiner", bcName);
    std::unique_ptr<Miner> minerL = std::make_unique<Miner>(congressCoin.getSize(), 50, "LibertarianMiner", bcName);
    miners.emplace_back(std::move(minerR));
    miners.emplace_back(std::move(minerD));
    miners.emplace_back(std::move(minerL));
    congressCoin.addMiners(miners);
    congressCoin.doMining();

    congressCoin.stopMining(120000); // mine blocks for 2 minutes
    congressCoin.printChain();

    return 0;
}
