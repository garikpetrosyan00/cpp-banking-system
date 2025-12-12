#include "../include/bank.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

TEST(BankTest, CreateAccount) {
  Bank bank;
  int id = bank.create_account(500.0);
  auto acc = bank.get_account(id);
  ASSERT_NE(acc, nullptr);
  EXPECT_EQ(acc->get_balance(), 500.0);
}

TEST(BankTest, TransferSuccess) {
  Bank bank;
  int acc1 = bank.create_account(100.0);
  int acc2 = bank.create_account(50.0);

  bool result = bank.transfer(acc1, acc2, 30.0);
  EXPECT_TRUE(result);
  EXPECT_EQ(bank.get_account(acc1)->get_balance(), 70.0);
  EXPECT_EQ(bank.get_account(acc2)->get_balance(), 80.0);
}

TEST(BankTest, TransferFailInsufficientFunds) {
  Bank bank;
  int acc1 = bank.create_account(10.0);
  int acc2 = bank.create_account(50.0);

  bool result = bank.transfer(acc1, acc2, 20.0);
  EXPECT_FALSE(result);
  EXPECT_EQ(bank.get_account(acc1)->get_balance(), 10.0);
  EXPECT_EQ(bank.get_account(acc2)->get_balance(), 50.0);
}

TEST(BankTest, ConcurrentTransfers) {
  Bank bank;
  int acc1 = bank.create_account(1000.0);
  int acc2 = bank.create_account(1000.0);

  int num_threads = 10;
  int transfers = 100;

  std::vector<std::thread> threads;
  // Threads transferring back and forth
  for (int i = 0; i < num_threads; ++i) {
    if (i % 2 == 0) {
      threads.emplace_back([&]() {
        for (int k = 0; k < transfers; ++k)
          bank.transfer(acc1, acc2, 1.0);
      });
    } else {
      threads.emplace_back([&]() {
        for (int k = 0; k < transfers; ++k)
          bank.transfer(acc2, acc1, 1.0);
      });
    }
  }

  for (auto &t : threads)
    t.join();

  double total = bank.get_account(acc1)->get_balance() +
                 bank.get_account(acc2)->get_balance();
  EXPECT_DOUBLE_EQ(total, 2000.0);
}

TEST(BankTest, DeadlockSafety) {
  Bank bank;
  int acc1 = bank.create_account(1000.0);
  int acc2 = bank.create_account(1000.0);

  // Thread A: 1 -> 2
  std::thread t1([&]() {
    for (int i = 0; i < 1000; ++i)
      bank.transfer(acc1, acc2, 1.0);
  });

  // Thread B: 2 -> 1
  std::thread t2([&]() {
    for (int i = 0; i < 1000; ++i)
      bank.transfer(acc2, acc1, 1.0);
  });

  t1.join();
  t2.join();

  // If we're here, no deadlock occurred.
  double total = bank.get_account(acc1)->get_balance() +
                 bank.get_account(acc2)->get_balance();
  EXPECT_DOUBLE_EQ(total, 2000.0);
}
