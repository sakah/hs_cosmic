#include "event.h"
#include "chamber.h"
#include "line.h"
#include "wiremap.h"
#include "track.h"
#include "xtcurve.h"
#include "trackfinder.h"
#include "analysis.h"

//___________________
struct Test1
{
   ConstXTcurve xt;
   Chamber chamber;
   Event event;
   Track track;
   TrackFinder finder;

   Test1() : event("test/run15.root")
   {
      xt.SetDriftVelocity(0.040);
      chamber.ReadWireMap("wirepos.proto4.sw.ver7.txt");
      chamber.SetT0(0, -850);
      chamber.SetT0(1, -850);
   };

   void plot(int iev, int cid1, int cid2, double z1, double z2, double z_step)
   {
      WireMap& wiremap = chamber.GetWireMap();

      event.GetEntry(iev);
      chamber.GetEvent(event);
      chamber.PrintHits(xt);
      chamber.DrawHits(event, xt);

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

      int num_tracks = finder.SetTracks(chamber, xt);
      if (num_tracks==0) return;
      finder.MakeTangents(chamber, xt, cid1, cid2, z1, z2);
      finder.PrintTracks(wiremap, xt);

      finder.FindBestTrack(chamber, xt, cid1, cid2, z_step);
      Track& min_track = finder.GetBestTrack();
      Line& min_tangent = min_track.GetMinTangent(wiremap, xt);
      chamber.DrawTrack(event, xt, min_track, min_tangent);
   };
};

//___________________
struct Test2
{
   AnaResXVSFitX ana16;
   AnaResXVSFitX ana24;
   AnaResXVSFitX ana27;
   Test2()
   {
      ana16.OpenRootFile("cosmic_root_out/freet0/run16.root");
      ana24.OpenRootFile("cosmic_root_out/freet0/run24.root");
      ana27.OpenRootFile("cosmic_root_out/freet0/run27.root");
      //ana16.Loop(); printf("run16 done\n");
      //ana24.Loop(); printf("run24 done\n");
      ana27.Loop(); printf("run27 done\n");
   };
};

//___________________
struct Test3
{
   // config1: cid1=1, cid2=7
   // config2: cid1=2, cid2=7
   AnaResXVSFitX ana16_1;
   AnaResXVSFitX ana24_1;
   AnaResXVSFitX ana27_1;
   AnaResXVSFitX ana16_2;
   AnaResXVSFitX ana24_2;
   AnaResXVSFitX ana27_2;
   Test3()
   {
      ana16_1.OpenRootFile("../cosmic_root_out/run16_config_1.root");
      ana24_1.OpenRootFile("../cosmic_root_out/run24_config_1.root");
      ana27_1.OpenRootFile("../cosmic_root_out/run27_config_1.root");
      ana16_2.OpenRootFile("../cosmic_root_out/run16_config_2.root");
      ana24_2.OpenRootFile("../cosmic_root_out/run24_config_2.root");
      ana27_2.OpenRootFile("../cosmic_root_out/run27_config_2.root");

      ana16_1.Loop(); printf("run16 config1 done\n");
      ana24_1.Loop(); printf("run24 config1 done\n");
      ana27_1.Loop(); printf("run27 config1 done\n");
      ana16_2.Loop(); printf("run16 config2 done\n");
      ana24_2.Loop(); printf("run24 config2 done\n");
      ana27_2.Loop(); printf("run27 config2 done\n");
   };
};
