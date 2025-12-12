#ifndef BANK_HPP
#define BANK_HPP

#include "account.hpp"
#include "transaction.hpp"
#include <map>
#include <memory>
#include <mutex>
#include <vector>

class Bank {
public:
  Bank();

  // Creates an account and returns its ID
  int create_account(double initial_balance);

  // Thread-safe operations
  void deposit(int account_id, double amount);
  bool withdraw(int account_id, double amount);
  bool transfer(int from_id, int to_id, double amount);

  std::shared_ptr<Account> get_account(int id);
  void print_summary();

private:
  std::map<int, std::shared_ptr<Account>> accounts;
  std::vector<Transaction> transactions;
  mutable std::mutex bank_mtx; // Protects accounts map and transaction log

  int next_account_id = 1;

  void log_transaction(const Transaction &t);
};

#endif // BANK_HPP
