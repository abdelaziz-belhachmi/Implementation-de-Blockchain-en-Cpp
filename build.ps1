# Build script for Blockchain project

Write-Host "Building Blockchain Project..." -ForegroundColor Green

# Compiler settings
$CXX = "g++"
$CXXFLAGS = "-std=c++11 -Wall -Wextra"
$INCLUDES = "-I1-ArbredeMerkle -I2-ProofofWork -I3-ProofofStake -I4-BlockchainComplete"

# Try to find OpenSSL installation
$OPENSSL_PATHS = @(
    "C:\Program Files\OpenSSL-Win64",
    "C:\Program Files\OpenSSL",
    "C:\OpenSSL-Win64",
    "C:\OpenSSL"
)

$OPENSSL_ROOT = $null
foreach ($path in $OPENSSL_PATHS) {
    if (Test-Path "$path\include\openssl\sha.h") {
        $OPENSSL_ROOT = $path
        Write-Host "Found OpenSSL at: $OPENSSL_ROOT" -ForegroundColor Green
        break
    }
}

if (-not $OPENSSL_ROOT) {
    Write-Host "ERROR: OpenSSL not found!" -ForegroundColor Red
    Write-Host "Please install OpenSSL from: https://slproweb.com/products/Win32OpenSSL.html" -ForegroundColor Yellow
    Write-Host "Or specify the path manually in the script." -ForegroundColor Yellow
    exit 1
}

# OpenSSL paths
$OPENSSL_INCLUDE = "-I`"$OPENSSL_ROOT\include`""
$OPENSSL_LIB = "-L`"$OPENSSL_ROOT\lib`""
$LDFLAGS = "-lssl -lcrypto"

# Combine all flags
$ALL_INCLUDES = "$INCLUDES $OPENSSL_INCLUDE"
$ALL_LDFLAGS = "$OPENSSL_LIB $LDFLAGS"

Write-Host "Include path: $ALL_INCLUDES" -ForegroundColor Gray
Write-Host "Library path: $ALL_LDFLAGS" -ForegroundColor Gray

# Build Merkle Tree
Write-Host "`nCompiling Merkle Tree..." -ForegroundColor Cyan
$cmd = "$CXX $CXXFLAGS $ALL_INCLUDES -o merkle.exe 1-ArbredeMerkle/merkle_tree.cpp $ALL_LDFLAGS"
Write-Host "Command: $cmd" -ForegroundColor Gray
Invoke-Expression $cmd
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Merkle Tree compiled successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Merkle Tree compilation failed" -ForegroundColor Red
}

# Build Proof of Work
Write-Host "`nCompiling Proof of Work..." -ForegroundColor Cyan
$cmd = "$CXX $CXXFLAGS $ALL_INCLUDES -o pow.exe 2-ProofofWork/proof_of_work.cpp $ALL_LDFLAGS"
Invoke-Expression $cmd
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Proof of Work compiled successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Proof of Work compilation failed" -ForegroundColor Red
}

# Build Proof of Stake
Write-Host "`nCompiling Proof of Stake..." -ForegroundColor Cyan
$cmd = "$CXX $CXXFLAGS $ALL_INCLUDES -o pos.exe 3-ProofofStake/proof_of_stake.cpp $ALL_LDFLAGS"
Invoke-Expression $cmd
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Proof of Stake compiled successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Proof of Stake compilation failed" -ForegroundColor Red
}

# Build Complete Blockchain
Write-Host "`nCompiling Complete Blockchain..." -ForegroundColor Cyan
$cmd = "$CXX $CXXFLAGS $ALL_INCLUDES -o complete.exe 4-BlockchainComplete/complete_blockchain.cpp $ALL_LDFLAGS"
Invoke-Expression $cmd
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