#ifndef TRACK_FIND_RANGE_H
#define TRACK_FIND_RANGE_H

#include "range.h"

class TrackFindRange
{
   public:
      TrackFindRange();
      TrackFindRange(Range<double> z1, Range<double> z2, Range<double> t0, Range<double> drift_radius, Range<int> num_tracks);
      Range<double>& z1();
      Range<double>& z2();
      Range<double>& t0();
      Range<double>& dr();
      Range<int>& nt();

      void PrintTrackFindRange();

   private:
      Range<double> z1_;
      Range<double> z2_;
      Range<double> t0_;
      Range<double> dr_;
      Range<int> nt_;
};

#endif
