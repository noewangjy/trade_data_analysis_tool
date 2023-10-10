#include <iostream>
#include <fstream>
#include <string>
#include <map>

struct TradeInfo {
    unsigned long long maxTimeGap;
    unsigned long long volume;
    unsigned long long weightedAveragePrice;
    unsigned long long maxPrice;
};

int main(int argc, char *argv[]) {
    std::ifstream outputFile(argv[1]);
    std::ifstream answerFile(argv[2]);

    if (!outputFile.is_open() || !answerFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return 1;
    }

    std::map<std::string, TradeInfo> outputData, answerData;

    // Load data from output.csv
    std::string line;
    while (std::getline(outputFile, line)) {
        std::string token;
        size_t pos = 0;
        TradeInfo info;
        std::string symbol;

        int counter = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos);
            switch (counter) {
                case 0: symbol = token; break;
                case 1: info.maxTimeGap = std::stoull(token); break;
                case 2: info.volume = std::stoull(token); break;
                case 3: info.weightedAveragePrice = std::stoull(token); break;
            }
            line.erase(0, pos + 1);
            counter++;
        }
        info.maxPrice = std::stoull(line);
        outputData[symbol] = info;
    }

    // Load data from answer.csv
    while (std::getline(answerFile, line)) {
        std::string token;
        size_t pos = 0;
        TradeInfo info;
        std::string symbol;

        int counter = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos);
            switch (counter) {
                case 0: symbol = token; break;
                case 1: info.maxTimeGap = std::stoull(token); break;
                case 2: info.volume = std::stoull(token); break;
                case 3: info.weightedAveragePrice = std::stoull(token); break;
            }
            line.erase(0, pos + 1);
            counter++;
        }
        info.maxPrice = std::stoull(line);
        answerData[symbol] = info;
    }

    outputFile.close();
    answerFile.close();

    // Compare and calculate accuracy
    int totalSymbols = 0;
    int correctSymbols = 0;

    for (const auto &pair : answerData) {
        totalSymbols++;

        auto it = outputData.find(pair.first);
        if (it != outputData.end()) {
            if (it->second.maxTimeGap == pair.second.maxTimeGap &&
                it->second.volume == pair.second.volume &&
                it->second.weightedAveragePrice == pair.second.weightedAveragePrice &&
                it->second.maxPrice == pair.second.maxPrice) {
                correctSymbols++;
            }
        }
    }

    double accuracyRate = (double) correctSymbols / totalSymbols * 100;
    std::cout << "Accuracy Rate: " << accuracyRate << "%" << " on " << argv[2]  << std::endl;

    return 0;
}
