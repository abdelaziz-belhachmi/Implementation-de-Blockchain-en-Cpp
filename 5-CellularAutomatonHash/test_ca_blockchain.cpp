//
// Created by abdelaziz on 10/25/2025.
//


#include "blockchain_with_ca_hash.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <cmath>

void printSeparator() {
    std::cout << "========================================" << std::endl;
}

// Question 4: Compare SHA256 vs AC_HASH performance
void compareHashPerformance() {
    std::cout << "\n=== QUESTION 4: Performance Comparison ===" << std::endl;
    printSeparator();

    const int NUM_BLOCKS = 10;
    const int DIFFICULTY = 3;

    std::vector<Transaction> testTxs;
    testTxs.push_back(Transaction("TX1", "Alice", "Bob", 100.0));
    testTxs.push_back(Transaction("TX2", "Bob", "Charlie", 50.0));

    // Test SHA256
    std::cout << "\nTesting SHA256 Mode..." << std::endl;
    BlockchainWithCA chainSHA(SHA256_MODE);

    auto startSHA = std::chrono::high_resolution_clock::now();
    int totalIterationsSHA = 0;

    for (int i = 0; i < NUM_BLOCKS; i++) {
        int nonceStart = chainSHA.getLastBlock().getNonce();
        chainSHA.addBlockPoW(testTxs, DIFFICULTY);
        int nonceEnd = chainSHA.getLastBlock().getNonce();
        totalIterationsSHA += (nonceEnd - nonceStart);
    }

    auto endSHA = std::chrono::high_resolution_clock::now();
    auto durationSHA = std::chrono::duration_cast<std::chrono::milliseconds>(endSHA - startSHA);

    // Test AC_HASH Rule 30
    std::cout << "Testing AC_HASH Mode (Rule 30)..." << std::endl;
    BlockchainWithCA chainAC(AC_HASH_MODE, 30, 128);

    auto startAC = std::chrono::high_resolution_clock::now();
    int totalIterationsAC = 0;

    for (int i = 0; i < NUM_BLOCKS; i++) {
        int nonceStart = chainAC.getLastBlock().getNonce();
        chainAC.addBlockPoW(testTxs, DIFFICULTY);
        int nonceEnd = chainAC.getLastBlock().getNonce();
        totalIterationsAC += (nonceEnd - nonceStart);
    }

    auto endAC = std::chrono::high_resolution_clock::now();
    auto durationAC = std::chrono::duration_cast<std::chrono::milliseconds>(endAC - startAC);

    // 4.3: Results table
    std::cout << "\n=== RESULTS TABLE ===" << std::endl;
    std::cout << std::left << std::setw(20) << "Hash Method"
              << std::setw(20) << "Total Time (ms)"
              << std::setw(20) << "Avg Time/Block (ms)"
              << std::setw(20) << "Avg Iterations" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    std::cout << std::left << std::setw(20) << "SHA256"
              << std::setw(20) << durationSHA.count()
              << std::setw(20) << durationSHA.count() / NUM_BLOCKS
              << std::setw(20) << totalIterationsSHA / NUM_BLOCKS << std::endl;

    std::cout << std::left << std::setw(20) << "AC_HASH (Rule 30)"
              << std::setw(20) << durationAC.count()
              << std::setw(20) << durationAC.count() / NUM_BLOCKS
              << std::setw(20) << totalIterationsAC / NUM_BLOCKS << std::endl;

    std::cout << "\nSpeedup factor: " << std::fixed << std::setprecision(2)
              << (double)durationAC.count() / durationSHA.count() << "x "
              << (durationAC.count() > durationSHA.count() ? "(SHA256 faster)" : "(AC_HASH faster)")
              << std::endl;
}

// Question 5: Avalanche effect analysis
void analyzeAvalancheEffect() {
    std::cout << "\n=== QUESTION 5: Avalanche Effect Analysis ===" << std::endl;
    printSeparator();

    std::string original = "Test message for avalanche";
    std::string modified = "Test message for avalancha"; // Changed last character

    // Test with Rule 30
    std::string hash1 = ac_hash(original, 30, 128);
    std::string hash2 = ac_hash(modified, 30, 128);

    // Count different bits
    int differentBits = 0;
    int totalBits = hash1.length() * 4; // Each hex char = 4 bits

    for (size_t i = 0; i < hash1.length(); i++) {
        int val1 = (hash1[i] >= 'a') ? (hash1[i] - 'a' + 10) : (hash1[i] - '0');
        int val2 = (hash2[i] >= 'a') ? (hash2[i] - 'a' + 10) : (hash2[i] - '0');
        int xorVal = val1 ^ val2;

        // Count bits set in xorVal
        for (int j = 0; j < 4; j++) {
            if (xorVal & (1 << j)) {
                differentBits++;
            }
        }
    }

    double percentage = (double)differentBits / totalBits * 100.0;

    std::cout << "Original:  " << original << std::endl;
    std::cout << "Modified:  " << modified << std::endl;
    std::cout << "\nHash1: " << hash1.substr(0, 40) << "..." << std::endl;
    std::cout << "Hash2: " << hash2.substr(0, 40) << "..." << std::endl;
    std::cout << "\nDifferent bits: " << differentBits << " / " << totalBits << std::endl;
    std::cout << "Percentage: " << std::fixed << std::setprecision(2) << percentage << "%" << std::endl;
    std::cout << "\nIdeal avalanche effect: ~50%" << std::endl;
    std::cout << "Result: " << (percentage >= 45 && percentage <= 55 ? "GOOD" : "NEEDS IMPROVEMENT") << std::endl;
}

// Question 6: Bit distribution analysis
void analyzeBitDistribution() {
    std::cout << "\n=== QUESTION 6: Bit Distribution Analysis ===" << std::endl;
    printSeparator();

    const int NUM_SAMPLES = 1000;
    int totalBits = 0;
    int onesCount = 0;

    std::cout << "Generating " << NUM_SAMPLES << " hashes..." << std::endl;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        std::stringstream ss;
        ss << "Sample message " << i;
        std::string hash = ac_hash(ss.str(), 30, 128);

        // Count 1s in the hash
        for (char c : hash) {
            int val = (c >= 'a') ? (c - 'a' + 10) : (c - '0');
            for (int j = 0; j < 4; j++) {
                totalBits++;
                if (val & (1 << j)) {
                    onesCount++;
                }
            }
        }
    }

    double percentage = (double)onesCount / totalBits * 100.0;

    std::cout << "\nTotal bits analyzed: " << totalBits << std::endl;
    std::cout << "Bits set to 1: " << onesCount << std::endl;
    std::cout << "Percentage of 1s: " << std::fixed << std::setprecision(2) << percentage << "%" << std::endl;
    std::cout << "\n6.2: Distribution is " << (percentage >= 48 && percentage <= 52 ? "BALANCED" : "UNBALANCED") << std::endl;
}

// Question 7: Compare different CA rules
void compareRules() {
    std::cout << "\n=== QUESTION 7: Comparing Different Rules ===" << std::endl;
    printSeparator();

    std::vector<uint32_t> rules = {30, 90, 110};
    std::vector<Transaction> testTxs;
    testTxs.push_back(Transaction("TX1", "A", "B", 50.0));

    const int NUM_BLOCKS = 5;
    const int DIFFICULTY = 3;

    std::cout << std::left << std::setw(15) << "Rule"
              << std::setw(20) << "Time (ms)"
              << std::setw(25) << "Avg Iterations"
              << std::setw(20) << "Chain Valid?" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (uint32_t rule : rules) {
        BlockchainWithCA chain(AC_HASH_MODE, rule, 128);

        auto start = std::chrono::high_resolution_clock::now();
        int totalIterations = 0;

        for (int i = 0; i < NUM_BLOCKS; i++) {
            int nonceStart = chain.getLastBlock().getNonce();
            chain.addBlockPoW(testTxs, DIFFICULTY);
            int nonceEnd = chain.getLastBlock().getNonce();
            totalIterations += (nonceEnd - nonceStart);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        bool valid = chain.isChainValid();

        std::cout << std::left << std::setw(15) << ("Rule " + std::to_string(rule))
                  << std::setw(20) << duration.count()
                  << std::setw(25) << (totalIterations / NUM_BLOCKS)
                  << std::setw(20) << (valid ? "YES" : "NO") << std::endl;
    }

    std::cout << "\n7.3: Best rule for hashing: Rule 30" << std::endl;
    std::cout << "Reason: Best balance of complexity, unpredictability, and performance" << std::endl;
}

// Test that validation works with AC_HASH
void testValidation() {
    std::cout << "\n=== QUESTION 3.3: Validation Test ===" << std::endl;
    printSeparator();

    BlockchainWithCA chain(AC_HASH_MODE, 30, 128);

    std::vector<Transaction> txs;
    txs.push_back(Transaction("TX1", "Alice", "Bob", 100.0));

    chain.addBlockPoW(txs, 3);
    chain.addBlockPoW(txs, 3);

    std::cout << "Blockchain created with " << chain.getSize() << " blocks using AC_HASH" << std::endl;
    std::cout << "Validation result: " << (chain.isChainValid() ? "VALID ✓" : "INVALID ✗") << std::endl;
}

int main() {
    std::cout << "BLOCKCHAIN WITH CELLULAR AUTOMATON HASH" << std::endl;
    printSeparator();

    // Question 3: Integration test
    testValidation();

    // Question 4: Performance comparison
    compareHashPerformance();

    // Question 5: Avalanche effect
    analyzeAvalancheEffect();

    // Question 6: Bit distribution
    analyzeBitDistribution();

    // Question 7: Compare rules
    compareRules();

    printSeparator();
    std::cout << "All tests completed!" << std::endl;

    return 0;
}