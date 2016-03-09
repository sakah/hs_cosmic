#ifndef TRACK_FIND_RANGE_H
#define TRACK_FIND_RANGE_H

#include "range.h"

class TrackFindRange
{
   public:
      TrackFindRange();
      TrackFindRange(Range z1, Range z2, Range t0, Range drift_radius, Range num_tracks);
      Range& GetRangeZ1();
      Range& GetRangeZ2();
      Range& GetRangeT0();
      Range& GetRangeDriftRadius();
      Range& GetRangeNumTracks();

      void PrintTrackFindRange();

      void UpdateRange(const char* name, double min, double max, double step=-1.0);
      void UpdateRange(const char* name, int min, int max, int step=-1);

   private:
      Range z1_;
      Range z2_;
      Range t0_;
      Range drift_radius_;
      Range num_tracks_;
};

#endif
