#include <stdio.h>
#include <string.h>
#include "trackfindrange.h"
#include "range.h"

TrackFindRange::TrackFindRange():
   z1_("z1", -200, 200, 10),
   z2_("z2", -200, 200, 10),
   t0_("t0", -900, -800, 1),
   dr_("dr", 0, 8.0),
   nt_("nt", 1, 4)
{
}

TrackFindRange::TrackFindRange(Range z1, Range z2, Range t0, Range drift_radius, Range num_tracks)
{
   z1_ = z1;
   z2_ = z2;
   t0_ = t0;
   dr_ = drift_radius;
   nt_ = num_tracks;
}

Range& TrackFindRange::GetRangeZ1()
{
   return z1_;
}

Range& TrackFindRange::GetRangeZ2()
{
   return z2_;
}

Range& TrackFindRange::GetRangeT0()
{
   return t0_;
}

Range& TrackFindRange::GetRangeDriftRadius()
{
   return dr_;
}

Range& TrackFindRange::GetRangeNumTracks()
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

void TrackFindRange::UpdateRange(const char* name, double min, double max, double step)
{
   if (strstr(name, "track_find_range_z1")) z1_ = Range("z1", min, max, step);
   if (strstr(name, "track_find_range_z2")) z2_ = Range("z2", min, max, step);
   if (strstr(name, "track_find_range_t0")) t0_ = Range("t0", min, max, step);
   if (strstr(name, "track_find_range_dt")) nt_ = Range("dr", min, max);
}

void TrackFindRange::UpdateRange(const char* name, int min, int max, int step)
{
   if (strstr(name, "track_find_num_tracks")) nt_ = Range("nt", min, max, step); 
}
