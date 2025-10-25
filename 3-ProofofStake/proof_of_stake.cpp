//
// Created by abdelaziz on 10/16/2025.
//

#include "proof_of_stake.h"
#include "../2-ProofofWork/proof_of_work.h"
#include <iostream>
#include <chrono>

int main() {
    std::cout << "=== Test Proof of Stake ===" << std::endl << std::endl;

    BlockchainPoS blockchainPoS;

    blockchainPoS.addValidator("Validator_A", 100);
    blockchainPoS.addValidator("Validator_B", 200);
    blockchainPoS.addValidator("Validator_C", 50);
    blockchainPoS.addValidator("Validator_D", 150);

    std::cout << "Validateurs ajoutes:" << std::endl;
    for(int i = 0; i < blockchainPoS.getPoS().getValidatorCount(); i++) {
        Validator v = blockchainPoS.getPoS().getValidator(i);
        std::cout << "  " << v.address << " - Stake: " << v.stake << std::endl;
    }
    std::cout << std::endl;

    auto startPoS = std::chrono::high_resolution_clock::now();

    for(int i = 1; i <= 5; i++) {
        std::stringstream ss;
        ss << "Block " << i << " Data PoS";
        BlockPoS block(i, blockchainPoS.getLastBlock().getHash(), ss.str());
        blockchainPoS.addBlock(block);

        std::cout << "Block " << i << " valide par: "
                  << blockchainPoS.getBlock(i).getValidator() << std::endl;
        std::cout << "Hash: " << blockchainPoS.getBlock(i).getHash() << std::endl;
    }

    auto endPoS = std::chrono::high_resolution_clock::now();
    auto durationPoS = std::chrono::duration_cast<std::chrono::microseconds>(endPoS - startPoS);

    std::cout << std::endl << "Temps d'execution PoS: " << durationPoS.count() << " microseconds" << std::endl;
    std::cout << "Chaine PoS valide: " << (blockchainPoS.isChainValid() ? "OUI" : "NON") << std::endl;

    std::cout << std::endl << "=== Comparaison PoW vs PoS ===" << std::endl << std::endl;

    int difficulty = 3;
    std::cout << "Test avec difficulte PoW = " << difficulty << std::endl << std::endl;

    Blockchain blockchainPoW;

    auto startPoW = std::chrono::high_resolution_clock::now();

    for(int i = 1; i <= 5; i++) {
        std::stringstream ss;
        ss << "Block " << i << " Data PoW";
        Block block(i, blockchainPoW.getLastBlock().getHash(), ss.str());
        block.mineBlock(difficulty);
        blockchainPoW.addBlock(block);
    }

    auto endPoW = std::chrono::high_resolution_clock::now();
    auto durationPoW = std::chrono::duration_cast<std::chrono::microseconds>(endPoW - startPoW);

    std::cout << "Temps d'execution PoW: " << durationPoW.count() << " microseconds" << std::endl;
    std::cout << "Temps d'execution PoS: " << durationPoS.count() << " microseconds" << std::endl;
    std::cout << std::endl;

    std::cout << "Ratio PoW/PoS: " << (double)durationPoW.count() / durationPoS.count() << "x" << std::endl;
    std::cout << "Methode la plus rapide: " << (durationPoS.count() < durationPoW.count() ? "PoS" : "PoW") << std::endl;

    return 0;
}