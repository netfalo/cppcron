class CronSegment {
private:
  int rangeStart;
  int rangeEnd;
  int stepValue;
public:
  CronSegment(int _rangeStart, int _rangeEnd, int _stepValue):
    rangeStart(_rangeStart),
    rangeEnd(_rangeEnd),
    stepValue(_stepValue)
  {
   
    
  }

  friend bool operator==(const CronSegment& lhs, const CronSegment& rhs);
};

inline bool operator==(const CronSegment& lhs, const CronSegment& rhs) {
  return lhs.rangeStart == rhs.rangeStart && lhs.rangeEnd == rhs.rangeEnd && lhs.stepValue == rhs.stepValue;
}

