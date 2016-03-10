#include <stdio.h>
#include <string.h>
#include "trackfindrange.h"
#include "range.h"

TrackFindRange::TrackFindRange():
   z1_("track_find_range_z1", -200, 200, 10),
   z2_("track_find_range_z2", -200, 200, 10),
   t0_("track_find_range_t0", -900, -800, 1),
   dr_("track_find_range_dr", 0, 8.0),
   nt_("track_find_range_nt", 1, 4)
{
}

TrackFindRange::TrackFindRange(Range<double> z1, Range<double> z2, Range<double> t0, Range<double> drift_radius, Range<int> num_tracks)
{
   z1_ = z1;
   z2_ = z2;
   t0_ = t0;
   dr_ = drift_radius;
   nt_ = num_tracks;
}

Range<double>& TrackFindRange::z1()
{
   return z1_;
}

Range<double>& TrackFindRange::z2()
{
   return z2_;
}

Range<double>& TrackFindRange::t0()
{
   return t0_;
}

Range<double>& TrackFindRange::dr()
{
   return dr_;
}

Range<int>& TrackFindRange::nt()
{
   return nt_;
}
void TrackFindRange::PrintTrackFindRange()
{
   z1_.PrintRange();
   z2_.PrintRange();
   t0_.PrintRange();
   dr_.PrintRange();
   nt_.PrintRange();
}
