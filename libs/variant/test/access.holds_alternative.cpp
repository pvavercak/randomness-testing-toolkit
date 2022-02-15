// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

TEST(Access_HoldsAlternative, Typical) {
  std::variant<int, std::string> v(42);
  EXPECT_TRUE(std::holds_alternative<0>(v));
  EXPECT_FALSE(std::holds_alternative<1>(v));
  EXPECT_TRUE(std::holds_alternative<int>(v));
  EXPECT_FALSE(std::holds_alternative<std::string>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv(42);
    static_assert(std::holds_alternative<0>(cv), "");
    static_assert(!std::holds_alternative<1>(cv), "");
    static_assert(std::holds_alternative<int>(cv), "");
    static_assert(!std::holds_alternative<const char *>(cv), "");
  }
}
