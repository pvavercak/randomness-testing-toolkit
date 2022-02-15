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

TEST(Cnstr_InPlace, IndexDirect) {
  std::variant<int, std::string> v(std::in_place_index<0>, 42);
  EXPECT_EQ(42, std::get<0>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv(std::in_place_index<0>,
                                                   42);
    static_assert(42 == std::get<0>(cv), "");
  }
}

TEST(Cnstr_InPlace, IndexDirectDuplicate) {
  std::variant<int, int> v(std::in_place_index<0>, 42);
  EXPECT_EQ(42, std::get<0>(v));

  /* constexpr */ {
    constexpr std::variant<int, int> cv(std::in_place_index<0>, 42);
    static_assert(42 == std::get<0>(cv), "");
  }
}

TEST(Cnstr_InPlace, IndexConversion) {
  std::variant<int, std::string> v(std::in_place_index<1>, "42");
  EXPECT_EQ("42"s, std::get<1>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv(std::in_place_index<0>,
                                                   1.1);
    static_assert(1 == std::get<0>(cv), "");
  }
}

#if 0
TEST(Cnstr_InPlace, IndexConversionDuplicateRef) {
  std::ostringstream strm;
  std::variant<std::ostream &, std::ostream &> v(std::in_place_index<0>, strm);
  EXPECT_EQ(&strm, &std::get<0>(v));
}
#endif

TEST(Cnstr_InPlace, IndexInitializerList) {
  std::variant<int, std::string> v(std::in_place_index<1>, {'4', '2'});
  EXPECT_EQ("42"s, std::get<1>(v));
}

TEST(Cnstr_InPlace, TypeDirect) {
  std::variant<int, std::string> v(std::in_place_type<std::string>, "42"s);
  EXPECT_EQ("42"s, std::get<std::string>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv(std::in_place_type<int>,
                                                   42);
    static_assert(42 == std::get<int>(cv), "");
  }
}

#if 0
TEST(Cnstr_InPlace, TypeDirectRef) {
  int expected = 42;
  std::variant<int &, std::ostream &> v(std::in_place_type<int &>, expected);
  EXPECT_EQ(expected, std::get<int &>(v));
  EXPECT_EQ(&expected, &std::get<int &>(v));
}
#endif

TEST(Cnstr_InPlace, TypeConversion) {
  std::variant<int, std::string> v(std::in_place_type<int>, 42.5);
  EXPECT_EQ(42, std::get<int>(v));

  /* constexpr */ {
    constexpr std::variant<int, const char *> cv(std::in_place_type<int>,
                                                   42.5);
    static_assert(42 == std::get<int>(cv), "");
  }
}

#if 0
TEST(Cnstr_InPlace, TypeConversionRef) {
  std::ostringstream strm;
  std::variant<int, std::ostream &> v(std::in_place_type<std::ostream &>, strm);
  EXPECT_EQ(&strm, &std::get<std::ostream &>(v));
}
#endif

TEST(Cnstr_InPlace, TypeInitializerList) {
  std::variant<int, std::string> v(std::in_place_type<std::string>, {'4', '2'});
  EXPECT_EQ("42"s, std::get<std::string>(v));
}
