#include "trackfinder.h"


TrackFinder::TrackFinder()
{
   has_found_ = false;
   num_tracks_ = 0;
   min_itrack_ = -1;
   max_r_ = 10.0; // 10.0 mm
   min_z_ = -200;
   max_z_ = +200;
   min_z1_ = 0;
   min_z2_ = 0;
}

bool TrackFinder::HasFound()
{
   return has_found_;
}

bool TrackFinder::FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z1, double z2)
{
   WireMap& wiremap = chamber.GetWireMap();
   MakeTracks(chamber, xt, cid1, cid2, z1, z2);

   if (num_tracks_==0) {
      return false;
   }

   double min_chi2 = 1e10;
   for (int itrack=0; itrack<num_tracks_; itrack++) {
      double chi2 = tracks_[itrack].GetChi2OfMinTangent(wiremap, xt);
      if (chi2 < min_chi2) {
         min_chi2 = chi2;
         min_itrack_ = itrack;
      }
   }
   return true;
}

bool TrackFinder::FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z_step)
{
   WireMap& wiremap = chamber.GetWireMap();

   int min_itrack = 0;
   double min_chi2 = 1e10;
   int num_z = (max_z_ - min_z_)/z_step;
   for (int iz1=0; iz1<num_z; iz1++) {
      for (int iz2=0; iz2<num_z; iz2++) {
         double z1 = min_z_ + iz1*z_step;
         double z2 = min_z_ + iz2*z_step;
         bool found = FindBestTrack(chamber, xt, cid1, cid2, z1, z2);
         if (!found) {
            return false;
         }
         double chi2 = GetBestTrack().GetChi2OfMinTangent(wiremap, xt);
         if (chi2 < min_chi2) {
            min_chi2 = chi2;
            min_itrack = min_itrack_;
            min_z1_ = z1;
            min_z2_ = z2;
         }
         if (g_debug_trackfinder>0) {
            printf("z1 %f z2 %f chi2 %f min_chi2 %f min_itrack %d min_z1_ %f min_z2_ %f\n", z1, z2, chi2, min_chi2, min_itrack, min_z1_, min_z2_);
         }
      }
   }
   min_itrack_ = min_itrack;

   Track& min_track = tracks_[min_itrack_];
   min_track.MakeTangents(wiremap, xt, cid1, cid2, min_z1_, min_z2_);
   min_track.SetHitZWithMinTangent(wiremap);

   has_found_ = true;

   return true;
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
   }
}

int TrackFinder::MakeTracks(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z1, double z2)
{
   num_tracks_ = 0;

   WireMap& wiremap = chamber.GetWireMap();

   int m1 = chamber.GetNumHitCells(1);
   int m2 = chamber.GetNumHitCells(2);
   int m3 = chamber.GetNumHitCells(3);
   int m4 = chamber.GetNumHitCells(4);
   int m5 = chamber.GetNumHitCells(5);
   int m6 = chamber.GetNumHitCells(6);
   int m7 = chamber.GetNumHitCells(7);

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
      if (hitR1<0 || hitR2<0 || hitR3<0 ||hitR4<0 || hitR5<0 || hitR6<0 || hitR7<0) {
         continue;
      }
      if (hitR1>max_r_ || hitR2>max_r_ || hitR3>max_r_ ||hitR4>max_r_ || hitR5>max_r_ || hitR6>max_r_ || hitR7>max_r_) {
         continue;
      }

      if (num_tracks_>=MAX_TRACK) {
         break;
      }

      Track& track = tracks_[num_tracks_];
      track.SetHit(1, hit1);
      track.SetHit(2, hit2);
      track.SetHit(3, hit3);
      track.SetHit(4, hit4);
      track.SetHit(5, hit5);
      track.SetHit(6, hit6);
      track.SetHit(7, hit7);
      track.MakeTangents(wiremap, xt, cid1, cid2, z1, z2);
      num_tracks_++;
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   return num_tracks_;
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

