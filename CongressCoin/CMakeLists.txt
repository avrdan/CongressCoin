# CMakeList.txt : CMake project for CongressCoin, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (CongressCoin "CongressCoin.cpp" "CongressCoin.h" "TransactionData.h" "Block.h" "Block.cpp" "Miner.h" "Miner.cpp" "Blockchain.h" "Blockchain.cpp")


