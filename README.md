<h1>CongressCoin</h1>

Congress Coin is a (Fake) C++ Cryptocurrency - based on Blockchain technology.

It can simulate miners using concurrent threads and a concurrent message queue.
These miners can produce new blocks and add them to the chain using *fake work*.

Fake work means that the threads just sleep for a random amount of time to simulate some work.
In addition, the difficulty does increases linearly with the number of blocks in the chain.
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

However, I will only do this last **Rendering** part if will have the time and mood. **Text is ok for now :)**.

Note that the miners are running for **2 minutes** by default!

**Code Structure**

* **CongressCoin.cpp/.h**

              entry point - data setup & main function
  * **Blockchain.cpp/.h**

              can be constructed, validated, printed
    * **Block.cpp/.h**

              a blockchain is made of blocks
              (stores a link to prev block hash,
              can check validity,
              can be chained together)
      * **TransactionData.h**

              each block has transaction data
              (monetary amount, sender, receiver,
              can also compute a hash of the data)
  * **Miner.cpp/.h**

         - 1 or more miners are created in the main function
         - they have a number of nodes and create threads for each
         - each node works towards completing a block
         - blocks are passed through a message queue
          (it is "static", so there is only 1 queue for all miners)
         - blocks are received from the queue and added to the blockchain


**How to Build**

`mkdir build` folder<br>
`cd build`<br>
`cmake ..`<br>
`make`<br>

**C++17** is required.

Run **CongressCoin** or **CongressCoin.exe**

**Example output** (for the Concurrent Congress Miner)

...

Block =================
Index: 1930
Hash: 16607105036473875798
Amount: 9397.02
Sender: RepublicanMiner
Receiver: CongressCoin
Timestamp: 1585603536
Previous Hash: 7144287885497096975
Block Valid? 1

Block =================
Index: 1931
Hash: 15265414603722926998
Amount: 14424
Sender: LibertarianMiner
Receiver: CongressCoin
Timestamp: 1585603536
Previous Hash: 16607105036473875798
Block Valid? 1

Block =================
Index: 1932
Hash: 17792309275297261657
Amount: 16064.7
Sender: LibertarianMiner
Receiver: CongressCoin
Timestamp: 1585603536
Previous Hash: 15265414603722926998
Block Valid? 1

=== SENDER SUMMARY ===

Sender/Miner: DemocratMiner has sent/produced: 95 blocks.
Sender/Miner: LibertarianMiner has sent/produced: 1610 blocks.
Sender/Miner: RepublicanMiner has sent/produced: 225 blocks.