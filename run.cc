#include "run.h"

Run::Run()
{
}

Run::Run(const char* root_file_path)
{
   xt_.SetDriftVelocity(0.040);
   chamber_.ReadWireMap("wirepos.proto4.sw.ver7.txt");
   event_.OpenRootFile(root_file_path);
}

void Run::GetNext()
{
   bool skip_empty_event = true;
   event_.GetNext(skip_empty_event);
   chamber_.GetEvent(event_);
}

void Run::GetEntry(Long64_t event_number)
{
   event_.GetEntry(event_number);
   chamber_.GetEvent(event_);
}

void Run::SetT0(double t0_0, double t0_1)
{
   chamber_.SetT0(0, t0_0);
   chamber_.SetT0(1, t0_1);
}

void Run::PrintHits()
{
   chamber_.PrintHits(xt_);
}

void Run::DrawHits()
{
   chamber_.DrawHits(event_, xt_);
}

void Run::MakeTangents(int cid1, int cid2, double z1, double z2)
{
   bool alllayerhit=true;
   for (int cid=1; cid<=7; cid++) {
      int n =  chamber_.GetNumHitCells(cid);
      if (n==0) {
         alllayerhit = false;
         break;
      }
   }
   if (!alllayerhit) {
      return;
   }

   for (int cid=1; cid<=7; cid++) {
      int icell = chamber_.GetHitCellNumber(cid, 0);
      if (cid==1) {
         icell = chamber_.GetHitCellNumber(cid, 1);
      }
      Hit& hit = chamber_.GetHit(cid, icell, 0);
      hit.SetT0(chamber_.GetT0(cid, icell));
      track_.SetHit(cid, hit);
   }

   WireMap& wiremap = chamber_.GetWireMap();
   track_.MakeTangents(wiremap, xt_, cid1, cid2, z1, z2);
}

void Run::PrintTangents()
{
   WireMap& wiremap = chamber_.GetWireMap();
   track_.PrintTangents(wiremap, xt_);
}

void Run::DrawTangents()
{
   DrawHits();
   track_.DrawTangents();
}

bool Run::FindBestTangent(Long64_t event_number, int cid1, int cid2, double z_step)
{
   if (event_number>=0) {
      GetEntry(event_number);
   } else {
      GetNext();
   }

   WireMap& wiremap = chamber_.GetWireMap();
   bool found = finder_.FindBestTrack(chamber_, xt_, cid1, cid2, z_step);
   finder_.PrintTracks(wiremap, xt_);
   if (!found) {
      return false;
   }
   return true;
}

void Run::DrawBestTangent(Long64_t event_number, int cid1, int cid2, double z_step)
{
   bool found = FindBestTangent(event_number, cid1, cid2, z_step);
   if (!found) {
      printf("tangent cannot be found...\n");
      return;
   }
   WireMap& wiremap = chamber_.GetWireMap();
   Track& min_track = finder_.GetBestTrack();
   min_track.PrintTrackWithLine(wiremap, xt_, min_track.GetMinTangent(wiremap, xt_));
   Line& min_tangent = min_track.GetMinTangent(wiremap, xt_);
   chamber_.DrawTrack(event_, xt_, min_track, min_tangent);
}

bool Run::DoFit(Long64_t event_number, int cid1, int cid2, double z_step, int test_cid)
{
   WireMap& wiremap = chamber_.GetWireMap();

   if (event_number>=0) {
      GetEntry(event_number);
   } else {
      GetNext();
   }

   bool found = FindBestTangent(event_number, cid1, cid2, z_step);
   if (!found) {
      printf("tangent cannot be found...\n");
      return false;
   }
   Track& min_track = finder_.GetBestTrack();
   min_track.PrintTrackWithLine(wiremap, xt_, min_track.GetMinTangent(wiremap, xt_));

   min_track.InitFit(wiremap, xt_, test_cid);
   min_track.DoFit(wiremap, xt_);
   min_track.PrintFitResults();
   min_track.PrintTrackWithLine(wiremap, xt_, min_track.GetFitLine());
   return true;
}

void Run::DrawFit(Long64_t event_number, int cid1, int cid2, double z_step, int test_cid)
{
   bool done = DoFit(event_number, cid1, cid2, z_step, test_cid);
   if (!done) {
      printf("fit cannot be done\n");
      return;
   }

   WireMap& wiremap = chamber_.GetWireMap();
   Track& min_track = finder_.GetBestTrack();
   Line& fit_line = min_track.GetFitLine();
   fit_line.PrintLine();
   chamber_.DrawTrack(event_, xt_, min_track, fit_line);
}
