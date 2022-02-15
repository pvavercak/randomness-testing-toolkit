// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <cassert>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

using namespace std::string_literals;

enum Qual { LRef, ConstLRef, RRef, ConstRRef };

struct get_qual {
  constexpr Qual operator()(int &) const { return LRef; }
  constexpr Qual operator()(const int &) const { return ConstLRef; }
  constexpr Qual operator()(int &&) const { return RRef; }
  constexpr Qual operator()(const int &&) const { return ConstRRef; }
};  // get_qual

TEST(Visit, MutVarMutType) {
  std::variant<int> v(42);
  // Check `v`.
  EXPECT_EQ(42, std::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, std::visit(get_qual(), v));
  EXPECT_EQ(RRef, std::visit(get_qual(), std::move(v)));
}

TEST(Visit, MutVarConstType) {
  std::variant<const int> v(42);
  EXPECT_EQ(42, std::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, std::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, std::visit(get_qual(), std::move(v)));
}

TEST(Visit, ConstVarMutType) {
  const std::variant<int> v(42);
  EXPECT_EQ(42, std::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, std::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, std::visit(get_qual(), std::move(v)));

  /* constexpr */ {
    constexpr std::variant<int> cv(42);
    static_assert(42 == std::get<int>(cv), "");
    // Check qualifier.
    static_assert(ConstLRef == std::visit(get_qual(), cv), "");
    static_assert(ConstRRef == std::visit(get_qual(), std::move(cv)), "");
  }
}

TEST(Visit, ConstVarConstType) {
  const std::variant<const int> v(42);
  EXPECT_EQ(42, std::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, std::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, std::visit(get_qual(), std::move(v)));

  /* constexpr */ {
    constexpr std::variant<const int> cv(42);
    static_assert(42 == std::get<const int>(cv), "");
    // Check qualifier.
    static_assert(ConstLRef == std::visit(get_qual(), cv), "");
    static_assert(ConstRRef == std::visit(get_qual(), std::move(cv)), "");
  }
}

TEST(Visit_Homogeneous, Double) {
  std::variant<int, std::string> v("hello"s), w("world!"s);
  EXPECT_EQ("hello world!"s,
            std::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv(101), cw(202), cx("helllo");
    struct add {
      constexpr int operator()(int lhs, int rhs) const { return lhs + rhs; }
      constexpr int operator()(int lhs, const char *) const { return lhs; }
      constexpr int operator()(const char *, int rhs) const { return rhs; }
      constexpr int operator()(const char *, const char *) const { return 0; }
    };  // add
    static_assert(303 == std::visit(add{}, cv, cw), "");
    static_assert(202 == std::visit(add{}, cw, cx), "");
    static_assert(101 == std::visit(add{}, cx, cv), "");
    static_assert(0 == std::visit(add{}, cx, cx), "");
  }
}

TEST(Visit_Homogeneous, Quintuple) {
  std::variant<int, std::string> v(101), w("+"s), x(202), y("="s), z(303);
  EXPECT_EQ("101+202=303"s,
            std::visit([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}

TEST(Visit_Heterogeneous, Double) {
  std::variant<int, std::string> v("hello"s);
  std::variant<double, const char *> w("world!");
  EXPECT_EQ("hello world!"s,
            std::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));
}

TEST(Visit_Heterogenous, Quintuple) {
  std::variant<int, double> v(101);
  std::variant<const char *> w("+");
  std::variant<bool, std::string, int> x(202);
  std::variant<char, std::string, const char *> y('=');
  std::variant<long, short> z(303L);
  EXPECT_EQ("101+202=303"s,
            std::visit([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}
