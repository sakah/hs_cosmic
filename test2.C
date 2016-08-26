#include "event.h"
#include "chamber.h"
#include "line.h"
#include "wiremap.h"
#include "track.h"
#include "xtcurve.h"
#include "trackfinder.h"

void plot(int iev, int cid1, int cid2, double z1, double z2, int itan, double z_step)
{
   ConstXTcurve xt;
   xt.SetDriftVelocity(0.025);

   Chamber chamber;
   double rot_deg = -90-11.25; // degree
   chamber.ReadWireMap("wirepos_20160814.txt", rot_deg);
   WireMap& wiremap = chamber.GetWireMap();

   Event event("test/run_000052.root");
   event.GetEntry(iev);
   event.PrintEvent();
   for (int bd=0; bd<104; bd++) {
      chamber.SetT0(bd, -1050);
   }
   chamber.GetEvent(event);

   double adc_peak_thre = 20;
   chamber.PrintHits(xt, adc_peak_thre);
   chamber.DrawHits(event, xt);

   bool alllayerhit = chamber.hasAllLayerHits();
   if (!alllayerhit) {
      printf("iev %d --> alllayehit false\n", iev);
      return;
   } else {
      printf("iev %d --> alllayehit true \n", iev);
   }
   printf("side1 %d cid1 %d z1 %f | side2 %d cid2 %d z2 %lf\n", side1, cid1, z1, side2, cid2, z2);

   Track track;
   for (int side=0; side<2; side++) {
      for (int cid=0; cid<20; cid++) {
         if (!chamber.isLayerUsed(side, cid)) {
            continue;
         }
         int icell = chamber.GetHitCellNumber(cid, 0);
         Hit& hit = chamber.GetHit(cid, icell, 0);
         hit.PrintHit(xt, adc_peak_thre);
         track.SetHit(side, cid, hit);
      }
   }

   track.MakeTangents(wiremap, xt, side1, side2, cid1, cid2, z1, z2);
   track.PrintTangents(wiremap, xt);
#if 0

   TrackFinder finder;
   finder.MakeTracks(chamber, xt, cid1, cid2, z1, z2);
   finder.PrintTracks(wiremap, xt);

   //chamber.DrawHitsWithTrack(event, xt, track, itan);

   //Track& min_track = finder.FindBestTrack(chamber, xt, cid1, cid2, z1, z2);
   //min_track.PrintTrack(wiremap, xt);
   //int min_itan = min_track.GetMinTangentNumber();
   //chamber.DrawHitsWithTrack(event, xt, min_track, min_itan);

   bool found = finder.FindBestTrack(chamber, xt, cid1, cid2, z_step);
   if (found) {
      Track& min_track = finder.GetBestTrack();
      min_track.PrintTrack(wiremap, xt);
      int min_itan = min_track.GetMinTangentNumber();
      chamber.DrawHitsWithTrack(event, xt, min_track, min_itan);
   }
#endif

}
