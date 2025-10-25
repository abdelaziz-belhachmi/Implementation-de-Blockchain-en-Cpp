//
// Created by abdelaziz on 10/25/2025.
//
#include <iostream>
#include "test_cellular_automaton.h"

// 1.3. Test that the automaton correctly reproduces the chosen rule
void test_rule30() {
    std::cout << "=== Testing Rule 30 ===" << std::endl;

    CellularAutomaton ca(30);

    // Small initial state: single 1 in the middle
    std::vector<bool> initial(15, false);
    initial[7] = true;

    ca.init_state(initial);

    std::cout << "Initial state:" << std::endl;
    ca.printState();

    std::cout << "\nEvolution for 5 steps:" << std::endl;
    for (int i = 0; i < 5; i++) {
        ca.evolve();
        ca.printState();
    }

    std::cout << std::endl;
}

void test_rule90() {
    std::cout << "=== Testing Rule 90 ===" << std::endl;

    CellularAutomaton ca(90);

    std::vector<bool> initial(15, false);
    initial[7] = true;

    ca.init_state(initial);

    std::cout << "Initial state:" << std::endl;
    ca.printState();

    std::cout << "\nEvolution for 5 steps:" << std::endl;
    for (int i = 0; i < 5; i++) {
        ca.evolve();
        ca.printState();
    }

    std::cout << std::endl;
}

void test_rule110() {
    std::cout << "=== Testing Rule 110 ===" << std::endl;

    CellularAutomaton ca(110);

    std::vector<bool> initial(15, false);
    initial[7] = true;

    ca.init_state(initial);

    std::cout << "Initial state:" << std::endl;
    ca.printState();

    std::cout << "\nEvolution for 5 steps:" << std::endl;
    for (int i = 0; i < 5; i++) {
        ca.evolve();
        ca.printState();
    }

    std::cout << std::endl;
}

int main() {
    std::cout << "CELLULAR AUTOMATON TESTS" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // Test different rules
    test_rule30();
    test_rule90();
    test_rule110();

    // Test hash function
    test_ac_hash();

    return 0;
}