#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <mutex>

class Account {
public:
  Account(int id, double balance);

  // Thread-safe methods
  void deposit(double amount);
  bool withdraw(double amount);
  bool transfer_to(Account &target, double amount);
  double get_balance() const;
  int get_id() const; // No lock needed as ID is constant after creation

  // Expose mutex for external locking logic (like Bank::transfer)
  // Alternatively, we could make Bank a friend, but public mutex/lock method is
  // cleaner for std::lock
  std::mutex &get_mutex() const;

private:
  int id;
  double balance;
  mutable std::mutex mtx; // mutable allows locking in const methods
};

#endif // ACCOUNT_HPP
