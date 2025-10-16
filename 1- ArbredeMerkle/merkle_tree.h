//
// Created by abdelaziz on 10/16/2025.
//

#ifndef IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_MERKLE_TREE_H
#define IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_MERKLE_TREE_H

#include <string>
#include <vector>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

class MerkleTree {
private:
    std::string calculateHash(const std::string& data) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)data.c_str(), data.size(), hash);

        std::stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return ss.str();
    }

    std::string combineHashes(const std::string& left, const std::string& right) {
        return calculateHash(left + right);
    }

public:
    std::string getMerkleRoot(std::vector<std::string> transactions) {
        if(transactions.empty()) {
            return calculateHash("");
        }

        std::vector<std::string> currentLevel;
        for(const auto& tx : transactions) {
            currentLevel.push_back(calculateHash(tx));
        }

        while(currentLevel.size() > 1) {
            std::vector<std::string> nextLevel;

            for(size_t i = 0; i < currentLevel.size(); i += 2) {
                if(i + 1 < currentLevel.size()) {
                    nextLevel.push_back(combineHashes(currentLevel[i], currentLevel[i + 1]));
                } else {
                    nextLevel.push_back(combineHashes(currentLevel[i], currentLevel[i]));
                }
            }

            currentLevel = nextLevel;
        }

        return currentLevel[0];
    }
};


#endif //IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_MERKLE_TREE_H
