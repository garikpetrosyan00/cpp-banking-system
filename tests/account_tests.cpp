#include "../include/account.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

TEST(AccountTest, InitialBalance) {
  Account acc(1, 100.0);
  EXPECT_EQ(acc.get_balance(), 100.0);
  EXPECT_EQ(acc.get_id(), 1);
}

TEST(AccountTest, Deposit) {
  Account acc(1, 100.0);
  acc.deposit(50.0);
  EXPECT_EQ(acc.get_balance(), 150.0);
}

TEST(AccountTest, WithdrawSuccess) {
  Account acc(1, 100.0);
  bool success = acc.withdraw(40.0);
  EXPECT_TRUE(success);
  EXPECT_EQ(acc.get_balance(), 60.0);
}

TEST(AccountTest, WithdrawFail) {
  Account acc(1, 100.0);
  bool success = acc.withdraw(150.0);
  EXPECT_FALSE(success);
  EXPECT_EQ(acc.get_balance(), 100.0);
}

TEST(AccountTest, ConcurrentDeposit) {
  Account acc(1, 0.0);
  int num_threads = 10;
  int deposits_per_thread = 1000;
  double amount = 1.0;

  std::vector<std::thread> threads;
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back([&]() {
      for (int k = 0; k < deposits_per_thread; ++k) {
        acc.deposit(amount);
      }
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  EXPECT_DOUBLE_EQ(acc.get_balance(),
                   num_threads * deposits_per_thread * amount);
}
