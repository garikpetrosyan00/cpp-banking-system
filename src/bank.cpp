#include "bank.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

Bank::Bank() {}

int Bank::create_account(double initial_balance) {
  std::lock_guard<std::mutex> lock(bank_mtx);
  int id = next_account_id++;
  accounts[id] = std::make_shared<Account>(id, initial_balance);
  return id;
}

std::shared_ptr<Account> Bank::get_account(int id) {
  std::lock_guard<std::mutex> lock(bank_mtx);
  if (accounts.find(id) != accounts.end()) {
    return accounts[id];
  }
  return nullptr;
}

// Helper to get current timestamp string
std::string get_current_time_str() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

void Bank::deposit(int account_id, double amount) {
  auto acc = get_account(account_id);
  if (acc) {
    acc->deposit(amount);
    log_transaction(Transaction(TransactionType::DEPOSIT, account_id, -1,
                                amount, get_current_time_str()));
  }
}

bool Bank::withdraw(int account_id, double amount) {
  auto acc = get_account(account_id);
  if (acc) {
    if (acc->withdraw(amount)) {
      log_transaction(Transaction(TransactionType::WITHDRAW, account_id, -1,
                                  amount, get_current_time_str()));
      return true;
    }
  }
  return false;
}

bool Bank::transfer(int from_id, int to_id, double amount) {
  auto from_acc = get_account(from_id);
  auto to_acc = get_account(to_id);

  if (!from_acc || !to_acc)
    return false;

  if (from_acc->transfer_to(*to_acc, amount)) {
    log_transaction(Transaction(TransactionType::TRANSFER, from_id, to_id,
                                amount, get_current_time_str()));
    return true;
  }
  return false;
}

void Bank::log_transaction(const Transaction &t) {
  std::lock_guard<std::mutex> lock(bank_mtx);
  transactions.push_back(t);
}

void Bank::print_summary() {
  std::lock_guard<std::mutex> lock(bank_mtx);
  std::cout << "\n=== BANK SUMMARY ===\n";
  std::cout << "Total Transactions: " << transactions.size() << "\n";
  std::cout << "Accounts:\n";
  for (const auto &pair : accounts) {
    std::cout << "ID: " << pair.first << " | Balance: $"
              << pair.second->get_balance() << "\n";
  }
  std::cout << "====================\n";
}
