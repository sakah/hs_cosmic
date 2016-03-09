#include <stdio.h>
#include <string.h>
#include "range.h"

Range::Range(const char* name, double min, double max, double step)
{
   strcpy(name_, name);
   min_ = min;
   max_ = max;
   step_ = step;
}

Range::Range(const char* name, double min, double max)
   :Range(name, min, max, -1)
{
}

Range::Range()
{
   Range("NOT_SET", -1, -1, -1);
}

char* Range::GetName()
{
   return name_;
}

double Range::GetMin()
{
   return min_;
}

double Range::GetMax()
{
   return max_;
}

double Range::GetStep()
{
   return step_;
}

void Range::PrintRange()
{
   printf("Range %s min %8.2f max %8.2f step %8.2f\n", name_, min_, max_, step_);
}
