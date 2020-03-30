// MyBlockchain.cpp : Defines the entry point for the application.
//

#include <ctime>
#include <vector>
#include <random>
#include "CongressCoin.h"
#include "Blockchain.h"
#include "Miner.h"

const std::string bcName = "CongressCoin";

int main()
{
    // TODO: store svgs inside a binary blobk in transaction data
    // each minor has an svg which they set in their respective transaction data

    // seed the random generator
    std::srand(static_cast<unsigned int>(time(nullptr)));

    Blockchain fakeCoin("FakeCoin");

    fakeCoin.addBlock({7.5, "Jack", "Sara", time(nullptr)});
    fakeCoin.addBlock({ 0.07653124, "Maria", "Frank", time(nullptr) });

    std::cout << "Is chain valid? " << fakeCoin.isChainValid() << std::endl;

    // :)
    Block* hackBlock = fakeCoin.getLatestBlock();
    char* p = reinterpret_cast<char*>(hackBlock);
    p += 3 * sizeof(size_t);
    TransactionData* data = (reinterpret_cast<TransactionData*>(p));
    data->amount = 12345;

    std::cout << "Is chain STILL valid? " << fakeCoin.isChainValid() << std::endl;

    fakeCoin.printChain();

    data->amount = 0.07653124; // make the block valid again

    Blockchain congressCoin(bcName);
    char c;
    std::cout << "\n\nPLEASE PRESS A KEY AND CONFIRM WITH ENTER (RETURN) IN ORDER TO START THE CONGRESS COIN GENERATOR\n\n";
    std::cin >> c;

    std::vector<std::unique_ptr<Miner>> miners;
    std::unique_ptr<Miner> minerR = std::make_unique<Miner>(&congressCoin, 7, "RepublicanMiner");
    std::unique_ptr<Miner> minerD = std::make_unique<Miner>(&congressCoin, 3, "DemocratMiner");
    std::unique_ptr<Miner> minerL = std::make_unique<Miner>(&congressCoin, 50, "LibertarianMiner");
    miners.emplace_back(std::move(minerR));
    miners.emplace_back(std::move(minerD));
    miners.emplace_back(std::move(minerL));
    congressCoin.addMiners(miners);
    congressCoin.doMining();

    congressCoin.stopMining(120000); // mine blocks for 2 minutes
    congressCoin.printChain();

    return 0;
}
