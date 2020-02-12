#include <string>
#include <iostream>
#include <vector>

#include "CronSegment.hh"

using namespace std;

vector<string> split(string str, string delimiter);

CronSegment parseMinute(string pattern);
