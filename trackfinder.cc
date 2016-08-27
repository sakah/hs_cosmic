#include "trackfinder.h"
#include <time.h>

TrackFinder::TrackFinder()
{
   num_tracks_ = 0;
   min_itrack_ = -1;
   max_r_ = 14.0; // mm, drift distance
   min_z_ = -200; //mm 
   max_z_ = +200; //mm
   min_z1_ = 0;
   min_z2_ = 0;
   max_tracks_ = 4;
//   max_tracks_ = 10;
}

int TrackFinder::SetTracks(Chamber& chamber, XTcurve& xt)
{
   return SetTracksByHitCells(chamber, xt);
   //return SetTracksByMaxAdcPeak(chamber, xt);
}

/*
int TrackFinder::SetTracksByMaxAdcPeak(Chamber& chamber, XTcurve& xt)
{
   num_tracks_ = 0;

   int m1 = chamber.GetNumHitCells(1);
   int m2 = chamber.GetNumHitCells(2);
   int m3 = chamber.GetNumHitCells(3);
   int m4 = chamber.GetNumHitCells(4);
   int m5 = chamber.GetNumHitCells(5);
   int m6 = chamber.GetNumHitCells(6);
   int m7 = chamber.GetNumHitCells(7);
   int num_all_tracks = m1*m2*m3*m4*m5*m6*m7;

   // Require one hit at least for all 7 layers 
   if (num_all_tracks==0) {
      return 0;
   }

   // Because we choose one cell and first tdc hit, number of tracks is 1.

   int icell1, ihit1;
   int icell2, ihit2;
   int icell3, ihit3;
   int icell4, ihit4;
   int icell5, ihit5;
   int icell6, ihit6;
   int icell7, ihit7;
   chamber.GetHitCellNumberByMaxAdcPeak(1, icell1, ihit1);
   chamber.GetHitCellNumberByMaxAdcPeak(2, icell2, ihit2);
   chamber.GetHitCellNumberByMaxAdcPeak(3, icell3, ihit3);
   chamber.GetHitCellNumberByMaxAdcPeak(4, icell4, ihit4);
   chamber.GetHitCellNumberByMaxAdcPeak(5, icell5, ihit5);
   chamber.GetHitCellNumberByMaxAdcPeak(6, icell6, ihit6);
   chamber.GetHitCellNumberByMaxAdcPeak(7, icell7, ihit7);

   // first tdc hit is used.
   //ihit1=0;
   //ihit2=0;
   //ihit3=0;
   //ihit4=0;
   //ihit5=0;
   //ihit6=0;
   //ihit7=0;

   Hit& hit1 = chamber.GetHit(1, icell1, ihit1); hit1.SetT0(chamber.GetT0(1, icell1));
   Hit& hit2 = chamber.GetHit(2, icell2, ihit2); hit2.SetT0(chamber.GetT0(2, icell2));
   Hit& hit3 = chamber.GetHit(3, icell3, ihit3); hit3.SetT0(chamber.GetT0(3, icell3));
   Hit& hit4 = chamber.GetHit(4, icell4, ihit4); hit4.SetT0(chamber.GetT0(4, icell4));
   Hit& hit5 = chamber.GetHit(5, icell5, ihit5); hit5.SetT0(chamber.GetT0(5, icell5));
   Hit& hit6 = chamber.GetHit(6, icell6, ihit6); hit6.SetT0(chamber.GetT0(6, icell6));
   Hit& hit7 = chamber.GetHit(7, icell7, ihit7); hit7.SetT0(chamber.GetT0(7, icell7));

   if (g_debug_trackfinder) {
      printf("max_icell icell1 %d ihit %d\n", icell1, ihit1);
      printf("max_icell icell2 %d ihit %d\n", icell2, ihit2);
      printf("max_icell icell3 %d ihit %d\n", icell3, ihit3);
      printf("max_icell icell4 %d ihit %d\n", icell4, ihit4);
      printf("max_icell icell5 %d ihit %d\n", icell5, ihit5);
      printf("max_icell icell6 %d ihit %d\n", icell6, ihit6);
      printf("max_icell icell7 %d ihit %d\n", icell7, ihit7);
   }

   double hitR1 = hit1.GetHitR(xt);
   double hitR2 = hit2.GetHitR(xt);
   double hitR3 = hit3.GetHitR(xt);
   double hitR4 = hit4.GetHitR(xt);
   double hitR5 = hit5.GetHitR(xt);
   double hitR6 = hit6.GetHitR(xt);
   double hitR7 = hit7.GetHitR(xt);
   //if (g_debug_trackfinder) {
      printf("hitR1 %f hitR2 %f hitR3 %f hitR4 %f hitR5 %f hitR6 %f hitR7 %f\n", hitR1, hitR2, hitR3, hitR4, hitR5, hitR6, hitR7);
   //}
   if (hitR1<0 || hitR2<0 || hitR3<0 ||hitR4<0 || hitR5<0 || hitR6<0 || hitR7<0) {
      return -1;
   }
   if (hitR1>max_r_ || hitR2>max_r_ || hitR3>max_r_ ||hitR4>max_r_ || hitR5>max_r_ || hitR6>max_r_ || hitR7>max_r_) {
      return -2;
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
   return num_tracks_;
}
*/
int TrackFinder::SetTracksByHitCells(Chamber& chamber, XTcurve& xt)
{
   num_tracks_ = 0;

   //int m11 = chamber.GetNumHitCells(0,19);
   int m12 = chamber.GetNumHitCells(0,18);
   int m13 = chamber.GetNumHitCells(0,17);
   int m14 = chamber.GetNumHitCells(0,16);
   int m15 = chamber.GetNumHitCells(0,15);
   int m16 = chamber.GetNumHitCells(0,14);
   int m17 = chamber.GetNumHitCells(0,13);
   int m18 = chamber.GetNumHitCells(0,12);
   int m19 = chamber.GetNumHitCells(0,11);
   int m1a = chamber.GetNumHitCells(0,10);
   int m1b = chamber.GetNumHitCells(0, 9);
   int m1c = chamber.GetNumHitCells(0, 8);
   //int m1d = chamber.GetNumHitCells(0, 7);

   //int m21 = chamber.GetNumHitCells(1,10);
   int m22 = chamber.GetNumHitCells(1,11);
   int m23 = chamber.GetNumHitCells(1,12);
   int m24 = chamber.GetNumHitCells(1,13);
   int m25 = chamber.GetNumHitCells(1,14);
   int m26 = chamber.GetNumHitCells(1,15);
   int m27 = chamber.GetNumHitCells(1,16);
   int m28 = chamber.GetNumHitCells(1,17);
   int m29 = chamber.GetNumHitCells(1,18);
   //int m2a = chamber.GetNumHitCells(1,19);
   //if (g_debug_trackfinder) {
      printf("#/hits@layer [ 8-20] => %d %d %d %d %d %d %d %d %d %d %d\n", m12, m13, m14, m15, m16, m17, m18, m19, m1a, m1b, m1c);
      printf("#/hits@layer [11-20] => %d %d %d %d %d %d %d %d\n", m22, m23, m24, m25, m26, m27, m28, m29);
   //}

   //for (int icellhit11=0; icellhit11<m11; icellhit11++) { int icell11 = chamber.GetHitCellNumber(0, 19, icellhit11); Hit& hit11 = chamber.GetHit(0,  19, icell11, 0);
   for (int icellhit12=0; icellhit12<m12; icellhit12++) { int icell12 = chamber.GetHitCellNumber(0, 18, icellhit12); Hit& hit12 = chamber.GetHit(0,  18, icell12, 0);
   for (int icellhit13=0; icellhit13<m13; icellhit13++) { int icell13 = chamber.GetHitCellNumber(0, 17, icellhit13); Hit& hit13 = chamber.GetHit(0,  17, icell13, 0);
   for (int icellhit14=0; icellhit14<m14; icellhit14++) { int icell14 = chamber.GetHitCellNumber(0, 16, icellhit14); Hit& hit14 = chamber.GetHit(0,  16, icell14, 0);
   for (int icellhit15=0; icellhit15<m15; icellhit15++) { int icell15 = chamber.GetHitCellNumber(0, 15, icellhit15); Hit& hit15 = chamber.GetHit(0,  15, icell15, 0);
   for (int icellhit16=0; icellhit16<m16; icellhit16++) { int icell16 = chamber.GetHitCellNumber(0, 14, icellhit16); Hit& hit16 = chamber.GetHit(0,  14, icell16, 0);
   for (int icellhit17=0; icellhit17<m17; icellhit17++) { int icell17 = chamber.GetHitCellNumber(0, 13, icellhit17); Hit& hit17 = chamber.GetHit(0,  13, icell17, 0);
   for (int icellhit18=0; icellhit18<m18; icellhit18++) { int icell18 = chamber.GetHitCellNumber(0, 12, icellhit18); Hit& hit18 = chamber.GetHit(0,  12, icell18, 0);
   for (int icellhit19=0; icellhit19<m19; icellhit19++) { int icell19 = chamber.GetHitCellNumber(0, 11, icellhit19); Hit& hit19 = chamber.GetHit(0,  11, icell19, 0);
   for (int icellhit1a=0; icellhit1a<m1a; icellhit1a++) { int icell1a = chamber.GetHitCellNumber(0, 10, icellhit1a); Hit& hit1a = chamber.GetHit(0,  10, icell1a, 0);
   for (int icellhit1b=0; icellhit1b<m1b; icellhit1b++) { int icell1b = chamber.GetHitCellNumber(0,  9, icellhit1b); Hit& hit1b = chamber.GetHit(0,   9, icell1b, 0);
   for (int icellhit1c=0; icellhit1c<m1c; icellhit1c++) { int icell1c = chamber.GetHitCellNumber(0,  8, icellhit1c); Hit& hit1c = chamber.GetHit(0,   8, icell1c, 0);
   //for (int icellhit1d=0; icellhit1d<m1d; icellhit1d++) { int icell1d = chamber.GetHitCellNumber(0,  7, icellhit1d); Hit& hit1d = chamber.GetHit(0,   7, icell1d, 0);

   //for (int icellhit21=0; icellhit21<m21; icellhit21++) { int icell21 = chamber.GetHitCellNumber(1, 10, icellhit21); Hit& hit21 = chamber.GetHit(1,  10, icell21, 0);
   for (int icellhit22=0; icellhit22<m22; icellhit22++) { int icell22 = chamber.GetHitCellNumber(1, 11, icellhit22); Hit& hit22 = chamber.GetHit(1,  11, icell22, 0);
   for (int icellhit23=0; icellhit23<m23; icellhit23++) { int icell23 = chamber.GetHitCellNumber(1, 12, icellhit23); Hit& hit23 = chamber.GetHit(1,  12, icell23, 0);
   for (int icellhit24=0; icellhit24<m24; icellhit24++) { int icell24 = chamber.GetHitCellNumber(1, 13, icellhit24); Hit& hit24 = chamber.GetHit(1,  13, icell24, 0);
   for (int icellhit25=0; icellhit25<m25; icellhit25++) { int icell25 = chamber.GetHitCellNumber(1, 14, icellhit25); Hit& hit25 = chamber.GetHit(1,  14, icell25, 0);
   for (int icellhit26=0; icellhit26<m26; icellhit26++) { int icell26 = chamber.GetHitCellNumber(1, 15, icellhit26); Hit& hit26 = chamber.GetHit(1,  15, icell26, 0);
   for (int icellhit27=0; icellhit27<m27; icellhit27++) { int icell27 = chamber.GetHitCellNumber(1, 16, icellhit27); Hit& hit27 = chamber.GetHit(1,  16, icell27, 0);
   for (int icellhit28=0; icellhit28<m28; icellhit28++) { int icell28 = chamber.GetHitCellNumber(1, 17, icellhit28); Hit& hit28 = chamber.GetHit(1,  17, icell28, 0);
   for (int icellhit29=0; icellhit29<m29; icellhit29++) { int icell29 = chamber.GetHitCellNumber(1, 18, icellhit29); Hit& hit29 = chamber.GetHit(1,  18, icell29, 0);
   //for (int icellhit2a=0; icellhit2a<m2a; icellhit2a++) { int icell2a = chamber.GetHitCellNumber(1, 19, icellhit2a); Hit& hit2a = chamber.GetHit(1,  19, icell2a, 0);

      //hit11.SetT0(chamber.GetT0(19, icell11));
      hit12.SetT0(chamber.GetT0(18, icell12));
      hit13.SetT0(chamber.GetT0(17, icell13));
      hit14.SetT0(chamber.GetT0(16, icell14));
      hit15.SetT0(chamber.GetT0(15, icell15));
      hit16.SetT0(chamber.GetT0(14, icell16));
      hit17.SetT0(chamber.GetT0(13, icell17));
      hit18.SetT0(chamber.GetT0(12, icell18));
      hit19.SetT0(chamber.GetT0(11, icell19));
      hit1a.SetT0(chamber.GetT0(10, icell1a));
      hit1b.SetT0(chamber.GetT0( 9, icell1b));
      hit1c.SetT0(chamber.GetT0( 8, icell1c));
      //hit1d.SetT0(chamber.GetT0( 7, icell1d));

      //hit21.SetT0(chamber.GetT0(10, icell21));
      hit22.SetT0(chamber.GetT0(11, icell22));
      hit23.SetT0(chamber.GetT0(12, icell23));
      hit24.SetT0(chamber.GetT0(13, icell24));
      hit25.SetT0(chamber.GetT0(14, icell25));
      hit26.SetT0(chamber.GetT0(15, icell26));
      hit27.SetT0(chamber.GetT0(16, icell27));
      hit28.SetT0(chamber.GetT0(17, icell28));
      hit29.SetT0(chamber.GetT0(18, icell29));
      //hit2a.SetT0(chamber.GetT0(19, icell2a));

      //double hitR11 = hit11.GetHitR(xt);
      double hitR12 = hit12.GetHitR(xt);
      double hitR13 = hit13.GetHitR(xt);
      double hitR14 = hit14.GetHitR(xt);
      double hitR15 = hit15.GetHitR(xt);
      double hitR16 = hit16.GetHitR(xt);
      double hitR17 = hit17.GetHitR(xt);
      double hitR18 = hit18.GetHitR(xt);
      double hitR19 = hit19.GetHitR(xt);
      double hitR1a = hit1a.GetHitR(xt);
      double hitR1b = hit1b.GetHitR(xt);
      double hitR1c = hit1c.GetHitR(xt);
      //double hitR1d = hit1d.GetHitR(xt);
      //double hitR21 = hit21.GetHitR(xt);
      double hitR22 = hit22.GetHitR(xt);
      double hitR23 = hit23.GetHitR(xt);
      double hitR24 = hit24.GetHitR(xt);
      double hitR25 = hit25.GetHitR(xt);
      double hitR26 = hit26.GetHitR(xt);
      double hitR27 = hit27.GetHitR(xt);
      double hitR28 = hit28.GetHitR(xt);
      double hitR29 = hit29.GetHitR(xt);
      //double hitR2a = hit2a.GetHitR(xt);

      //if (g_debug_trackfinder) {
         printf("hitR12 %f hitR13 %f hitR14 %f hitR15 %f hitR16 %f hitR17 %f hitR18 %f hitR19 %f hitR1a %f hitR1b %f hitR1c %f\n", 
               hitR12, hitR13, hitR14, hitR15, hitR16, hitR17, hitR18, hitR19, hitR1a, hitR1b, hitR1c);
         printf("hitR22 %f hitR23 %f hitR24 %f hitR25 %f hitR26 %f hitR27 %f hitR28 %f hitR29 %f\n",
               hitR22, hitR23, hitR24, hitR25, hitR26, hitR27, hitR28, hitR29);
      //}

      if (hitR12<0 || hitR13<0 ||hitR14<0 || hitR15<0 || hitR16<0 || hitR17<0 || hitR18<0 || hitR19<0 || hitR1a<0 || hitR1b<0 || hitR1c<0 ) {
         continue;
      }
      if (hitR22<0 || hitR23<0 ||hitR24<0 || hitR25<0 || hitR26<0 || hitR27<0 || hitR28<0 || hitR29<0)  {
         continue;
      }

      if (hitR12>max_r_ || hitR13>max_r_ ||hitR14>max_r_ || hitR15>max_r_ || hitR16>max_r_ || hitR17>max_r_ || hitR18>max_r_ || hitR19>max_r_ || hitR1a>max_r_ || hitR1b>max_r_ || hitR1c>max_r_ ) {
         continue;
      }

      if (hitR22>max_r_ || hitR23>max_r_ ||hitR24>max_r_ || hitR25>max_r_ || hitR26>max_r_ || hitR27>max_r_ || hitR28>max_r_ || hitR29>max_r_) {
         continue;
      }

      if (num_tracks_>= max_tracks_) {
         goto end;
      }

      Track& track = tracks_[num_tracks_];
      //track.SetHit(0, 19, hit11);
      track.SetHit(0, 18, hit12);
      track.SetHit(0, 17, hit13);
      track.SetHit(0, 16, hit14);
      track.SetHit(0, 15, hit15);
      track.SetHit(0, 14, hit16);
      track.SetHit(0, 13, hit17);
      track.SetHit(0, 12, hit18);
      track.SetHit(0, 11, hit19);
      track.SetHit(0, 10, hit1a);
      track.SetHit(0,  9, hit1b);
      track.SetHit(0,  8, hit1c);
      //track.SetHit(0,  7, hit1d);

      //track.SetHit(1, 10, hit21);
      track.SetHit(1, 11, hit22);
      track.SetHit(1, 12, hit23);
      track.SetHit(1, 13, hit24);
      track.SetHit(1, 14, hit25);
      track.SetHit(1, 15, hit26);
      track.SetHit(1, 16, hit27);
      track.SetHit(1, 17, hit28);
      track.SetHit(1, 18, hit29);
      //track.SetHit(1, 19, hit2a);

      //printf("num_tracks %d time %d max_tracks %d\n", num_tracks_, time(NULL), max_tracks_);
      num_tracks_++;

   //} 
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   //} 

   //} 
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   } 
   //} 
end:

   if (g_debug_trackfinder) {
      printf("num_tracks_ %d\n", num_tracks_);
   }

   return num_tracks_;
}

void TrackFinder::FindBestTrack(Chamber& chamber, XTcurve& xt, int side1, int side2, int cid1, int cid2, double z1, double z2)
{
   WireMap& wiremap = chamber.GetWireMap();
   MakeTangents(chamber, xt, side1, side2, cid1, cid2, z1, z2);

   double min_chi2 = 1e10;
   for (int itrack=0; itrack<num_tracks_; itrack++) {
      Line& min_tangent = tracks_[itrack].GetMinTangent(wiremap, xt);
      double chi2 = tracks_[itrack].GetChi2OfLine(wiremap, xt, min_tangent);
      if (chi2 < min_chi2) {
         min_chi2 = chi2;
         min_itrack_ = itrack;
      }
   }
}

void TrackFinder::FindBestTrack(Chamber& chamber, XTcurve& xt, int side1, int side2, int cid1, int cid2, double z_step)
{
   WireMap& wiremap = chamber.GetWireMap();

   int min_itrack = 0;
   double min_chi2 = 1e10;
   int num_z = (max_z_ - min_z_)/z_step;
   for (int iz1=0; iz1<num_z; iz1++) {
      for (int iz2=0; iz2<num_z; iz2++) {
         double z1 = min_z_ + iz1*z_step;
         double z2 = min_z_ + iz2*z_step;
         FindBestTrack(chamber, xt, side1, side2, cid1, cid2, z1, z2);
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
   min_track.MakeTangents(wiremap, xt, side1, side2, cid1, cid2, min_z1_, min_z2_);
   min_track.SetHitZWithMinTangent(wiremap, xt);
}

void TrackFinder::MakeTangents(Chamber& chamber, XTcurve& xt, int side1, int side2, int cid1, int cid2, double z1, double z2)
{
   WireMap& wiremap = chamber.GetWireMap();

   for (int itrack=0; itrack<num_tracks_; itrack++) {
      tracks_[itrack].MakeTangents(wiremap, xt, side1, side2, cid1, cid2, z1, z2);
   }
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

