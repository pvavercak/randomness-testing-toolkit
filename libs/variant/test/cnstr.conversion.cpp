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

TEST(Cnstr_Conversion, Direct) {
  std::variant<int, std::string> v(42);
  EXPECT_EQ(42, std::get<int>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv(42);
    static_assert(42 == std::get<int>(cv), "");
  }
}

#if 0
TEST(Cnstr_Conversion, DirectRef) {
  int expected = 42;
  std::variant<int &, std::string &> v(expected);
  EXPECT_EQ(expected, std::get<int &>(v));
  EXPECT_EQ(&expected, &std::get<int &>(v));

  /* constexpr */ {
    static constexpr int expected = 42;
    constexpr std::variant<const int &> cv(expected);
    static_assert(expected == std::get<const int &>(cv), "");
    static_assert(&expected == &std::get<const int &>(cv), "");
  }
}
#endif

TEST(Cnstr_Conversion, DirectConversion) {
  std::variant<int, std::string> v("42");
  EXPECT_EQ("42"s, std::get<std::string>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv(1.1);
    static_assert(1 == std::get<int>(cv), "");
  }
}

#if 0
TEST(Cnstr_Conversion, DirectConversionRef) {
  std::ostringstream strm;
  std::variant<int &, std::ostream &> v(strm);
  EXPECT_EQ(&strm, &std::get<std::ostream &>(v));
}
#endif

TEST(Cnstr_Conversion, CopyInitialization) {
  std::variant<int, std::string> v = 42;
  EXPECT_EQ(42, std::get<int>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv = 42;
    static_assert(42 == std::get<int>(cv), "");
  }
}

#if 0
TEST(Cnstr_Conversion, CopyInitializationRef) {
  std::string expected = "42";
  std::variant<int &, std::string &> v = expected;
  EXPECT_EQ(expected, std::get<std::string &>(v));
  EXPECT_EQ(&expected, &std::get<std::string &>(v));
}
#endif

TEST(Cnstr_Conversion, CopyInitializationConversion) {
  std::variant<int, std::string> v = "42";
  EXPECT_EQ("42"s, std::get<std::string>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv = 1.1;
    static_assert(1 == std::get<int>(cv), "");
  }
}

#if 0
TEST(Cnstr_Conversion, CopyInitializationConversionRef) {
  std::ostringstream strm;
  std::variant<int &, std::ostream &> v = strm;
  EXPECT_EQ(&strm, &std::get<std::ostream &>(v));
}
#endif
