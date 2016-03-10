#ifndef RANGE_H
#define RANGE_H

#include <iostream>

template <typename T>
class Range
{
   public:
      Range(const char* name, T min, T max, T step)
      {
         strcpy(name_, name);
         min_ = min;
         max_ = max;
         step_ = step;
      };

      Range(const char* name, T min, T max)
      {
         strcpy(name_, name);
         min_ = min;
         max_ = max;
         step_ = -1;
      };

      Range()
      {
         strcpy(name_, "NOT_SET");
         min_ = -1;
         max_ = -1;
         step_ = -1;
      };
      void Update(T min, T max, T step)
      {
         min_ = min;
         max_ = max;
         step_ = step;
      };

      char* GetName() { return name_; };
      T GetMin() { return min_; };
      T GetMax() { return max_; };
      T GetStep() { return step_; };

      void PrintRange()
      {
         //printf("Range %s min %8.2f max %8.2f step %8.2f\n", name_, min_, max_, step_);
         std::cout << name_ << " " << min_ << " " << max_ << " " << step_ << std::endl;
      };

   private:
      char name_[128];
      T min_;
      T max_;
      T step_;
};

#endif

