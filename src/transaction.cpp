#include "transaction.hpp"
#include <sstream>

Transaction::Transaction(TransactionType type, int from_id, int to_id,
                         double amount, const std::string &timestamp)
    : type(type), from_id(from_id), to_id(to_id), amount(amount),
      timestamp(timestamp) {}

TransactionType Transaction::get_type() const { return type; }
int Transaction::get_from_id() const { return from_id; }
int Transaction::get_to_id() const { return to_id; }
double Transaction::get_amount() const { return amount; }
std::string Transaction::get_timestamp() const { return timestamp; }

std::string Transaction::to_string() const {
  std::stringstream ss;
  ss << "[" << timestamp << "] ";
  switch (type) {
  case TransactionType::DEPOSIT:
    ss << "DEPOSIT: Account " << from_id << " +$" << amount;
    break;
  case TransactionType::WITHDRAW:
    ss << "WITHDRAW: Account " << from_id << " -$" << amount;
    break;
  case TransactionType::TRANSFER:
    ss << "TRANSFER: Account " << from_id << " -> Account " << to_id << " $"
       << amount;
    break;
  }
  return ss.str();
}
