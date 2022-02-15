// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <string>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Cnstr_Move, Value) {
  // `v`
  std::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, std::get<std::string>(v));
  // `w`
  std::variant<int, std::string> w(std::move(v));
  EXPECT_EQ("hello"s, std::get<std::string>(w));
  // Check `v`
  EXPECT_TRUE(std::get<std::string>(v).empty());

  /* constexpr */ {
    // `cv`
    constexpr std::variant<int, const char *> cv(42);
    static_assert(42 == std::get<int>(cv), "");
    // `cw`
    constexpr std::variant<int, const char *> cw(std::move(cv));
    static_assert(42 == std::get<int>(cw), "");
  }
}

#if 0
TEST(Cnstr_Move, Ref) {
  std::string s = "hello"s;
  // `v`
  std::variant<int &, std::string &> v(s);
  EXPECT_EQ("hello"s, std::get<std::string &>(v));
  EXPECT_EQ(&s, &std::get<std::string &>(v));
  // `w`
  std::variant<int &, std::string &> w(std::move(v));
  EXPECT_EQ("hello"s, std::get<std::string &>(w));
  EXPECT_EQ(&s, &std::get<std::string &>(w));
  // Check `v`
  EXPECT_EQ("hello"s, std::get<std::string &>(v));
  EXPECT_EQ(&s, &std::get<std::string &>(v));
}
#endif

TEST(Cnstr_Move, ValuelessByException) {
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
  std::variant<int, move_thrower_t> w(std::move(v));
  EXPECT_TRUE(w.valueless_by_exception());
}
