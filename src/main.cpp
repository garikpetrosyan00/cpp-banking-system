#include "bank.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

// Function to simulate random banking operations
void simulate_user(Bank &bank, int my_id, int other_id, int iterations) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> op_dist(0, 2); // 0: dep, 1: with, 2: trans
  std::uniform_int_distribution<> amount_dist(10, 100);
  std::uniform_int_distribution<> sleep_dist(1, 10);

  for (int i = 0; i < iterations; ++i) {
    int op = op_dist(gen);
    double amount = amount_dist(gen);

    if (op == 0) {
      bank.deposit(my_id, amount);
    } else if (op == 1) {
      bank.withdraw(my_id, amount);
    } else {
      bank.transfer(my_id, other_id, amount);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(gen)));
  }
}

int main() {
  std::cout << "Initing Banking System..." << std::endl;
  Bank bank;

  // Create accounts
  int acc1 = bank.create_account(1000.0);
  int acc2 = bank.create_account(1000.0);
  int acc3 = bank.create_account(1000.0);
  int acc4 = bank.create_account(1000.0);

  std::cout << "Accounts created. Starting simulation..." << std::endl;

  // Create threads simulating users
  std::vector<std::thread> threads;

  // Each thread represents activity initiated by a user or targeting a user
  // To stress test high contention, we'll swap funds between couples
  threads.emplace_back(simulate_user, std::ref(bank), acc1, acc2, 100);
  threads.emplace_back(simulate_user, std::ref(bank), acc2, acc1, 100);
  threads.emplace_back(simulate_user, std::ref(bank), acc3, acc4, 100);
  threads.emplace_back(simulate_user, std::ref(bank), acc4, acc3, 100);

  // Join threads
  for (auto &t : threads) {
    t.join();
  }

  std::cout << "Simulation complete." << std::endl;
  bank.print_summary();

  return 0;
}
