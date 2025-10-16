//
// Created by abdelaziz on 10/16/2025.
//

#ifndef IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_PROOF_OF_WORK_H
#define IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_PROOF_OF_WORK_H

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

class Block {
private:
    int index;
    std::string previousHash;
    std::string data;
    time_t timestamp;
    int nonce;
    std::string hash;

    std::string calculateHash(const std::string& input) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)input.c_str(), input.size(), hash);

        std::stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return ss.str();
    }

public:
    Block(int idx, const std::string& prevHash, const std::string& blockData)
        : index(idx), previousHash(prevHash), data(blockData), nonce(0) {
        timestamp = time(nullptr);
        hash = "";
    }

    void mineBlock(int difficulty) {
        std::string target(difficulty, '0');

        do {
            nonce++;
            hash = calculateBlockHash();
        } while (hash.substr(0, difficulty) != target);
    }

    std::string calculateBlockHash() {
        std::stringstream ss;
        ss << index << previousHash << data << timestamp << nonce;
        return calculateHash(ss.str());
    }

    std::string getHash() const { return hash; }
    int getIndex() const { return index; }
    std::string getPreviousHash() const { return previousHash; }
    std::string getData() const { return data; }
    time_t getTimestamp() const { return timestamp; }
    int getNonce() const { return nonce; }
};

class Blockchain {
private:
    std::vector<Block> chain;

public:
    Blockchain() {
        chain.push_back(createGenesisBlock());
    }

    Block createGenesisBlock() {
        Block genesis(0, "0", "Genesis Block");
        genesis.mineBlock(1);
        return genesis;
    }

    Block getLastBlock() {
        return chain.back();
    }

    void addBlock(Block newBlock) {
        chain.push_back(newBlock);
    }

    bool isChainValid() {
        for(size_t i = 1; i < chain.size(); i++) {
            Block currentBlock = chain[i];
            Block previousBlock = chain[i - 1];

            if(currentBlock.getHash() != currentBlock.calculateBlockHash()) {
                return false;
            }

            if(currentBlock.getPreviousHash() != previousBlock.getHash()) {
                return false;
            }
        }
        return true;
    }

    size_t getSize() const { return chain.size(); }

    Block getBlock(int index) const { return chain[index]; }
};


#endif //IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_PROOF_OF_WORK_H
