#include "gtest/gtest.h"

#include "../src/cppcron.hh"


TEST(split, split1) {
  vector<string> actual = split("ap ple", " ");
  vector<string> expected = {"ap", "ple"};
  EXPECT_EQ(actual, expected);
}

TEST(split, split2) {
  vector<string> actual = split("ap ple ", " ");
  vector<string> expected = {"ap", "ple", ""};
  EXPECT_EQ(actual, expected);
}

TEST(split, split3) {
  vector<string> actual = split("ap  ple ", "  ");
  vector<string> expected = {"ap", "ple "};
  EXPECT_EQ(actual, expected);
}

TEST(split, cron) {
  vector<string> actual = split("*/15 7-22 * * *", " ");
  vector<string> expected = {"*/15", "7-22", "*", "*", "*"};
  EXPECT_EQ(actual, expected);
}

TEST(split, no_delimiter) {
  vector<string> actual = split("apple", " ");
  vector<string> expected = {"apple"};
  EXPECT_EQ(actual, expected);
}

TEST(parseMinute, star) {
  CronSegment actual = parseMinute("*");
  CronSegment expected = CronSegment(0, 59, 1);
  EXPECT_EQ(actual, expected);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
