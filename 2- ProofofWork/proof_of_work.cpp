//
// Created by abdelaziz on 10/16/2025.
//

#include "proof_of_work.h"
#include <iostream>
#include <chrono>

int main() {
    std::cout << "=== Test Proof of Work avec differentes difficultes ===" << std::endl << std::endl;

    for(int difficulty = 1; difficulty <= 5; difficulty++) {
        std::cout << "Difficulte: " << difficulty << std::endl;

        Blockchain blockchain;

        auto start = std::chrono::high_resolution_clock::now();

        Block block1(1, blockchain.getLastBlock().getHash(), "Block 1 Data");
        block1.mineBlock(difficulty);
        blockchain.addBlock(block1);

        Block block2(2, blockchain.getLastBlock().getHash(), "Block 2 Data");
        block2.mineBlock(difficulty);
        blockchain.addBlock(block2);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Block 1 Hash: " << block1.getHash() << std::endl;
        std::cout << "Block 1 Nonce: " << block1.getNonce() << std::endl;
        std::cout << "Block 2 Hash: " << block2.getHash() << std::endl;
        std::cout << "Block 2 Nonce: " << block2.getNonce() << std::endl;
        std::cout << "Temps d'execution: " << duration.count() << " ms" << std::endl;
        std::cout << "Chaine valide: " << (blockchain.isChainValid() ? "OUI" : "NON") << std::endl;
        std::cout << std::endl;
    }

    std::cout << "=== Test de verification d'integrite ===" << std::endl;
    Blockchain testChain;

    Block b1(1, testChain.getLastBlock().getHash(), "Transaction 1");
    b1.mineBlock(3);
    testChain.addBlock(b1);

    Block b2(2, testChain.getLastBlock().getHash(), "Transaction 2");
    b2.mineBlock(3);
    testChain.addBlock(b2);

    std::cout << "Chaine initiale valide: " << (testChain.isChainValid() ? "OUI" : "NON") << std::endl;

    return 0;
}