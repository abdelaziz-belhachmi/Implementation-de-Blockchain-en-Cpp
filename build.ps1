# Build script for Blockchain project

Write-Host "Building Blockchain Project..." -ForegroundColor Green

# Compiler settings
$CXX = "g++"
$CXXFLAGS = "-std=c++11 -Wall -Wextra"
$INCLUDES = "-I1-ArbredeMerkle -I2-ProofofWork -I3-ProofofStake -I4-BlockchainComplete"
$LDFLAGS = "-lssl -lcrypto"

# Build Merkle Tree
Write-Host "`nCompiling Merkle Tree..." -ForegroundColor Cyan
& $CXX $CXXFLAGS.Split() $INCLUDES.Split() -o merkle.exe 1-ArbredeMerkle/merkle_tree.cpp $LDFLAGS.Split()
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Merkle Tree compiled successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Merkle Tree compilation failed" -ForegroundColor Red
}

# Build Proof of Work
Write-Host "`nCompiling Proof of Work..." -ForegroundColor Cyan
& $CXX $CXXFLAGS.Split() $INCLUDES.Split() -o pow.exe 2-ProofofWork/proof_of_work.cpp $LDFLAGS.Split()
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Proof of Work compiled successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Proof of Work compilation failed" -ForegroundColor Red
}

# Build Proof of Stake
Write-Host "`nCompiling Proof of Stake..." -ForegroundColor Cyan
& $CXX $CXXFLAGS.Split() $INCLUDES.Split() -o pos.exe 3-ProofofStake/proof_of_stake.cpp $LDFLAGS.Split()
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Proof of Stake compiled successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Proof of Stake compilation failed" -ForegroundColor Red
}

# Build Complete Blockchain
Write-Host "`nCompiling Complete Blockchain..." -ForegroundColor Cyan
& $CXX $CXXFLAGS.Split() $INCLUDES.Split() -o complete.exe 4-BlockchainComplete/complete_blockchain.cpp $LDFLAGS.Split()
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Complete Blockchain compiled successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Complete Blockchain compilation failed" -ForegroundColor Red
}

Write-Host "`n========================================" -ForegroundColor Green
Write-Host "Build complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host "`nTo run the programs, use:" -ForegroundColor Yellow
Write-Host "  .\merkle.exe" -ForegroundColor White
Write-Host "  .\pow.exe" -ForegroundColor White
Write-Host "  .\pos.exe" -ForegroundColor White
Write-Host "  .\complete.exe" -ForegroundColor White