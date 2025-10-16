//
// Created by abdelaziz on 10/16/2025.
//

#include "merkle_tree.h"
#include <iostream>

int main() {
    MerkleTree tree;

    std::cout << "=== Test 1: 4 transactions ===" << std::endl;
    std::vector<std::string> transactions1 = {
            "Alice sends 10 BTC to Bob",
            "Bob sends 5 BTC to Charlie",
            "Charlie sends 3 BTC to Dave",
            "Dave sends 2 BTC to Alice"
    };

    std::string root1 = tree.getMerkleRoot(transactions1);
    std::cout << "Merkle Root: " << root1 << std::endl << std::endl;

    std::cout << "=== Test 2: 3 transactions (nombre impair) ===" << std::endl;
    std::vector<std::string> transactions2 = {
            "Transaction 1",
            "Transaction 2",
            "Transaction 3"
    };

    std::string root2 = tree.getMerkleRoot(transactions2);
    std::cout << "Merkle Root: " << root2 << std::endl << std::endl;

    std::cout << "=== Test 3: 1 transaction ===" << std::endl;
    std::vector<std::string> transactions3 = {"Single transaction"};

    std::string root3 = tree.getMerkleRoot(transactions3);
    std::cout << "Merkle Root: " << root3 << std::endl << std::endl;

    std::cout << "=== Test 4: Verification d'integrite ===" << std::endl;
    std::string root4a = tree.getMerkleRoot(transactions1);
    std::cout << "Root original: " << root4a << std::endl;

    transactions1[0] = "Alice sends 11 BTC to Bob";
    std::string root4b = tree.getMerkleRoot(transactions1);
    std::cout << "Root modifie: " << root4b << std::endl;
    std::cout << "Les roots sont differents: " << (root4a != root4b ? "OUI" : "NON") << std::endl;

    return 0;
}