#include "trackfinder.h"
#include <time.h>

TrackFinder::TrackFinder()
{
   ClearTracks();

   max_r_ = 10.0; // 10.0 mm
   min_z_ = -400;
   max_z_ = +400;
   max_tracks_ = 4;

   min_z1_ = 0;
   min_z2_ = 0;
   min_t0_ = 0;

   chi2_of_small_track_ = 100;
}

void TrackFinder::ClearTracks()
{
   num_tracks_ = 0;
   min_itrack_ = -1;

   for (int itrack=0; itrack<MAX_TRACK; itrack++) {
      tracks_[itrack].ClearFitFlag();
   }

   num_small_tracks_ = 0;
   for (int itrack=0; itrack<MAX_TRACK; itrack++) {
      small_tracks_[itrack].ClearFitFlag();
   }

}

int TrackFinder::SetTracks(Chamber& chamber, XTcurve& xt, bool include_outer_guard_layer)
{
   if (include_outer_guard_layer) {
      return SetTracks_with_outer_guard_layer(chamber, xt);
   } 
   return SetTracks_without_guard_layers(chamber, xt);
}


double TrackFinder::FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z1, double z2)
{
   double min_chi2 = 1e10;
   WireMap& wiremap = chamber.GetWireMap();
   int ret = MakeTangents(chamber, xt, cid1, cid2, z1, z2);
   if (ret==0) {
      printf("could not make tangents\n");
      return min_chi2;
   }

   for (int itrack=0; itrack<num_tracks_; itrack++) {
      Line& min_tangent = tracks_[itrack].GetMinTangent(wiremap, xt);
      double chi2 = tracks_[itrack].GetChi2OfLine(wiremap, xt, min_tangent);
      if (chi2 < min_chi2) {
         min_chi2 = chi2;
         min_itrack_ = itrack;
      }
   }
   return min_chi2;
}

double TrackFinder::FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z_step)
{
   WireMap& wiremap = chamber.GetWireMap();

   double min_chi2 = 1e10;
   int min_itrack = 0;
   int num_z = (max_z_ - min_z_)/z_step;
   for (int iz1=0; iz1<num_z; iz1++) {
      for (int iz2=0; iz2<num_z; iz2++) {
         double z1 = min_z_ + iz1*z_step;
         double z2 = min_z_ + iz2*z_step;
         bool found = FindBestTrack(chamber, xt, cid1, cid2, z1, z2);
         if (!found) {
            return false;
         }
         if (g_debug_trackfinder) {
            printf("z1 %f z2 %f min_itrack_ %d\n", z1, z2, min_itrack_);
         }
         Line& min_tangent = GetBestTrack().GetMinTangent(wiremap, xt);
         double chi2 = GetBestTrack().GetChi2OfLine(wiremap, xt, min_tangent);
         if (chi2 < min_chi2) {
            min_chi2 = chi2;
            min_itrack = min_itrack_;
            min_z1_ = z1;
            min_z2_ = z2;
         }
         if (g_debug_trackfinder) {
            printf("z1 %f z2 %f chi2 %f min_chi2 %f min_itrack %d min_z1_ %f min_z2_ %f\n", z1, z2, chi2, min_chi2, min_itrack, min_z1_, min_z2_);
         }
      }
   }
   min_itrack_ = min_itrack;

   Track& min_track = tracks_[min_itrack_];
   min_track.MakeTangents(wiremap, xt, cid1, cid2, min_z1_, min_z2_);
   min_track.SetHitZWithMinTangent(wiremap, xt);
   return min_chi2;
}
double TrackFinder::FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z_step, double t0_min, double t0_max, double t0_step)
{
   double min_chi2 = 1e10;
   int num_t0 = (t0_max - t0_min)/t0_step;
   for (int it0=0; it0<num_t0; it0++) {
      double t0 = t0_min + it0*t0_step;
      chamber.SetT0(t0, t0);
      double chi2 = FindBestTrack(chamber, xt, cid1, cid2, z_step);
      if (chi2<min_chi2) {
         min_t0_ = t0;
         min_chi2 = chi2;
      }
      if (chi2<chi2_of_small_track_) {
         printf("t0 %f min_z1 %f min_z2 %f\n", t0, min_z1_, min_z2_ );
      }
   }
   return min_chi2;
}

int TrackFinder::MakeTangents(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z1, double z2)
{
   WireMap& wiremap = chamber.GetWireMap();

   for (int itrack=0; itrack<num_tracks_; itrack++) {
      int ret = tracks_[itrack].MakeTangents(wiremap, xt, cid1, cid2, z1, z2);
      if (ret==0) {
         return 0;
      }
   }
   return 1;
}

Track& TrackFinder::GetBestTrack()
{
   return tracks_[min_itrack_];
}

int TrackFinder::GetNumTracks()
{
   return num_tracks_;
}

void TrackFinder::PrintTracks(WireMap& wiremap, XTcurve& xt)
{
   printf("num_tracks %d\n", num_tracks_);
   for (int itrack=0; itrack<num_tracks_; itrack++) {
      printf("--- itrack %d ---\n", itrack);
      tracks_[itrack].PrintTangents(wiremap, xt);
      tracks_[itrack].PrintHits(xt);
   }
}

void TrackFinder::SetMaxRadius(double max_r)
{
   max_r_ = max_r;
}

void TrackFinder::SetMinZ(double min_z)
{
   min_z_ = min_z;
}

void TrackFinder::SetMaxZ(double max_z)
{
   max_z_ = max_z;
}

void TrackFinder::SetMinZ1(double min_z1)
{
   min_z1_ = min_z1;
}

void TrackFinder::SetMinZ2(double min_z2)
{
   min_z2_ = min_z2;
}

void TrackFinder::SetMaxTracks(int max_tracks)
{
   max_tracks_ = max_tracks;
}

double TrackFinder::GetMaxRadius()
{
   return max_r_;
}

double TrackFinder::GetMinZ()
{
   return min_z_;
}

double TrackFinder::GetMaxZ()
{
   return max_z_;
}

double TrackFinder::GetMinZ1()
{
   return min_z1_;
}

double TrackFinder::GetMinZ2()
{
   return min_z2_;
}

int TrackFinder::GetMaxTracks()
{
   return max_tracks_;
}

int TrackFinder::SetTracks_with_outer_guard_layer(Chamber& chamber, XTcurve& xt)
{
   ClearTracks();

   int m1 = chamber.GetNumHitCells(1);
   int m2 = chamber.GetNumHitCells(2);
   int m3 = chamber.GetNumHitCells(3);
   int m4 = chamber.GetNumHitCells(4);
   int m5 = chamber.GetNumHitCells(5);
   int m6 = chamber.GetNumHitCells(6);
   int m7 = chamber.GetNumHitCells(7);
   int m8 = chamber.GetNumHitCells(8);
   if (g_debug_trackfinder) {
      printf("#/hits@layer [1-8] => %d %d %d %d %d %d %d %d\n", m1, m2, m3, m4, m5, m6, m7, m8);
   }

   for (int icellhit1=0; icellhit1<m1; icellhit1++) { int icell1 = chamber.GetHitCellNumber(1, icellhit1); Hit& hit1 = chamber.GetHit(1, icell1, 0);
   for (int icellhit2=0; icellhit2<m2; icellhit2++) { int icell2 = chamber.GetHitCellNumber(2, icellhit2); Hit& hit2 = chamber.GetHit(2, icell2, 0);
   for (int icellhit3=0; icellhit3<m3; icellhit3++) { int icell3 = chamber.GetHitCellNumber(3, icellhit3); Hit& hit3 = chamber.GetHit(3, icell3, 0);
   for (int icellhit4=0; icellhit4<m4; icellhit4++) { int icell4 = chamber.GetHitCellNumber(4, icellhit4); Hit& hit4 = chamber.GetHit(4, icell4, 0);
   for (int icellhit5=0; icellhit5<m5; icellhit5++) { int icell5 = chamber.GetHitCellNumber(5, icellhit5); Hit& hit5 = chamber.GetHit(5, icell5, 0);
   for (int icellhit6=0; icellhit6<m6; icellhit6++) { int icell6 = chamber.GetHitCellNumber(6, icellhit6); Hit& hit6 = chamber.GetHit(6, icell6, 0);
   for (int icellhit7=0; icellhit7<m7; icellhit7++) { int icell7 = chamber.GetHitCellNumber(7, icellhit7); Hit& hit7 = chamber.GetHit(7, icell7, 0);
   for (int icellhit8=0; icellhit8<m8; icellhit8++) { int icell8 = chamber.GetHitCellNumber(8, icellhit7); Hit& hit8 = chamber.GetHit(8, icell8, 0);
      hit1.SetT0(chamber.GetT0(1, icell1));
      hit2.SetT0(chamber.GetT0(2, icell2));
      hit3.SetT0(chamber.GetT0(3, icell3));
      hit4.SetT0(chamber.GetT0(4, icell4));
      hit5.SetT0(chamber.GetT0(5, icell5));
      hit6.SetT0(chamber.GetT0(6, icell6));
      hit7.SetT0(chamber.GetT0(7, icell7));
      hit8.SetT0(chamber.GetT0(8, icell8));
      double hitR1 = hit1.GetHitR(xt);
      double hitR2 = hit2.GetHitR(xt);
      double hitR3 = hit3.GetHitR(xt);
      double hitR4 = hit4.GetHitR(xt);
      double hitR5 = hit5.GetHitR(xt);
      double hitR6 = hit6.GetHitR(xt);
      double hitR7 = hit7.GetHitR(xt);
      double hitR8 = hit8.GetHitR(xt);
      if (g_debug_trackfinder) {
         printf("hitR1 %f hitR2 %f hitR3 %f hitR4 %f hitR5 %f hitR6 %f hitR7 %f hitR8 %f\n", hitR1, hitR2, hitR3, hitR4, hitR5, hitR6, hitR7, hitR8);
      }
      if (hitR1<0 || hitR2<0 || hitR3<0 ||hitR4<0 || hitR5<0 || hitR6<0 || hitR7<0 || hitR8<0) {
         continue;
      }
      if (hitR1>max_r_ || hitR2>max_r_ || hitR3>max_r_ ||hitR4>max_r_ || hitR5>max_r_ || hitR6>max_r_ || hitR7>max_r_ || hitR8>max_r_) {
         continue;
      }

      if (num_tracks_>= max_tracks_) {
         goto end;
      }

      Track& track = tracks_[num_tracks_];
      track.SetHit(1, hit1);
      track.SetHit(2, hit2);
      track.SetHit(3, hit3);
      track.SetHit(4, hit4);
      track.SetHit(5, hit5);
      track.SetHit(6, hit6);
      track.SetHit(7, hit7);
      track.SetHit(8, hit8);
      //printf("num_tracks %d time %d max_tracks %d\n", num_tracks_, time(NULL), max_tracks_);
      num_tracks_++;
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   } 
end:

   if (g_debug_trackfinder) {
      printf("num_tracks_ %d\n", num_tracks_);
   }

   return num_tracks_;
}

int TrackFinder::SetTracks_without_guard_layers(Chamber& chamber, XTcurve& xt)
{
   ClearTracks();

   int m1 = chamber.GetNumHitCells(1);
   int m2 = chamber.GetNumHitCells(2);
   int m3 = chamber.GetNumHitCells(3);
   int m4 = chamber.GetNumHitCells(4);
   int m5 = chamber.GetNumHitCells(5);
   int m6 = chamber.GetNumHitCells(6);
   int m7 = chamber.GetNumHitCells(7);
   if (g_debug_trackfinder) {
      printf("#/hits@layer [1-7] => %d %d %d %d %d %d %d\n", m1, m2, m3, m4, m5, m6, m7);
   }

   for (int icellhit1=0; icellhit1<m1; icellhit1++) { int icell1 = chamber.GetHitCellNumber(1, icellhit1); Hit& hit1 = chamber.GetHit(1, icell1, 0);
   for (int icellhit2=0; icellhit2<m2; icellhit2++) { int icell2 = chamber.GetHitCellNumber(2, icellhit2); Hit& hit2 = chamber.GetHit(2, icell2, 0);
   for (int icellhit3=0; icellhit3<m3; icellhit3++) { int icell3 = chamber.GetHitCellNumber(3, icellhit3); Hit& hit3 = chamber.GetHit(3, icell3, 0);
   for (int icellhit4=0; icellhit4<m4; icellhit4++) { int icell4 = chamber.GetHitCellNumber(4, icellhit4); Hit& hit4 = chamber.GetHit(4, icell4, 0);
   for (int icellhit5=0; icellhit5<m5; icellhit5++) { int icell5 = chamber.GetHitCellNumber(5, icellhit5); Hit& hit5 = chamber.GetHit(5, icell5, 0);
   for (int icellhit6=0; icellhit6<m6; icellhit6++) { int icell6 = chamber.GetHitCellNumber(6, icellhit6); Hit& hit6 = chamber.GetHit(6, icell6, 0);
   for (int icellhit7=0; icellhit7<m7; icellhit7++) { int icell7 = chamber.GetHitCellNumber(7, icellhit7); Hit& hit7 = chamber.GetHit(7, icell7, 0);
      hit1.SetT0(chamber.GetT0(1, icell1));
      hit2.SetT0(chamber.GetT0(2, icell2));
      hit3.SetT0(chamber.GetT0(3, icell3));
      hit4.SetT0(chamber.GetT0(4, icell4));
      hit5.SetT0(chamber.GetT0(5, icell5));
      hit6.SetT0(chamber.GetT0(6, icell6));
      hit7.SetT0(chamber.GetT0(7, icell7));
      double hitR1 = hit1.GetHitR(xt);
      double hitR2 = hit2.GetHitR(xt);
      double hitR3 = hit3.GetHitR(xt);
      double hitR4 = hit4.GetHitR(xt);
      double hitR5 = hit5.GetHitR(xt);
      double hitR6 = hit6.GetHitR(xt);
      double hitR7 = hit7.GetHitR(xt);
      if (g_debug_trackfinder) {
         printf("hitR1 %f hitR2 %f hitR3 %f hitR4 %f hitR5 %f hitR6 %f hitR7 %f\n", hitR1, hitR2, hitR3, hitR4, hitR5, hitR6, hitR7);
      }
      if (hitR1<0 || hitR2<0 || hitR3<0 ||hitR4<0 || hitR5<0 || hitR6<0 || hitR7<0) {
         continue;
      }
      if (hitR1>max_r_ || hitR2>max_r_ || hitR3>max_r_ ||hitR4>max_r_ || hitR5>max_r_ || hitR6>max_r_ || hitR7>max_r_) {
         continue;
      }

      if (num_tracks_>= max_tracks_) {
         goto end;
      }

      Track& track = tracks_[num_tracks_];
      track.SetHit(1, hit1);
      track.SetHit(2, hit2);
      track.SetHit(3, hit3);
      track.SetHit(4, hit4);
      track.SetHit(5, hit5);
      track.SetHit(6, hit6);
      track.SetHit(7, hit7);
      //printf("num_tracks %d time %d max_tracks %d\n", num_tracks_, time(NULL), max_tracks_);
      num_tracks_++;
   } 
   } 
   } 
   } 
   } 
   } 
   } 
end:

   if (g_debug_trackfinder) {
      printf("num_tracks_ %d\n", num_tracks_);
   }

   return num_tracks_;
}
