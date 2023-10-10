#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

struct TradeData {
    unsigned long long lastTimeStamp;
    unsigned long long maxTimeGap;
    unsigned long long volume;
    unsigned long long weightedSum;
    unsigned long long maxPrice;
    
    TradeData() : lastTimeStamp(0), maxTimeGap(0), volume(0), weightedSum(0), maxPrice(0) {}
};

int main(int argc, char *argv[]) {
    std::map<std::string, TradeData> symbolData;
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);

    if (!input.is_open() || !output.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }

    std::string line;
    while (getline(input, line)) {
        unsigned long long timeStamp;
        std::string symbol;
        unsigned long long quantity, price;
        
        std::replace(line.begin(), line.end(), ',', ' ');  // Replacing commas with space for easier parsing
        std::istringstream iss(line);
        
        iss >> timeStamp >> symbol >> quantity >> price;

        if (symbolData.find(symbol) == symbolData.end()) {
            symbolData[symbol].lastTimeStamp = timeStamp;
        } else {
            unsigned long long gap = timeStamp - symbolData[symbol].lastTimeStamp;
            if (gap > symbolData[symbol].maxTimeGap) {
                symbolData[symbol].maxTimeGap = gap;
            }
            symbolData[symbol].lastTimeStamp = timeStamp;
        }

        symbolData[symbol].volume += quantity;
        symbolData[symbol].weightedSum += (quantity * price);

        if (price > symbolData[symbol].maxPrice) {
            symbolData[symbol].maxPrice = price;
        }
    }

    for (const auto &pair : symbolData) {
        const std::string &symbol = pair.first;
        const TradeData &data = pair.second;

        unsigned long long weightedAvgPrice = data.weightedSum / data.volume;

        output << symbol << "," << data.maxTimeGap << "," << data.volume << "," << weightedAvgPrice << "," << data.maxPrice << std::endl;
    }

    input.close();
    output.close();
    
    return 0;
}
