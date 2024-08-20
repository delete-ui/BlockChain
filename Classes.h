#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>

using namespace std;

//Declaring classes;

class SmartContract;
class BalanceContract;
class Transaction;
class StakeManager;
class Block;

//Description of classes;
class SmartContract 
{
public:
    virtual void execute() = 0;
    virtual ~SmartContract() = default;
};

class BalanceContract : public SmartContract
{
private:
    unordered_map<string,int> balances;

public:
    void addBalance(const std::string& user, int amount);
    int getBalance(const std::string& user);
    void execute() override {};

};

class Transaction 
{
public:
    Transaction(const std::string& from, const std::string& to, int amount);

private:
    std::string from;
    std::string to;
    int amount;
    time_t timestamp;

    friend class Block;
    friend class BlockChain;

};

class StakeManager
{
public:
    void addStake(const std::string& wallet, int amount);
    std::string selectValidator();
    int getTotalStake() const;

private:
    std::map<std::string, int> stakes;
    int totalStake = 0;

};

class Block 
{
public:
    Block(size_t index, std::vector<Transaction> transactions, std::string previousHash = "", std::string validator = "", int difficulty = 4);

private:
    std::string uniqueHash;
    std::string previousHash;
    std::vector<Transaction> transactions;
    time_t timestamp;
    size_t index;
    std::string validator;
    int difficulty;
    int nonce;

    std::string computeHash();
    std::string mineBlock();

    friend class BlockChain;

};

class BlockChain 
{
public:
    BlockChain();
    void printChain();
    void addBlock(const std::vector<Transaction>& transactions);
    void deployContract(const std::string& contractName, std::unique_ptr<SmartContract> contract);
    void executeContract(const std::string& contractName);
    unordered_map<string, unique_ptr<SmartContract>> contracts;

private:
    std::vector<Block> chain;
    StakeManager stakeManager;
    

};

#endif