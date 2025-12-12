#include "account.hpp"
#include <iostream>

Account::Account(int id, double balance) : id(id), balance(balance) {}

void Account::deposit(double amount) {
  std::lock_guard<std::mutex> lock(mtx);
  balance += amount;
}

bool Account::withdraw(double amount) {
  std::lock_guard<std::mutex> lock(mtx);
  if (balance >= amount) {
    balance -= amount;
    return true;
  }
  return false;
}

bool Account::transfer_to(Account &target, double amount) {
  if (this == &target)
    return false;

  // Deadlock-safe locking of both mutexes
  std::lock(this->mtx, target.mtx);
  std::lock_guard<std::mutex> lock1(this->mtx, std::adopt_lock);
  std::lock_guard<std::mutex> lock2(target.mtx, std::adopt_lock);

  if (this->balance >= amount) {
    this->balance -= amount;
    target.balance += amount;
    return true;
  }
  return false;
}

double Account::get_balance() const {
  std::lock_guard<std::mutex> lock(mtx);
  return balance;
}

int Account::get_id() const { return id; }

std::mutex &Account::get_mutex() const { return mtx; }
