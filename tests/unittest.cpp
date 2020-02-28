#include "gtest/gtest.h"

#include "../src/cppcron.hh"

using namespace std;
using namespace cron;

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

TEST(in_cron_range, star) {
  EXPECT_TRUE(in_cron_range(5, {"*"}, 0, 59));
}

TEST(in_cron_range, bigger_than_max) {
  EXPECT_FALSE(in_cron_range(61, {"*"}, 0, 59));
}

TEST(in_cron_range, star_with_step_value) {
  EXPECT_TRUE(in_cron_range(30, {"*/15"}, 0, 59));
}

TEST(in_cron_range, simple_range) {
  EXPECT_TRUE(in_cron_range(25, {"20-30"}, 0, 59));
}

TEST(in_cron_range, range_with_step_value) {
  EXPECT_TRUE(in_cron_range(25, {"20-30/5"}, 0, 59));
}

TEST(in_cron, all_wild_card) {
  time_t epoch = 1582738582;

  EXPECT_TRUE(is_cal_in_cron_expression(localtime(&epoch), "* * * * *"));
}

TEST(in_cron, all_wild_card_with_step) {
  time_t epoch = 1582738582;

  EXPECT_TRUE(is_cal_in_cron_expression(localtime(&epoch), "*/6 * * * *"));
}

TEST(in_cron, value_list) {
  time_t epoch = 1582738582;

  EXPECT_TRUE(is_cal_in_cron_expression(localtime(&epoch), "6,12,18,24,30,36,42,48,54 * * * *"));
}

TEST(next, with_fixed_value) {
  time_t epoch = 1582738642;
  time_t expected = 1585244160;
  time_t actual = cron::next("36 18 26 * *", epoch);

  EXPECT_EQ(expected, actual);
}

TEST(next, with_fixed_value_one_year) {
  time_t epoch = 1582738642;
  time_t expected = 1614360960;
  time_t actual = cron::next("36 18 26 1 *", epoch);

  EXPECT_EQ(expected, actual);
}

TEST(next, with_fixed_value_fifteen_minutes) {
  time_t epoch = 1582738642;
  time_t expected = 1582739100;
  time_t actual = cron::next("*/15 * * * *", epoch);

  EXPECT_EQ(expected, actual);
}

TEST(next, with_fixed_wday) {
  time_t epoch = 1582738642;
  time_t expected = 1583103600;
  time_t actual = cron::next("*/15 * * * 1", epoch);

  EXPECT_EQ(expected, actual);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
