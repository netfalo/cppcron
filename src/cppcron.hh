#include <string>

#include <ctime>
#include <vector>
#include <map>

namespace cron {

  std::vector<std::string> split(std::string str, std::string delimiter) {
    std::vector<std::string> segments = std::vector<std::string>();
    int startPos = 0;
    int endPos = 0;

    while((endPos = str.find(delimiter, startPos)) != -1) {
      std::string segment = str.substr(startPos, endPos - startPos);
      startPos = endPos + delimiter.length();
      segments.push_back(segment);
    }

    segments.push_back(str.substr(startPos));

    return segments;
  }

  std::vector<std::string> splitList(std::string segment) {
    return split(segment, ",");
  }

  bool in_cron_range(int n, std::vector<std::string> segment, int minValue, int maxValue) {
    for(std::vector<std::string>::iterator it = segment.begin(); it < segment.end(); it++) {
      std::vector<std::string> splitted_range = split(*it, "/");
      int step_value = 1;
      int lower_limit = minValue;
      int upper_limit = maxValue;
      if (splitted_range.size() == 2) {
        step_value = std::stoi(splitted_range[1]);
      }

      std::vector<std::string> ranges = split(splitted_range[0], "-");
      if (ranges.size() == 2) {
        lower_limit = stoi(ranges[0]);
        upper_limit = stoi(ranges[1]);
        if (n >= lower_limit && n <= upper_limit && n % step_value == 0) {
          return true;
        }
      } else if (ranges.size() == 1 && n >= lower_limit && n <= upper_limit) {
        if (ranges[0].compare("*") == 0 && n % step_value == 0) {
          return true;
        } else if (ranges[0].compare("*") != 0 && stoi(ranges[0]) == n) {
          return true;
        }
      }
    }
    
    return false;
  }

  std::vector<std::string> split_list(std::string segment) {
    return split(segment, ",");
  }

  bool is_good_minute(tm* cal, std::string segment) {
    if (segment.compare("*") == 0) {
      return true;
    }
    return in_cron_range(cal->tm_min, split_list(segment), 0, 59);
  }

  bool is_good_hour(tm* cal, std::string segment) {
    if (segment.compare("*") == 0) {
      return true;
    }
    return in_cron_range(cal->tm_hour, split_list(segment), 0, 23);
  }

  bool is_good_mday(tm* cal, std::string mday) {
    if (mday.compare("*") == 0) {
      return true;
    } else {
      return in_cron_range(cal->tm_mday, split_list(mday), 1, 31);
    }
  }

  bool is_good_wday(tm* cal, std::string wday) {
    if (wday.compare("*") == 0) {
      return true;
    } else {
      return in_cron_range(cal->tm_wday, split_list(wday), 0, 6);
    }
  }
  
  bool is_good_month(tm* cal, std::string segment) {
    if (segment.compare("*") == 0) {
      return true;
    }
    return in_cron_range(cal->tm_mon, split_list(segment), 0, 11);
  }

  bool is_cal_in_cron_expression(tm* cal, std::string expression) {
    std::vector<std::string> segments = split(expression, " ");
    assert(segments.size() == 5);
    return is_good_minute(cal, segments[0]) &&
      is_good_hour(cal, segments[1]) &&
      is_good_mday(cal, segments[2]) &&
      is_good_month(cal, segments[3]) &&
      is_good_wday(cal, segments[4]);
  }

  tm* next_year(tm* cal) {
    cal->tm_year += 1;
    return cal;
  }

  tm* next_month(tm* cal) {
    if (cal->tm_mon == 11) {
      cal->tm_mon = 0;
      cal = next_year(cal);
    } else {
      cal->tm_mon += 1;
    }
    cal->tm_mday = 1;
    cal->tm_hour = 0;
    cal->tm_min = 0;
    return cal;
  }

  bool is_leap_year(int year) {
    if (year % 4 != 0) return false;
    else if (year % 100 != 0) return true;
    else if (year % 400 != 0) return false;
    else return true;
  }

  tm* next_day(tm* cal) {
    if ((cal->tm_mon == 0 || cal->tm_mon == 2 || cal->tm_mon == 4 || cal->tm_mon == 6 || cal->tm_mon == 7 || cal->tm_mon == 9 || cal->tm_mon == 11) && cal->tm_mday == 31) {
      cal->tm_mday = 1;
      cal->tm_wday = (cal->tm_wday + 1) % 7;
      cal = next_month(cal);
    } else if ((cal->tm_mon == 3 || cal->tm_mon == 5 || cal->tm_mon == 8 || cal->tm_mon == 9) && cal->tm_mday == 30) {
      cal->tm_mday = 1;
      cal->tm_wday = (cal->tm_wday + 1) % 7;
      cal = next_month(cal);
    } else if (cal->tm_mon == 1 && cal->tm_mday == 28 && !is_leap_year(cal->tm_year)) {
      cal->tm_mday = 1;
      cal->tm_wday = (cal->tm_wday + 1) % 7;
      cal = next_month(cal);
    } else if (cal->tm_mon == 1 && cal->tm_mday == 29 && is_leap_year(cal->tm_year)) {
      cal->tm_mday = 1;
      cal->tm_wday = (cal->tm_wday + 1) % 7;
      cal = next_month(cal);
    } else {
      cal->tm_mday += 1;
      cal->tm_wday = (cal->tm_wday + 1) % 7;
    }

    cal->tm_hour = 0;
    cal->tm_min = 0;

    return cal;
  }

  tm* next_hour(tm* cal) {
    if (cal->tm_hour == 24) {
      cal->tm_hour = 0;
      cal = next_day(cal);
    } else {
      cal->tm_hour += 1;
    }

    cal->tm_min = 0;

    return cal;
  }

  tm* next_minute(tm* cal) {
    if (cal->tm_min == 60) {
      cal->tm_min = 0;
      cal = next_hour(cal);
    } else {
      cal->tm_min += 1;
    }
    return cal;
  }

  time_t next(std::string expression, time_t after) {
    std::vector<std::string> segments = split(expression, " ");
    std::tm* next = localtime(&after);
    next->tm_sec = 0;

    next = next_minute(next);
    while (true) {
      if (!is_good_month(next, segments[3])) {
        next = next_month(next);
      } else if (!is_good_mday(next, segments[2])) {
        next = next_day(next);
      } else if (!is_good_wday(next, segments[4])) {
        next = next_day(next);
      } else if (!is_good_hour(next, segments[1])) {
        next = next_hour(next);
      } else if (!is_good_minute(next, segments[0])) {
        next = next_minute(next);
      } else {
        break;
      }
    }

    return mktime(next);
  }
}
