#pragma once

#include <string>
#include <ctime>
#include <iostream>

struct TransactionData
{
    // TODO: add some binary data and also implement copy constructors etc
    //char* blob = nullptr;
    double amount = 0;
    std::string keySender = "None";
    std::string keyReceiver= "None";
    std::time_t timestamp;

    std::string toHashString() const {
        return std::to_string(amount)
            + keyReceiver
            + keySender
            + std::to_string(timestamp);
    }

    bool isValid() const
    {
        return (keySender != "None" && keySender != "None") && amount >= 0;
    }
};