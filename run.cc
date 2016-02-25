#include "run.h"

Run::Run()
{
   g_debug_line = 0;
   g_debug_track = 0;
   g_debug_trackfinder = 0;
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

bool Run::FindBestTangent(int cid1, int cid2, double z_step)
{
   WireMap& wiremap = chamber_.GetWireMap();
   bool found = finder_.FindBestTrack(chamber_, xt_, cid1, cid2, z_step);
   if (!found) {
      return false;
   }
   return true;
}

void Run::DrawBestTangent()
{
   if (!finder_.HasFound()) {
      printf("track not found yet..\n");
      return;
   }

   WireMap& wiremap = chamber_.GetWireMap();
   Track& min_track = finder_.GetBestTrack();
   min_track.PrintTrack(wiremap, xt_);
   int min_itan = min_track.GetMinTangentNumber();
   chamber_.DrawHitsWithTrack(event_, xt_, min_track, min_itan);
}

void Run::DrawBestTangent(Long64_t event_number, int cid1, int cid2, double z_step)
{
   if (event_number>=0) {
      GetEntry(event_number);
   } else {
      GetNext();
   }
   bool found = FindBestTangent(cid1, cid2, z_step);
   if (!found) {
      printf("cannot find tangent...\n");
      return;
   }
   DrawBestTangent();
}

TH1F* Run::FillResidual(const char* hname, int nx, double xmin, double xmax, int cid1, int cid2, double z_step, int cid3, int max_event)
{
   WireMap& wiremap = chamber_.GetWireMap();
   TH1F* h1 = new TH1F(hname, hname, nx, xmin, xmax);
   Long64_t num_total = event_.GetEntries();
   if (max_event < num_total) {
      num_total = max_event;
   }
   printf("num_total %lld\n", num_total);
   for (Long64_t iev=0; iev<num_total; iev++) {
      GetEntry(iev);
      bool found = FindBestTangent(cid1, cid2, z_step);
      if (found) {
         printf("iev %lld\n", iev);
         h1->Fill(finder_.GetBestTrack().GetResidualOfMinTangent(wiremap, xt_, cid3));
      }
   }
   h1->Draw();
   return h1;
}

TH2F* Run::FillXT(const char* hname, int nx, double xmin, double xmax, int ny, double ymin, double ymax, int cid1, int cid2, double z_step, int cid3, int max_event)
{
   WireMap& wiremap = chamber_.GetWireMap();
   TH2F* h2 = new TH2F(hname, hname, nx, xmin, xmax, ny, ymin, ymax);
   Long64_t num_total = event_.GetEntries();
   if (max_event < num_total) {
      num_total = max_event;
   }
   printf("num_total %lld\n", num_total);
   for (Long64_t iev=0; iev<num_total; iev++) {
      GetEntry(iev);
      bool found = FindBestTangent(cid1, cid2, z_step);
      if (found) {
         printf("iev %lld\n", iev);
         double fitR = finder_.GetBestTrack().GetResidualOfMinTangent(wiremap, xt_, cid3);
         double dT = finder_.GetBestTrack().GetHit(cid3).GetDriftTimeFromT0();
         h2->Fill(fitR, dT);
      }
   }
   h2->Draw();
   return h2;
}
