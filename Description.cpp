#include "Classes.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <openssl/sha.h>
#include <ctime>
#include <sstream>
#include <memory>
#include <iomanip>

using namespace std;

//Class Transaction 

Transaction::Transaction(const string& from, const string& to, int amount)
    : from(from), to(to), amount(amount), timestamp(time(nullptr)) {}

//Class Block

Block::Block(size_t index, vector<Transaction> transactions, string previousHash, string validator, int difficulty)
    : index(index), transactions(transactions), previousHash(previousHash), timestamp(time(nullptr)), validator(validator), difficulty(difficulty), nonce(0) {
    uniqueHash = mineBlock();
}
string Block::computeHash() {
    stringstream ss;
    ss << index << previousHash << static_cast<unsigned long>(timestamp) << validator << nonce;
    for (const auto& tx : transactions) {
        ss << tx.from << tx.to << tx.amount << static_cast<unsigned long>(tx.timestamp);
    }
    string input = ss.str();

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest);

    stringstream hashSS;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hashSS << hex << setw(2) << setfill('0') << static_cast<int>(digest[i]);
    }
    return hashSS.str();
}
string Block::mineBlock() {
    string hash;
    do {
        nonce++;
        hash = computeHash();
    } while (hash.substr(0, difficulty) != string(difficulty, '0'));
    return hash;
}

//Class BlockChain

BlockChain::BlockChain() {
    stakeManager.addStake("genesis", 1000);
    chain.emplace_back(Block(0, vector<Transaction>(), "0", "genesis"));
}
void BlockChain::addBlock(const vector<Transaction>& transactions) {
    string previousHash = chain.back().uniqueHash;
    size_t newIndex = chain.size();
    string validator = stakeManager.selectValidator();
    chain.emplace_back(Block(newIndex, transactions, previousHash, validator));
}
void BlockChain::printChain() {
    for (const auto& block : chain) {
        cout << "Index: " << block.index << endl;
        cout << "Timestamp: " << block.timestamp << endl;
        cout << "Validator: " << block.validator << endl;
        cout << "Previous Hash: " << block.previousHash << endl;
        cout << "Hash: " << block.uniqueHash << endl;
        for (const auto& tx : block.transactions) {
            cout << "Transaction - From: " << tx.from << ", To: " << tx.to << ", Amount: " << tx.amount << endl;
        }
        cout << endl;
    }
}
void BlockChain::deployContract(const std::string& contractName, std::unique_ptr<SmartContract> contract)
{
    contracts[contractName] = move(contract);
};
void BlockChain::executeContract(const std::string& contractName)
{
    if (contracts.find(contractName) != contracts.end())
    {
        contracts[contractName]->execute();
    }
    else
    {
        throw std::runtime_error("Contract not found");
    }
};

//Class StakeManager

void StakeManager::addStake(const string& wallet, int amount) {
    if (amount <= 0) {
        cerr << "Ошибка: Ставка должна быть положительным числом." << endl;
        return;
    }
    stakes[wallet] += amount;
    totalStake += amount;
}\
string StakeManager::selectValidator() {
    if (totalStake == 0) {
        throw runtime_error("No stakes available to select validator");
    }

    int randWeight = rand() % totalStake;
    int currentWeight = 0;
    for (const auto& stake : stakes) {
        currentWeight += stake.second;
        if (currentWeight >= randWeight) {
            return stake.first;
        }
    }

    return "undefined";
};
int StakeManager::getTotalStake() const {
    return totalStake;
};

//Class BalanceContract

void BalanceContract::addBalance(const string& user, int amount)
{
    balances[user] += amount;
};
int BalanceContract::getBalance(const string& user)
{
    auto it = balances.find(user);
    if (it != balances.end())
    {
        return it->second;
    }
    throw std::runtime_error("User not found");
}
