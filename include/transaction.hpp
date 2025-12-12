#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>

enum class TransactionType { DEPOSIT, WITHDRAW, TRANSFER };

class Transaction {
public:
  Transaction(TransactionType type, int from_id, int to_id, double amount,
              const std::string &timestamp);

  // Getters
  TransactionType get_type() const;
  int get_from_id() const;
  int get_to_id() const;
  double get_amount() const;
  std::string get_timestamp() const;

  std::string to_string() const;

private:
  TransactionType type;
  int from_id;
  int to_id; // -1 if not applicable
  double amount;
  std::string timestamp;
};

#endif // TRANSACTION_HPP
