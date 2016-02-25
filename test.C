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
   xt.SetDriftVelocity(0.040);

   Chamber chamber;
   chamber.ReadWireMap("wirepos.proto4.sw.ver7.txt");
   WireMap& wiremap = chamber.GetWireMap();

   Event event("test/run15.root");
   event.SetT0(-850);
   event.GetEntry(iev);
   chamber.GetEvent(event);
   chamber.PrintHits(xt);
   chamber.DrawHits(event, xt);

   Track track;
   bool alllayerhit=true;
   for (int cid=1; cid<=7; cid++) {
      int n =  chamber.GetNumHitCells(cid);
      if (n==0) {
         alllayerhit = false;
         break;
      }
   }
   if (!alllayerhit) {
      return;
   }

   for (int cid=1; cid<=7; cid++) {
      int icell = chamber.GetHitCellNumber(cid, 0);
      Hit& hit = chamber.GetHit(cid, icell, 0);
      track.SetHit(cid, hit);
   }

   track.MakeTangents(wiremap, xt, cid1, cid2, z1, z2);
   track.PrintTangents(wiremap, xt);

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

}
