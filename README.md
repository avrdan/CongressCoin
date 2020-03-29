<h1>CongressCoin</h1>

Congress Coin is a (Fake) C++ Cryptocurrency - based on Blockchain technology.

It can simulate miners using concurrent threads and a concurrent message queue.
These miners can produce new blocks and add them to the chain using *fake work*.

Fake work means that the threads just sleep for an random amount of time to simulate some work.
In addition, the difficulty does increase exponentially (powers of 2), but it is based on the number of current blocks in the chain.
Thus, this is also not realistic, although it could be easily changed to the real algorithm.

Note that this is for entertainment purposes only. It does not use appropriate hashing algorithms
and relies on the standard *std::hash*.

Furthermore, it can detect if any block or even the whole chain are valid.
The hash of the previous block and the hash of the current block are computed based on the
transaction data. This combined hash can be checked in order to make sure the block has not been tempered with.

In the chosen scenario, each miner corresponds to a US political party.

The idea was to also display the blocks in a grid, perhaps also with a snake-like head, using SDL.
This crypto-snake would keep growing, each new block being displayed with a separate img 
(depending on the miner which generated the block -i.e elephant image for the Republican miner).

However, I will only do this last Rendering part if will have the time and mood. This is ok for now :).
