CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -lssl -lcrypto

all: merkle pow pos complete

merkle: main_merkle.cpp merkle_tree.h
	$(CXX) $(CXXFLAGS) -o merkle main_merkle.cpp $(LDFLAGS)

pow: main_pow.cpp proof_of_work.h
	$(CXX) $(CXXFLAGS) -o pow main_pow.cpp $(LDFLAGS)

pos: main_pos.cpp proof_of_stake.h proof_of_work.h
	$(CXX) $(CXXFLAGS) -o pos main_pos.cpp $(LDFLAGS)

complete: main_complete.cpp complete_blockchain.h
	$(CXX) $(CXXFLAGS) -o complete main_complete.cpp $(LDFLAGS)

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