CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Wno-reorder
LDFLAGS = -lssl -lcrypto

# Include directories
INCLUDES = -I1-ArbredeMerkle -I2-ProofofWork -I3-ProofofStake -I4-BlockchainComplete -I5-CellularAutomatonHash

all: merkle pow pos complete ca_test ca_blockchain

merkle: 1-ArbredeMerkle/merkle_tree.cpp 1-ArbredeMerkle/merkle_tree.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o merkle 1-ArbredeMerkle/merkle_tree.cpp $(LDFLAGS)

pow: 2-ProofofWork/proof_of_work.cpp 2-ProofofWork/proof_of_work.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o pow 2-ProofofWork/proof_of_work.cpp $(LDFLAGS)

pos: 3-ProofofStake/proof_of_stake.cpp 3-ProofofStake/proof_of_stake.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o pos 3-ProofofStake/proof_of_stake.cpp $(LDFLAGS)

complete: 4-BlockchainComplete/complete_blockchain.cpp 4-BlockchainComplete/complete_blockchain.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o complete 4-BlockchainComplete/complete_blockchain.cpp $(LDFLAGS)

ca_test: 5-CellularAutomatonHash/test_cellular_automaton.cpp 5-CellularAutomatonHash/cellular_automaton.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o ca_test 5-CellularAutomatonHash/test_cellular_automaton.cpp

ca_blockchain: 5-CellularAutomatonHash/test_ca_blockchain.cpp 5-CellularAutomatonHash/blockchain_with_ca_hash.h 5-CellularAutomatonHash/cellular_automaton.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o ca_blockchain 5-CellularAutomatonHash/test_ca_blockchain.cpp $(LDFLAGS)

clean:
	rm -f merkle pow pos complete ca_test ca_blockchain

test: all
	@echo "Running Merkle Tree tests..."
	./merkle
	@echo ""
	@echo "Running Proof of Work tests..."
	./pow
	@echo ""
	@echo "Running Proof of Stake tests..."
	./pos
	@echo ""
	@echo "Running Complete Blockchain tests..."
	./complete
	@echo ""
	@echo "Running Cellular Automaton tests..."
	./ca_test
	@echo ""
	@echo "Running CA Blockchain Analysis..."
	./ca_blockchain

.PHONY: all clean test