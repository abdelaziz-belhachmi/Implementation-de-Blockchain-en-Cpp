//
// Created by abdelaziz on 10/16/2025.
//

#include "complete_blockchain.h"
#include <iostream>
#include <chrono>

void printSeparator() {
    std::cout << "========================================" << std::endl;
}

int main() {
    std::cout << "BLOCKCHAIN COMPLETE - EXERCICE 4" << std::endl;
    printSeparator();

    std::cout << std::endl << "PARTIE 1: Structure des blocs et de la chaine" << std::endl;
    printSeparator();

    CompleteBlockchain blockchain;

    std::vector<Transaction> transactions1;
    transactions1.push_back(Transaction("TX1", "Alice", "Bob", 50.0));
    transactions1.push_back(Transaction("TX2", "Bob", "Charlie", 25.0));
    transactions1.push_back(Transaction("TX3", "Charlie", "Dave", 10.0));

    MerkleTreeComplete merkle;
    std::string root = merkle.getMerkleRoot(transactions1);

    std::cout << "Transactions creees: " << transactions1.size() << std::endl;
    std::cout << "Merkle Root: " << root << std::endl;
    std::cout << "Blockchain initialisee avec bloc Genesis" << std::endl;
    std::cout << "Taille de la chaine: " << blockchain.getSize() << std::endl;

    std::cout << std::endl << "PARTIE 2: Proof of Work" << std::endl;
    printSeparator();

    std::cout << std::endl << "Ajout de blocs avec PoW..." << std::endl;

    std::vector<int> difficulties = {2, 3, 4};

    for(int diff : difficulties) {
        std::cout << std::endl << "Difficulte: " << diff << std::endl;

        std::vector<Transaction> txs;
        for(int i = 0; i < 3; i++) {
            std::stringstream ss;
            ss << "TX_PoW_" << diff << "_" << i;
            txs.push_back(Transaction(ss.str(), "User1", "User2", 10.0 * (i + 1)));
        }

        auto start = std::chrono::high_resolution_clock::now();
        blockchain.addBlockPoW(txs, diff);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        BlockComplete lastBlock = blockchain.getLastBlock();
        std::cout << "Block #" << lastBlock.getIndex() << " mine" << std::endl;
        std::cout << "Hash: " << lastBlock.getHash().substr(0, 20) << "..." << std::endl;
        std::cout << "Nonce: " << lastBlock.getNonce() << std::endl;
        std::cout << "Temps de minage: " << duration.count() << " ms" << std::endl;
    }

    std::cout << std::endl << "Verification de l'integrite de la chaine: "
              << (blockchain.isChainValid() ? "VALIDE" : "INVALIDE") << std::endl;

    std::cout << std::endl << "PARTIE 3: Proof of Stake" << std::endl;
    printSeparator();

    blockchain.addValidator("Validator_Alpha", 100);
    blockchain.addValidator("Validator_Beta", 200);
    blockchain.addValidator("Validator_Gamma", 150);
    blockchain.addValidator("Validator_Delta", 50);

    std::cout << std::endl << "Validateurs enregistres:" << std::endl;
    for(const auto& v : blockchain.getValidators()) {
        std::cout << "  " << v.address << " - Stake: " << v.stake << std::endl;
    }

    std::cout << std::endl << "Ajout de blocs avec PoS..." << std::endl;

    for(int i = 0; i < 5; i++) {
        std::vector<Transaction> txs;
        for(int j = 0; j < 3; j++) {
            std::stringstream ss;
            ss << "TX_PoS_" << i << "_" << j;
            txs.push_back(Transaction(ss.str(), "UserA", "UserB", 5.0 * (j + 1)));
        }

        auto start = std::chrono::high_resolution_clock::now();
        blockchain.addBlockPoS(txs);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        BlockComplete lastBlock = blockchain.getLastBlock();
        std::cout << "Block #" << lastBlock.getIndex() << " valide par: "
                  << lastBlock.getValidator() << " (" << duration.count() << " us)" << std::endl;
    }

    std::cout << std::endl << "Verification de l'integrite de la chaine: "
              << (blockchain.isChainValid() ? "VALIDE" : "INVALIDE") << std::endl;

    std::cout << std::endl << "PARTIE 4: Analyse comparative" << std::endl;
    printSeparator();

    CompleteBlockchain chainPoW;
    CompleteBlockchain chainPoS;

    chainPoS.addValidator("Val1", 100);
    chainPoS.addValidator("Val2", 200);
    chainPoS.addValidator("Val3", 150);

    int numBlocks = 10;
    int difficulty = 3;

    std::cout << std::endl << "Test avec " << numBlocks << " blocs" << std::endl;
    std::cout << "Difficulte PoW: " << difficulty << std::endl << std::endl;

    std::vector<Transaction> testTxs;
    testTxs.push_back(Transaction("TX_TEST", "Sender", "Receiver", 100.0));

    auto startPoW = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numBlocks; i++) {
        chainPoW.addBlockPoW(testTxs, difficulty);
    }
    auto endPoW = std::chrono::high_resolution_clock::now();
    auto durationPoW = std::chrono::duration_cast<std::chrono::milliseconds>(endPoW - startPoW);

    auto startPoS = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numBlocks; i++) {
        chainPoS.addBlockPoS(testTxs);
    }
    auto endPoS = std::chrono::high_resolution_clock::now();
    auto durationPoS = std::chrono::duration_cast<std::chrono::milliseconds>(endPoS - startPoS);

    std::cout << "RESULTATS:" << std::endl;
    std::cout << std::endl << "Proof of Work:" << std::endl;
    std::cout << "  Temps total: " << durationPoW.count() << " ms" << std::endl;
    std::cout << "  Temps moyen par bloc: " << durationPoW.count() / numBlocks << " ms" << std::endl;
    std::cout << "  Chaine valide: " << (chainPoW.isChainValid() ? "OUI" : "NON") << std::endl;

    std::cout << std::endl << "Proof of Stake:" << std::endl;
    std::cout << "  Temps total: " << durationPoS.count() << " ms" << std::endl;
    std::cout << "  Temps moyen par bloc: " << (double)durationPoS.count() / numBlocks << " ms" << std::endl;
    std::cout << "  Chaine valide: " << (chainPoS.isChainValid() ? "OUI" : "NON") << std::endl;

    std::cout << std::endl << "COMPARAISON:" << std::endl;
    std::cout << "  Rapidite: PoS est " << (double)durationPoW.count() / durationPoS.count()
              << "x plus rapide que PoW" << std::endl;
    std::cout << "  Consommation CPU: PoW >> PoS (PoW necessite calculs intensifs)" << std::endl;
    std::cout << "  Mise en oeuvre: PoS plus simple (pas de minage)" << std::endl;

    std::cout << std::endl << "RESUME FINAL:" << std::endl;
    std::cout << "  Blocks totaux dans blockchain principale: " << blockchain.getSize() << std::endl;
    std::cout << "  Validateurs enregistres: " << blockchain.getValidators().size() << std::endl;
    std::cout << "  Integrite de la chaine: " << (blockchain.isChainValid() ? "VALIDE" : "INVALIDE") << std::endl;

    printSeparator();
    std::cout << "FIN DES TESTS" << std::endl;

    return 0;
}