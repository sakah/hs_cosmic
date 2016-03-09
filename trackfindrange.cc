#include <stdio.h>
#include <string.h>
#include "trackfindrange.h"
#include "range.h"

TrackFindRange::TrackFindRange():
   z1_("z1", -200, 200, 10),
   z2_("z2", -200, 200, 10),
   t0_("t0", -900, -800, 1),
   drift_radius_("drift_raidus", 0, 8.0),
   num_tracks_("num_tracks", 1, 4)
{
}

TrackFindRange::TrackFindRange(Range z1, Range z2, Range t0, Range drift_radius, Range num_tracks)
{
   z1_           = z1;
   z2_           = z2;
   t0_           = t0;
   drift_radius_ = drift_radius;
   num_tracks_   = num_tracks;
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
   return drift_radius_;
}

Range& TrackFindRange::GetRangeNumTracks()
{
   return num_tracks_;
}
void TrackFindRange::PrintTrackFindRange()
{
   printf("----------\n");
   z1_.PrintRange();
   z2_.PrintRange();
   t0_.PrintRange();
   drift_radius_.PrintRange();
   num_tracks_.PrintRange();
   printf("----------\n");

}

void TrackFindRange::UpdateRange(const char* name, double min, double max, double step)
{
   if (strstr(name, "track_find_range_z1"))   z1_ = Range("z1", min, max, step);
   if (strstr(name, "track_find_range_z2"))   z2_ = Range("z2", min, max, step);
   if (strstr(name, "track_find_range_t0"))   t0_ = Range("t0", min, max, step);
   if (strstr(name, "track_find_range_drift_radius")) drift_radius_ = Range("drift_radius", min, max);
}

void TrackFindRange::UpdateRange(const char* name, int min, int max, int step)
{
   if (strstr(name, "track_find_num_tracks")) num_tracks_ = Range("num_tracks", min, max, step); 
}
