cmake_minimum_required(VERSION 3.10)
project(banking_system VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# Define source files
set(SOURCES
    src/main.cpp
    src/account.cpp
    src/bank.cpp
    src/transaction.cpp
)

# Create executable
add_executable(banking_system ${SOURCES})

# Threading library is usually required for C++ threads
find_package(Threads REQUIRED)
target_link_libraries(banking_system Threads::Threads)

# --- Testing ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
    tests/account_tests.cpp
    tests/bank_tests.cpp
    src/account.cpp
    src/transaction.cpp
    src/bank.cpp
)

target_link_libraries(unit_tests
    GTest::gtest_main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
