#include "cppcron.hh"

#include <cassert>

vector<string> split(string str, string delimiter) {
  vector<string> segments = vector<string>();
  int startPos = 0;
  int endPos = 0;

  while((endPos = str.find(delimiter, startPos)) != -1) {
    string segment = str.substr(startPos, endPos - startPos);
    startPos = endPos + delimiter.length();
    segments.push_back(segment);
  }

  segments.push_back(str.substr(startPos));

  return segments;
}

CronSegment parseMinute(string pattern) {
  vector<string> tmp = split(pattern, "/");
  vector<string> range = split(tmp[0], "-");
  int rangeStart = 0;
  int rangeEnd = 0;
  if (range.size() == 2) {
    rangeStart = stoi(range[0]);
    rangeEnd = stoi(range[1]);
  } else {
    assert(range.size() == 1);

    if (range[0].compare("*") == 0) {
      rangeStart = 0;
      rangeEnd = 59;
    } else {
      rangeStart = rangeEnd = stoi(range[0]);
    }
  }

  int stepValue = 1;
  if (tmp.size() == 2) {
    stepValue = stoi(tmp[1]);
  }

  return CronSegment(rangeStart, rangeEnd, stepValue);
}


