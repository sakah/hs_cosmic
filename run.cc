#include "run.h"

Run::Run()
{
}

Run::Run(const char* root_file_path)
{
   xt_.SetDriftVelocity(0.040);
   chamber_.ReadWireMap("wirepos.proto4.sw.ver7.txt");
   event_.OpenRootFile(root_file_path);
   event_.SetT0(-850);
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

void Run::FindBestTangent(int cid1, int cid2, double z_step)
{
   WireMap& wiremap = chamber_.GetWireMap();
   bool found = finder_.FindBestTrack(chamber_, xt_, cid1, cid2, z_step);
   if (found) {
      Track& min_track = finder_.GetBestTrack();
      min_track.PrintTrack(wiremap, xt_);
      int min_itan = min_track.GetMinTangentNumber();
      chamber_.DrawHitsWithTrack(event_, xt_, min_track, min_itan);
   }
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
   GetEntry(event_number);
   FindBestTangent(cid1, cid2, z_step);
   DrawBestTangent();
}

