//
// Created by abdelaziz on 10/25/2025.
//

#ifndef CELLULAR_AUTOMATON_H
#define CELLULAR_AUTOMATON_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <iostream>


class CellularAutomaton {
private:
    std::vector<bool> state;
    uint32_t rule;
    int radius;

    // Apply rule to get next bit based on neighborhood
    bool applyRule(bool left, bool center, bool right) {
        // Convert neighborhood to decimal index (0-7)
        int index = (left << 2) | (center << 1) | right;
        // Check if bit at position 'index' in rule is set
        return (rule >> index) & 1;
    }

public:
    CellularAutomaton(uint32_t r = 30, int rad = 1) : rule(r), radius(rad) {}

    // 1.1. Initialize state from bit vector
    void init_state(const std::vector<bool>& initial_state) {
        state = initial_state;
    }

    // Initialize state from string (convert to bits)
    void init_state_from_string(const std::string& input) {
        state.clear();
        for (char c : input) {
            // Convert each character to 8 bits
            for (int i = 7; i >= 0; i--) {
                state.push_back((c >> i) & 1);
            }
        }
    }

    // 1.2. Apply evolution rule for one step
    void evolve() {
        std::vector<bool> new_state(state.size());

        for (size_t i = 0; i < state.size(); i++) {
            // Get neighbors (with periodic boundary conditions)
            bool left = state[(i - 1 + state.size()) % state.size()];
            bool center = state[i];
            bool right = state[(i + 1) % state.size()];

            new_state[i] = applyRule(left, center, right);
        }

        state = new_state;
    }

    // Get current state
    std::vector<bool> getState() const {
        return state;
    }

    // Get state as hex string
    std::string getStateAsHex() const {
        std::stringstream ss;

        // Process bits in groups of 4 to create hex digits
        for (size_t i = 0; i < state.size(); i += 4) {
            int nibble = 0;
            for (int j = 0; j < 4 && (i + j) < state.size(); j++) {
                if (state[i + j]) {
                    nibble |= (1 << (3 - j));
                }
            }
            ss << std::hex << nibble;
        }

        return ss.str();
    }

    // Print state (for debugging)
    void printState() const {
        for (bool bit : state) {
            std::cout << (bit ? '1' : '0');
        }
        std::cout << std::endl;
    }

    // Set rule
    void setRule(uint32_t r) {
        rule = r;
    }
};

// 2.1. Hash function based on cellular automaton
std::string ac_hash(const std::string& input, uint32_t rule, size_t steps) {
    CellularAutomaton ca(rule);

    // 2.2. Convert input text to bits
    ca.init_state_from_string(input);

    // If input is too short, pad to at least 256 bits
    std::vector<bool> state = ca.getState();
    while (state.size() < 256) {
        // Pad with pattern based on current state
        for (size_t i = 0; i < state.size() && state.size() < 256; i++) {
            state.push_back(state[i]);
        }
    }
    ca.init_state(state);

    // 2.3. Evolve for specified number of steps
    for (size_t i = 0; i < steps; i++) {
        ca.evolve();
    }

    // Extract final 256 bits as hash
    state = ca.getState();
    std::vector<bool> hash_bits(256);

    // Use XOR folding if state is larger than 256 bits
    for (size_t i = 0; i < state.size(); i++) {
        hash_bits[i % 256] = hash_bits[i % 256] ^ state[i];
    }

    // Convert to hex string (256 bits = 64 hex characters)
    std::stringstream ss;
    for (size_t i = 0; i < 256; i += 4) {
        int nibble = 0;
        for (int j = 0; j < 4; j++) {
            if (hash_bits[i + j]) {
                nibble |= (1 << (3 - j));
            }
        }
        ss << std::hex << nibble;
    }

    return ss.str();
}

// Test function to verify different inputs give different outputs
void test_ac_hash() {
    std::cout << "=== Testing AC Hash Function ===" << std::endl;

    std::string input1 = "Hello World";
    std::string input2 = "Hello World!";
    std::string input3 = "hello world";

    std::string hash1 = ac_hash(input1, 30, 128);
    std::string hash2 = ac_hash(input2, 30, 128);
    std::string hash3 = ac_hash(input3, 30, 128);

    std::cout << "Input: '" << input1 << "'" << std::endl;
    std::cout << "Hash:  " << hash1 << std::endl << std::endl;

    std::cout << "Input: '" << input2 << "'" << std::endl;
    std::cout << "Hash:  " << hash2 << std::endl << std::endl;

    std::cout << "Input: '" << input3 << "'" << std::endl;
    std::cout << "Hash:  " << hash3 << std::endl << std::endl;

    // 2.4. Verify different inputs produce different outputs
    std::cout << "Hash1 == Hash2? " << (hash1 == hash2 ? "YES (ERROR!)" : "NO (CORRECT)") << std::endl;
    std::cout << "Hash1 == Hash3? " << (hash1 == hash3 ? "YES (ERROR!)" : "NO (CORRECT)") << std::endl;
}

#endif // CELLULAR_AUTOMATON_H