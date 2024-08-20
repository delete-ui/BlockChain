#include "Classes.h"
#include <iostream>

using namespace std;

int main() 
{
    setlocale(LC_ALL, "RU");
    srand(static_cast<unsigned int>(time(nullptr)));

    BlockChain bc;

    // Creating and deploying a smart contract
    auto balanceContract = make_unique<BalanceContract>();
    balanceContract->addBalance("Alice", 100);
    balanceContract->addBalance("Bob", 50);
    bc.deployContract("BalanceContract", std::move(balanceContract));

    // We are fulfilling a smart contract
    bc.executeContract("BalanceContract");

    // Mining blocks with transactions
    vector<Transaction> transactions1 =
    {
        Transaction("Alice", "Bob", 10),
        Transaction("Charlie", "Dave", 20),
    };
    vector<Transaction> transactions2 = 
    {
        Transaction("Eve", "Frank", 30),
        Transaction("Grace", "Hank", 40),
    };

    bc.addBlock(transactions1);
    bc.addBlock(transactions2);

    bc.printChain();

    // We receive and withdraw the balance
    auto contracts = dynamic_cast<BalanceContract*>(bc.contracts["BalanceContract"].get());
    cout << "Баланс Alice: " << contracts->getBalance("Alice") << endl;
    cout << "Баланс Bob: " << contracts->getBalance("Bob") << endl;

    return 0;
}