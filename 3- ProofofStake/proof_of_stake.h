//
// Created by abdelaziz on 10/16/2025.
//

#ifndef IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_PROOF_OF_STAKE_H
#define IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_PROOF_OF_STAKE_H

#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <random>

class Validator {
public:
    std::string address;
    int stake;

    Validator(const std::string& addr, int stakeAmount)
        : address(addr), stake(stakeAmount) {}
};

class BlockPoS {
private:
    int index;
    std::string previousHash;
    std::string data;
    time_t timestamp;
    std::string hash;
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
    BlockPoS(int idx, const std::string& prevHash, const std::string& blockData)
        : index(idx), previousHash(prevHash), data(blockData), validatorAddress("") {
        timestamp = time(nullptr);
        hash = "";
    }

    void validateBlock(const std::string& validator) {
        validatorAddress = validator;
        hash = calculateBlockHash();
    }

    std::string calculateBlockHash() {
        std::stringstream ss;
        ss << index << previousHash << data << timestamp << validatorAddress;
        return calculateHash(ss.str());
    }

    std::string getHash() const { return hash; }
    int getIndex() const { return index; }
    std::string getPreviousHash() const { return previousHash; }
    std::string getData() const { return data; }
    time_t getTimestamp() const { return timestamp; }
    std::string getValidator() const { return validatorAddress; }
};

class ProofOfStake {
private:
    std::vector<Validator> validators;
    std::mt19937 rng;

public:
    ProofOfStake() : rng(std::random_device{}()) {}

    void addValidator(const std::string& address, int stake) {
        validators.push_back(Validator(address, stake));
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

    int getValidatorCount() const { return validators.size(); }

    Validator getValidator(int index) const { return validators[index]; }
};

class BlockchainPoS {
private:
    std::vector<BlockPoS> chain;
    ProofOfStake pos;

public:
    BlockchainPoS() {
        chain.push_back(createGenesisBlock());
    }

    BlockPoS createGenesisBlock() {
        BlockPoS genesis(0, "0", "Genesis Block PoS");
        genesis.validateBlock("Genesis");
        return genesis;
    }

    BlockPoS getLastBlock() {
        return chain.back();
    }

    void addValidator(const std::string& address, int stake) {
        pos.addValidator(address, stake);
    }

    void addBlock(BlockPoS newBlock) {
        std::string validator = pos.selectValidator();
        newBlock.validateBlock(validator);
        chain.push_back(newBlock);
    }

    bool isChainValid() {
        for(size_t i = 1; i < chain.size(); i++) {
            BlockPoS currentBlock = chain[i];
            BlockPoS previousBlock = chain[i - 1];

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

    BlockPoS getBlock(int index) const { return chain[index]; }

    ProofOfStake& getPoS() { return pos; }
};


#endif //IMPLEMENTATION_DE_BLOCKCHAIN_EN_CPP_PROOF_OF_STAKE_H
