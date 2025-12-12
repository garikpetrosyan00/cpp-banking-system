# multithreaded-cpp-banking-system

A high-performance, thread-safe banking system simulation written in C++. This project demonstrates modern C++ concurrency patterns, deadlock prevention, and clean architecture.

## Features

- **Thread-safe Account Operations**: Deposit, Withdraw, and Transfer operations are fully synchronized using `std::mutex`.
- **Deadlock Prevention**: Uses `std::lock` algorithm to safely lock multiple accounts during transfers.
- **Transaction Logging**: Thread-safe global transaction log.
- **Simulation**: Includes a multi-threaded simulation of concurrent user activity.

## Requirements

- C++17 compliant compiler (GCC, Clang, MSVC)
- CMake 3.10+

## Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

After building, run the executable to start the simulation:

```bash
./banking_system
```

The simulation will:
1. Initialize a bank with 4 accounts.
2. Spawn 4 concurrent threads simulating users performing random deposits, withdrawals, and transfers.
3. Output a summary of final balances and total transactions after completion.

## Testing

The project is integrated with **GoogleTest** for unit testing.

To run the tests:

```bash
cd build
ctest --output-on-failure
```

Or run the test executable directly:

```bash
./unit_tests
```

## Project Structure

- `src/account.cpp`: Core account logic with mutex protection.
- `src/bank.cpp`: Manages accounts and orchestration of transfers.
- `src/transaction.cpp`: Immutable transaction records.
- `src/main.cpp`: Simulation driver.
