#include "Block.h"

Block::Block(int idx, const TransactionData& d, size_t prevHash) 
    : m_index(idx)
    , m_data(d)
    , m_prevHash(prevHash)
{
    m_blockHash = generateHash();
}

Block::Block(Block&& source)
{
    //std::cout << "Move constructor - BLOCK " << std::endl;
    this->m_blockHash = source.m_blockHash;
    this->m_data = source.m_data;
    this->m_index = source.m_index;
    this->m_prevHash = source.m_prevHash;

    // invalidate source
    source.m_blockHash = 0;
    source.m_data = {};
    source.m_index = 0;
    source.m_prevHash = 0;
}

Block& Block::operator=(Block&& source)
{
    std::cout << "Move assignment operator - BLOCK " << std::endl;
    if (&source == this) // protect against self-assignment
        return *this;

    this->m_blockHash = source.m_blockHash;
    this->m_data = source.m_data;
    this->m_index = source.m_index;
    this->m_prevHash = source.m_prevHash;
    
    // invalidate source
    source.m_blockHash = 0;
    source.m_data = {};
    source.m_index = 0;
    source.m_prevHash = 0;

    return *this;
}

int Block::getIndex() const
{
    return m_index;
}

size_t Block::getHash() const
{
    return m_blockHash;
}

size_t Block::getPrevHash() const
{
    return m_prevHash;
}

TransactionData Block::getData() const
{
    return m_data;
}

bool Block::isHashValid() const
{
    return generateHash() == getHash();
}

size_t Block::generateHash() const
{
    std::hash<std::string> dataHash;
    std::hash<std::string> prevHash;

    //return finalHash(dataHash(toHash) + prevHash(m_prevHash) );
    return dataHash(m_data.toHashString()) ^ (prevHash(std::to_string(m_prevHash)) << 1);
}
