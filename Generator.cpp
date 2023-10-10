#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <map>


const unsigned long long MAX_TIMESTAMP_INCREMENT = 5000; // Adjust this value to get desired maximum increment
const int MAX_SYMBOLS = 2000;
const int MAX_QUANTITY = 100;
const int MAX_PRICE = 1500;


struct TradeInfo {
    unsigned long long lastTimeStamp = 0;
    unsigned long long maxTimeGap = 0;
    unsigned long long volume = 0;
    unsigned long long weightedPriceSum = 0;
    unsigned long long maxPrice = 0;
};


std::string generateSymbol() {
    std::string chars = "abcdefghijklmnopqrstuvwxyz";
    std::string symbol;
    for (int i = 0; i < 3; i++) {
        symbol += chars[rand() % chars.size()];
    }
    return symbol;
}


int main(int argc, char *argv[]) {
    srand(time(NULL));

    // File output
    std::ofstream inputFile(argv[1]);
    std::map<std::string, TradeInfo> symbolInfo;

    // Create unique symbols
    std::vector<std::string> symbols;
    for(int i = 0; i < MAX_SYMBOLS; i++) {
        symbols.push_back(generateSymbol());
    }

    unsigned long long currentTimestamp = 0;
    // Generate random data
    unsigned long long current_timestamp = 0;
    for (const auto& symbol : symbols) {
        unsigned long long entries = 1 + rand() % 100; // Arbitrarily choosing to produce up to 100 entries per symbol
        for(unsigned long long j = 0; j < entries; j++) {
            current_timestamp += rand() % MAX_TIMESTAMP_INCREMENT;
            unsigned long long quantity = 1 + rand() % MAX_QUANTITY;
            unsigned long long price = 1 + rand() % MAX_PRICE;
            
            inputFile << current_timestamp << ","
                 << symbol << ","
                 << quantity << ","
                 << price << "\n";
            // Update symbol info
            TradeInfo &info = symbolInfo[symbol];
            if(info.lastTimeStamp != 0) {
                unsigned long long timeGap = current_timestamp - info.lastTimeStamp;
                if(timeGap > info.maxTimeGap) {
                    info.maxTimeGap = timeGap;
                }
            }
            info.lastTimeStamp = current_timestamp;
            info.volume += quantity;
            info.weightedPriceSum += quantity * price;
            if(price > info.maxPrice) {
                info.maxPrice = price;
            }
        }
    }
    inputFile.close();
    std::cout << "Generated " << symbols.size() << " symbols with up to " << MAX_QUANTITY << " entries each in " << argv[1] << std::endl;

    // Save the computed values in answer.csv
    std::ofstream answerFile(argv[2]);
    for (const auto& kv : symbolInfo) {
        const std::string &symbol = kv.first;
        const TradeInfo &info = kv.second;
        unsigned long long weightedAveragePrice = info.weightedPriceSum / info.volume;
        answerFile << symbol << ","
                   << info.maxTimeGap << ","
                   << info.volume << ","
                   << weightedAveragePrice << ","
                   << info.maxPrice << "\n";
    }
    answerFile.close();
    std::cout << "Saved computed values in " << argv[2] << std::endl;
    return 0;
}
