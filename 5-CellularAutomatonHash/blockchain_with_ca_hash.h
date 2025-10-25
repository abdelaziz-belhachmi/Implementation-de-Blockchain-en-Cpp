//
// Created by abdelaziz on 10/25/2025.
//

#ifndef BLOCKCHAIN_WITH_CA_H
#define BLOCKCHAIN_WITH_CA_H

#include "cellular_automaton.h"
#include "../4-BlockchainComplete/complete_blockchain.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

// Enum for hash mode selection
enum HashMode {
    SHA256_MODE,
    AC_HASH_MODE
};

// Extended block class with configurable hash function
class BlockWithCA : public BlockComplete {
private:
    HashMode hashMode;
    uint32_t caRule;
    size_t caSteps;

public:
    BlockWithCA(int idx, const std::string& prevHash,
                const std::vector<Transaction>& txs,
                HashMode mode = SHA256_MODE,
                uint32_t rule = 30,
                size_t steps = 128)
            : BlockComplete(idx, prevHash, txs),
              hashMode(mode), caRule(rule), caSteps(steps) {
        calculateHash();
    }

    // SHA256 hash (original implementation)
    std::string sha256Hash(const std::string& data) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)data.c_str(), data.length(), hash);

        std::stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return ss.str();
    }

    // Calculate hash based on selected mode
    void calculateHash() {
        std::stringstream ss;
        ss << index << timestamp << previousHash << merkleRoot << nonce;

        for(const auto& tx : transactions) {
            ss << tx.id << tx.sender << tx.receiver << tx.amount;
        }

        std::string data = ss.str();

        // 3.1 & 3.2: Use selected hash mode
        if (hashMode == AC_HASH_MODE) {
            hash = ac_hash(data, caRule, caSteps);
        } else {
            hash = sha256Hash(data);
        }
    }

    // Mine block with selected hash function
    void mineBlock(int difficulty) {
        std::string target(difficulty, '0');

        do {
            nonce++;
            calculateHash();
        } while (hash.substr(0, difficulty) != target);
    }

    // Getters for CA parameters
    HashMode getHashMode() const { return hashMode; }
    uint32_t getCaRule() const { return caRule; }
    size_t getCaSteps() const { return caSteps; }

    // Set CA parameters
    void setCaRule(uint32_t rule) {
        caRule = rule;
        calculateHash();
    }

    void setCaSteps(size_t steps) {
        caSteps = steps;
        calculateHash();
    }
};

// Extended blockchain class with CA hash support
class BlockchainWithCA {
private:
    std::vector<BlockWithCA> chain;
    HashMode defaultHashMode;
    uint32_t defaultCaRule;
    size_t defaultCaSteps;
    std::vector<Validator> validators;

    BlockWithCA createGenesisBlock() {
        std::vector<Transaction> emptyTxs;
        return BlockWithCA(0, "0", emptyTxs, defaultHashMode, defaultCaRule, defaultCaSteps);
    }

public:
    BlockchainWithCA(HashMode mode = SHA256_MODE, uint32_t rule = 30, size_t steps = 128)
            : defaultHashMode(mode), defaultCaRule(rule), defaultCaSteps(steps) {
        chain.push_back(createGenesisBlock());
    }

    // Add block with Proof of Work
    void addBlockPoW(const std::vector<Transaction>& transactions, int difficulty) {
        BlockWithCA newBlock(chain.size(),
                             chain.back().getHash(),
                             transactions,
                             defaultHashMode,
                             defaultCaRule,
                             defaultCaSteps);
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

    // Add block with Proof of Stake
    void addBlockPoS(const std::vector<Transaction>& transactions) {
        if (validators.empty()) {
            std::cout << "No validators registered!" << std::endl;
            return;
        }

        std::string selectedValidator = selectValidator();

        BlockWithCA newBlock(chain.size(),
                             chain.back().getHash(),
                             transactions,
                             defaultHashMode,
                             defaultCaRule,
                             defaultCaSteps);
        newBlock.setValidator(selectedValidator);
        chain.push_back(newBlock);
    }

    // Validate chain with current hash function
    bool isChainValid() {
        for (size_t i = 1; i < chain.size(); i++) {
            BlockWithCA currentBlock = chain[i];
            BlockWithCA previousBlock = chain[i - 1];

            // Recalculate hash to verify
            std::string originalHash = currentBlock.getHash();
            BlockWithCA tempBlock = currentBlock;
            tempBlock.calculateHash();

            if (originalHash != tempBlock.getHash()) {
                return false;
            }

            if (currentBlock.getPreviousHash() != previousBlock.getHash()) {
                return false;
            }
        }
        return true;
    }

    // Validator management
    void addValidator(const std::string& address, double stake) {
        validators.push_back(Validator(address, stake));
    }

    std::string selectValidator() {
        double totalStake = 0;
        for (const auto& v : validators) {
            totalStake += v.stake;
        }

        double random = (double)rand() / RAND_MAX * totalStake;
        double cumulative = 0;

        for (const auto& v : validators) {
            cumulative += v.stake;
            if (random <= cumulative) {
                return v.address;
            }
        }

        return validators[0].address;
    }

    // Getters
    BlockWithCA getLastBlock() const { return chain.back(); }
    size_t getSize() const { return chain.size(); }
    std::vector<Validator> getValidators() const { return validators; }
    HashMode getHashMode() const { return defaultHashMode; }

    // Change hash mode for future blocks
    void setHashMode(HashMode mode) { defaultHashMode = mode; }
    void setCaRule(uint32_t rule) { defaultCaRule = rule; }
    void setCaSteps(size_t steps) { defaultCaSteps = steps; }

    // Get block at index
    BlockWithCA getBlock(size_t index) const {
        if (index < chain.size()) {
            return chain[index];
        }
        return chain[0];
    }
};

#endif // BLOCKCHAIN_WITH_CA_H