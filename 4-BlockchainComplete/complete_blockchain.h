//
// Created by abdelaziz on 10/16/2025.
//

#ifndef IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_COMPLETE_BLOCKCHAIN_H
#define IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_COMPLETE_BLOCKCHAIN_H

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <random>

class Transaction {
public:
    std::string id;
    std::string sender;
    std::string receiver;
    double amount;

    Transaction(const std::string& txId, const std::string& from,
                const std::string& to, double amt)
            : id(txId), sender(from), receiver(to), amount(amt) {}

    std::string toString() const {
        std::stringstream ss;
        ss << id << sender << receiver << amount;
        return ss.str();
    }
};

class MerkleTreeComplete {
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

public:
    std::string getMerkleRoot(const std::vector<Transaction>& transactions) {
        if(transactions.empty()) {
            return calculateHash("");
        }

        std::vector<std::string> currentLevel;
        for(const auto& tx : transactions) {
            currentLevel.push_back(calculateHash(tx.toString()));
        }

        while(currentLevel.size() > 1) {
            std::vector<std::string> nextLevel;

            for(size_t i = 0; i < currentLevel.size(); i += 2) {
                if(i + 1 < currentLevel.size()) {
                    nextLevel.push_back(calculateHash(currentLevel[i] + currentLevel[i + 1]));
                } else {
                    nextLevel.push_back(calculateHash(currentLevel[i] + currentLevel[i]));
                }
            }

            currentLevel = nextLevel;
        }

        return currentLevel[0];
    }
};

class BlockComplete {
private:
    int index;
    time_t timestamp;
    std::string previousHash;
    std::string merkleRoot;
    int nonce;
    std::string hash;
    std::vector<Transaction> transactions;
    std::string validatorAddress;

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
    BlockComplete(int idx, const std::string& prevHash,
                  const std::vector<Transaction>& txs)
            : index(idx), previousHash(prevHash), transactions(txs),
              nonce(0), validatorAddress("") {
        timestamp = time(nullptr);

        MerkleTreeComplete merkle;
        merkleRoot = merkle.getMerkleRoot(transactions);
        hash = "";
    }

    void mineBlock(int difficulty) {
        std::string target(difficulty, '0');

        do {
            nonce++;
            hash = calculateBlockHash();
        } while (hash.substr(0, difficulty) != target);
    }

    void validateBlockPoS(const std::string& validator) {
        validatorAddress = validator;
        hash = calculateBlockHash();
    }

    std::string calculateBlockHash() {
        std::stringstream ss;
        ss << index << timestamp << previousHash << merkleRoot << nonce << validatorAddress;
        return calculateHash(ss.str());
    }

    std::string getHash() const { return hash; }
    int getIndex() const { return index; }
    std::string getPreviousHash() const { return previousHash; }
    std::string getMerkleRoot() const { return merkleRoot; }
    int getNonce() const { return nonce; }
    time_t getTimestamp() const { return timestamp; }
    std::string getValidator() const { return validatorAddress; }
    const std::vector<Transaction>& getTransactions() const { return transactions; }
};

class ValidatorComplete {
public:
    std::string address;
    int stake;

    ValidatorComplete(const std::string& addr, int stakeAmount)
            : address(addr), stake(stakeAmount) {}
};

class CompleteBlockchain {
private:
    std::vector<BlockComplete> chain;
    std::vector<ValidatorComplete> validators;
    std::mt19937 rng;

public:
    CompleteBlockchain() : rng(std::random_device{}()) {
        chain.push_back(createGenesisBlock());
    }

    BlockComplete createGenesisBlock() {
        std::vector<Transaction> genesisTxs;
        genesisTxs.push_back(Transaction("TX0", "Genesis", "Genesis", 0));
        BlockComplete genesis(0, "0", genesisTxs);
        genesis.validateBlockPoS("Genesis");
        return genesis;
    }

    BlockComplete getLastBlock() {
        return chain.back();
    }

    void addValidator(const std::string& address, int stake) {
        validators.push_back(ValidatorComplete(address, stake));
    }

    std::string selectValidator() {
        if(validators.empty()) {
            return "";
        }

        int totalStake = 0;
        for(const auto& v : validators) {
            totalStake += v.stake;
        }

        std::uniform_int_distribution<int> dist(0, totalStake - 1);
        int randomValue = dist(rng);

        int cumulativeStake = 0;
        for(const auto& v : validators) {
            cumulativeStake += v.stake;
            if(randomValue < cumulativeStake) {
                return v.address;
            }
        }

        return validators[0].address;
    }

    void addBlockPoW(const std::vector<Transaction>& transactions, int difficulty) {
        BlockComplete newBlock(chain.size(), getLastBlock().getHash(), transactions);
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

    void addBlockPoS(const std::vector<Transaction>& transactions) {
        BlockComplete newBlock(chain.size(), getLastBlock().getHash(), transactions);
        std::string validator = selectValidator();
        newBlock.validateBlockPoS(validator);
        chain.push_back(newBlock);
    }

    bool isChainValid() {
        for(size_t i = 1; i < chain.size(); i++) {
            BlockComplete currentBlock = chain[i];
            BlockComplete previousBlock = chain[i - 1];

            if(currentBlock.getHash() != currentBlock.calculateBlockHash()) {
                return false;
            }

            if(currentBlock.getPreviousHash() != previousBlock.getHash()) {
                return false;
            }

            MerkleTreeComplete merkle;
            if(currentBlock.getMerkleRoot() != merkle.getMerkleRoot(currentBlock.getTransactions())) {
                return false;
            }
        }
        return true;
    }

    size_t getSize() const { return chain.size(); }
    BlockComplete getBlock(int index) const { return chain[index]; }
    const std::vector<ValidatorComplete>& getValidators() const { return validators; }
};


#endif //IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_COMPLETE_BLOCKCHAIN_H
