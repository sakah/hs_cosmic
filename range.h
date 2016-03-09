#ifndef RANGE_H
#define RANGE_H

class Range
{
   public:
      Range(const char* name, double min, double max, double step);
      Range(const char* name, double min, double max);
      Range();
      char* GetName();
      double GetMin();
      double GetMax();
      double GetStep();

      void PrintRange();

   private:
      char name_[128];
      double min_;
      double max_;
      double step_;
};

#endif

