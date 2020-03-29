#pragma once

#include "TransactionData.h"

// Block Class - any block can have multiple transactions
class Block
{
public:
    Block(int idx, const TransactionData& d, size_t prevHash);
    ~Block() = default;

    Block(Block&& source);
    Block& operator=(Block&& source);

    int getIndex() const;
    size_t getHash() const;
    size_t getPrevHash() const;
    TransactionData getData() const;

    bool isHashValid() const;
private:
    size_t generateHash() const;

    int m_index = 0;
    size_t m_blockHash = 0;
    size_t m_prevHash = 0;

    TransactionData m_data;
};
