// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

enum Qual { Ptr, ConstPtr, LRef, ConstLRef, RRef, ConstRRef };

constexpr Qual get_qual(int *) { return Ptr; }
constexpr Qual get_qual(const int *) { return ConstPtr; }
constexpr Qual get_qual(int &) { return LRef; }
constexpr Qual get_qual(const int &) { return ConstLRef; }
constexpr Qual get_qual(int &&) { return RRef; }
constexpr Qual get_qual(const int &&) { return ConstRRef; }

TEST(Access_GetIf, MutVarMutType) {
  std::variant<int> v(42);
  EXPECT_EQ(42, *std::get_if<int>(&v));
  // Check qualifier.
  EXPECT_EQ(Ptr, get_qual(std::get_if<int>(&v)));
}

#if 0
TEST(Access_GetIf, MutVarMutTypeRef) {
  int expected = 42;
  std::variant<int &> v(expected);
  EXPECT_EQ(expected, *std::get_if<int &>(&v));
  EXPECT_EQ(&expected, std::get_if<int &>(&v));
  // Check qualifier.
  EXPECT_EQ(Ptr, get_qual(std::get_if<int &>(&v)));
}
#endif

TEST(Access_GetIf, MutVarConstType) {
  std::variant<const int> v(42);
  EXPECT_EQ(42, *std::get_if<const int>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(std::get_if<const int>(&v)));
}

#if 0
TEST(Access_GetIf, MutVarConstTypeRef) {
  int expected = 42;
  std::variant<const int &> v(expected);
  EXPECT_EQ(expected, *std::get_if<const int &>(&v));
  EXPECT_EQ(&expected, std::get_if<const int &>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(std::get_if<const int &>(&v)));
}
#endif

TEST(Access_GetIf, ConstVarMutType) {
  const std::variant<int> v(42);
  EXPECT_EQ(42, *std::get_if<int>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(std::get_if<int>(&v)));

  /* constexpr */ {
    constexpr std::variant<int> cv(42);
    static_assert(42 == *std::get_if<int>(&cv), "");
    // Check qualifier.
    static_assert(ConstPtr == get_qual(std::get_if<int>(&cv)), "");
  }
}

#if 0
TEST(Access_GetIf, ConstVarMutTypeRef) {
  int expected = 42;
  const std::variant<int &> v(expected);
  EXPECT_EQ(expected, *std::get_if<int &>(&v));
  EXPECT_EQ(&expected, std::get_if<int &>(&v));
  // Check qualifier.
  EXPECT_EQ(Ptr, get_qual(std::get_if<int &>(&v)));
}
#endif

TEST(Access_GetIf, ConstVarConstType) {
  const std::variant<const int> v(42);
  EXPECT_EQ(42, *std::get_if<const int>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(std::get_if<const int>(&v)));

  /* constexpr */ {
    constexpr std::variant<const int> cv(42);
    static_assert(42 == *std::get_if<const int>(&cv), "");
    // Check qualifier.
    static_assert(ConstPtr == get_qual(std::get_if<const int>(&cv)), "");
  }
}

#if 0
TEST(Access_GetIf, ConstVarConstTypeRef) {
  int expected = 42;
  const std::variant<const int &> v(expected);
  EXPECT_EQ(expected, *std::get_if<const int &>(&v));
  EXPECT_EQ(&expected, std::get_if<const int &>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(std::get_if<const int &>(&v)));

  /* constexpr */ {
    static constexpr int expected = 42;
    constexpr std::variant<const int &> cv(expected);
    static_assert(42 == *std::get_if<const int &>(&cv), "");
    // Check qualifier.
    static_assert(ConstPtr == get_qual(std::get_if<const int &>(&cv)), "");
  }
}
#endif

TEST(Access_GetIf, MoveValuelessByException) {
  struct move_thrower_t {
    move_thrower_t() = default;
    move_thrower_t(const move_thrower_t &) = default;
    [[noreturn]] move_thrower_t(move_thrower_t &&) {
      throw std::runtime_error("");
    }
    move_thrower_t &operator=(const move_thrower_t &) = default;
    move_thrower_t &operator=(move_thrower_t &&) = default;
  };  // move_thrower_t
  std::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  EXPECT_TRUE(v.valueless_by_exception());
  EXPECT_EQ(nullptr, std::get_if<int>(&v));
  EXPECT_EQ(nullptr, std::get_if<move_thrower_t>(&v));
}
