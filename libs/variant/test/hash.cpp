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

TEST(Hash, Monostate) {
  std::variant<int, std::monostate, std::string> v(std::monostate{});
  // Construct hash function objects.
  std::hash<std::monostate> monostate_hash;
  std::hash<std::variant<int, std::monostate, std::string>> variant_hash;
  // Check the hash.
  EXPECT_NE(monostate_hash(std::monostate{}), variant_hash(v));
}

TEST(Hash, String) {
  std::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, std::get<std::string>(v));
  // Construct hash function objects.
  std::hash<std::string> string_hash;
  std::hash<std::variant<int, std::string>> variant_hash;
  // Check the hash.
  EXPECT_NE(string_hash("hello"s), variant_hash(v));
}
