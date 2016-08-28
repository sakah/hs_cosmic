#include "event.h"
#include "chamber.h"
#include "line.h"
#include "wiremap.h"
#include "track.h"
#include "xtcurve.h"
#include "trackfinder.h"

Chamber chamber;

void loop()
{
   Event event("test/run_000052.root");

   double rot_deg = -90-11.25; // degree
   chamber.ReadWireMap("wirepos_20160814.txt", rot_deg);
   for (int bd=0; bd<104; bd++) {
      chamber.SetT0(bd, -1050);
   }
   //WireMap& wiremap = chamber.GetWireMap();

   int nevents = event.GetEntries();
   for (int iev=0; iev<nevents; iev++) {
      event.GetEntry(iev);
      chamber.GetEvent(event);
      bool alllayerhit = chamber.hasAllLayerHits();
      printf("iev %d alllayerhit %d\n", iev, alllayerhit);
   }
}
void plot(int iev, int side1, int side2, int cid1, int cid2, double z1, double z2, int itan, double z_step, char* pdf_dir=NULL)
{
   int test_side=0;
   int test_cid=15;

   ConstXTcurve xt;
   xt.SetDriftVelocity(0.025);

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

   double adc_peak_thre = 10;
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

   int nhits=0;
   Track track;
   for (int side=0; side<2; side++) {
      for (int cid=0; cid<20; cid++) {
         if (!chamber.isLayerUsed(side, cid)) {
            continue;
         }
         if (chamber.isGuardLayer(side, cid)) {
            continue;
         }
         int icell = chamber.GetHitCellNumber(side, cid, 0);
         if (icell==-1) continue;

         Hit& hit = chamber.GetHit(side, cid, icell, 0);
         hit.PrintHit(xt, adc_peak_thre);
         track.SetHit(side, cid, hit);
         nhits++;
      }
   }
   printf("nhits %d\n", nhits);

   track.MakeTangents(wiremap, xt, side1, side2, cid1, cid2, z1, z2);
   track.PrintTangents(wiremap, xt);
   //Line& line = track.GetTangent(itan);
   //chamber.DrawTrack(event, xt, track, line);

   TrackFinder finder;
   finder.SetTracks(chamber, xt);
   finder.MakeTangents(chamber, xt, side1, side2, cid1, cid2, z1, z2);
   finder.PrintTracks(wiremap, xt);

   if (!finder.FindBestTrack(chamber, xt, side1, side2, cid1, cid2, z1, z2)) {
      printf("FindBestTrack failed\n");
      return;
   }
   Track& min_track = finder.GetBestTrack();
   //min_track.PrintTrack(wiremap, xt);
   Line& min_line = min_track.GetMinTangent(wiremap, xt);
   chamber.DrawTrack(event, xt, min_track, min_line, test_side, test_cid);

   if (finder.FindBestTrack(chamber, xt, side1, side2, cid1, cid2, z_step)) {
      Track& min_track = finder.GetBestTrack();
      //min_track.PrintTrack(wiremap, xt);
      Line& min_line = min_track.GetMinTangent(wiremap, xt);
      chamber.DrawTrack(event, xt, min_track, min_line, test_side, test_cid);

      double ypos_S1 = 1900/2.0; // mm
      double ypos_S2 = -ypos_S1;
      TVector3 pos_s1 = min_line.GetPosAtY(ypos_S1);
      TVector3 pos_s2 = min_line.GetPosAtY(ypos_S2);
      printf("pos_s1: [mm] "); pos_s1.Print();
      printf("pos_s2: [mm] "); pos_s2.Print();

      min_track.InitFit(wiremap, xt, test_side, test_cid);
      min_track.DoFit(wiremap, xt);
      min_track.PrintFitResults();

      Line& fit_line = min_track.GetFitLine();
      chamber.DrawTrack(event, xt, min_track, fit_line, test_side, test_cid, pdf_dir);
   } else {
      printf("FindBestTrack failed\n");
   }
}
void good_run52()
{
   std::vector<int> list;
   list.push_back(83);
   list.push_back(245);
   list.push_back(295);
   list.push_back(392);
   list.push_back(426);
   list.push_back(527);
   list.push_back(580);
   list.push_back(593);
   list.push_back(611);
   list.push_back(779);
   list.push_back(834);
   list.push_back(1067);
   list.push_back(1077);
   list.push_back(1091);
   list.push_back(1104);
   list.push_back(1124);
   list.push_back(1425);
   list.push_back(1594);
   list.push_back(1646);
   list.push_back(1678);
   list.push_back(1709);
   char* pdf_dir = "run52/";
   for (int i=0; i<list.size(); i++) {
      int iev = list[i];
      plot(iev, 0, 1, 18, 18, 0, 0, 0, 10, pdf_dir);
   }
}

