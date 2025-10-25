CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -lssl -lcrypto

# Include directories
INCLUDES = -I1-ArbredeMerkle -I2-ProofofWork -I3-ProofofStake -I4-BlockchainComplete

all: merkle pow pos complete

merkle: 1-ArbredeMerkle/merkle_tree.cpp 1-ArbredeMerkle/merkle_tree.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o merkle 1-ArbredeMerkle/merkle_tree.cpp $(LDFLAGS)

pow: 2-ProofofWork/proof_of_work.cpp 2-ProofofWork/proof_of_work.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o pow 2-ProofofWork/proof_of_work.cpp $(LDFLAGS)

pos: 3-ProofofStake/proof_of_stake.cpp 3-ProofofStake/proof_of_stake.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o pos 3-ProofofStake/proof_of_stake.cpp $(LDFLAGS)

complete: 4-BlockchainComplete/complete_blockchain.cpp 4-BlockchainComplete/complete_blockchain.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o complete 4-BlockchainComplete/complete_blockchain.cpp $(LDFLAGS)

clean:
	rm -f merkle pow pos complete

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

.PHONY: all clean test