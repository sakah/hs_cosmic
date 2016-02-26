#include "chamber.h"

#include "TVector3.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TEllipse.h"

Chamber::Chamber()
{
}

void Chamber::GetEvent(Event& event)
{
   ClearEvent();

   for (int ch=0; ch<MAX_CH; ch++) {
      int cid = wiremap_.GetLayerNumber(ch);
      int icell = wiremap_.GetCellNumber(ch);
      if (cid==-1) continue; // not connected to wire
      int nhits = event.GetTdcNhit(ch);
      //printf("ch %d cid %d icell %d nhits %d\n", ch, cid, icell, nhits);
      for (int ihit=0; ihit<nhits; ihit++) {
         Hit& hit = hits_[cid][icell][ihit];
         hit.SetHitFlag(true);
         hit.SetLayerNumber(cid);
         hit.SetCellNumber(icell);
         hit.SetChanNumber(ch);
         hit.SetDriftTime(event.GetDriftTime(ch, ihit));
         hit.SetT0(GetT0(cid, icell));
         hit.SetZ(wiremap_.GetZRO());
      }
   }
}

void Chamber::ClearEvent()
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      for (int icell=0; icell<MAX_CELL; icell++) {
         for (int ihit=0; ihit<MAX_CELL_HIT; ihit++) {
            hits_[cid][icell][ihit].SetHitFlag(false);
         }
      }
   }
}

void Chamber::ReadWireMap(const char* path)
{
   wiremap_.Read(path);
}

WireMap& Chamber::GetWireMap()
{
   return wiremap_;
}

int Chamber::GetNumHitCells(int cid)
{
   int num_hits = 0;
   for (int icell=0; icell<MAX_CELL; icell++) {
      bool has_hit = false;
      for (int ihit=0; ihit<MAX_CELL_HIT; ihit++) {
         has_hit = hits_[cid][icell][ihit].HasHit();
         if (has_hit) {
            has_hit = true;
            break;
         }
      }
      if (has_hit) {
         num_hits++;
      }
   }
   return num_hits;
}

int Chamber::GetNumHitsInCell(int cid, int icell)
{
   int num_hits = 0;
   int num_hit_cells = GetNumHitCells(cid);
   for (int ihit=0; ihit<num_hit_cells; ihit++) {
      bool has_hit = hits_[cid][icell][ihit].HasHit();
      if (has_hit) {
         num_hits++;
      }
   }
   return num_hits;
}

int Chamber::GetHitCellNumber(int cid, int icellhit)
{
   int num_hit_cells = GetNumHitCells(cid);
   if (num_hit_cells==0) {
      return -1;
   }

   int num=0;
   for (int icell=0; icell<MAX_CELL; icell++) {
      bool has_hit = hits_[cid][icell][0].HasHit();
      if (has_hit) {
         if (num==icellhit) {
            return icell;
         }
         num++;
      }
   }
   return -1;
}

Hit& Chamber::GetHit(int cid, int icell, int ihit)
{
   return hits_[cid][icell][ihit];
}

void Chamber::PrintHits(XTcurve& xt)
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      printf("cid %d num_hit_cells %d\n", cid, GetNumHitCells(cid));
   }
   for (int cid=0; cid<MAX_LAYER; cid++) {
      for (int icell=0; icell<MAX_CELL; icell++) {
         int nhits =  GetNumHitsInCell(cid, icell);
         if (nhits==0) continue;
         printf("cid %d icell %d num_hits_in_cell %d\n", cid, icell, nhits);
         for (int ihit=0; ihit<nhits; ihit++) {
            hits_[cid][icell][ihit].PrintHit(xt);
         }
      }
      printf("----\n");
   }
}

void Chamber::DrawHits(Event& event, XTcurve& xt)
{
   TCanvas*c1 = new TCanvas("c1-chamber-drawhits", "", 700, 700);
   TH2F* h2 = new TH2F("h2", Form("%s Event# %lld", event.GetRootPath(), event.GetEventNumber()), 
         100, -100, 100, 100, 480, 680);
   h2->SetStats(0);
   h2->Draw();

   for (int ch=0; ch<MAX_CH; ch++) {
      int cid = wiremap_.GetLayerNumber(ch);
      int icell = wiremap_.GetCellNumber(ch);
      TVector3 pos = wiremap_.GetWire(cid, icell).GetPosAtZ(wiremap_.GetZRO());
      //printf("cid %d zpos %f\n", cid, zpos_[cid]);
      //pos.Print();
      TMarker* m = new TMarker(pos.X(), pos.Y(), 20);
      m->SetMarkerSize(0.3);
      m->Draw();
   }

   for (int cid=0; cid<MAX_LAYER; cid++) {
      for (int icell=0; icell<MAX_CELL; icell++) {
         int nhits = GetNumHitsInCell(cid, icell);
         if (nhits==0) continue;
         for (int ihit=0; ihit<nhits; ihit++) {
            Hit& hit = hits_[cid][icell][ihit];
            int cid = hit.GetLayerNumber();
            int icell = hit.GetCellNumber();
            double hitR = hit.GetHitR(xt);
            double hitZ = hit.GetZ();
            //printf("ihit %d cid %d z %f\n", ihit, cid,z);
            if (hitR<0) {
               printf("skip this hit due to negative hitR %f @cid %d icell %d\n", hitR, cid, icell);
               continue;
            }
            TVector3 pos = wiremap_.GetWire(cid, icell).GetPosAtZ(hitZ);
            TEllipse* e = new TEllipse(pos.X(), pos.Y(), hitR);
            if (ihit==0) {
               e->SetLineColor(kBlue);
            } else {
               e->SetLineColor(kRed);
            }
            e->SetFillStyle(4000);
            e->Draw();
         }
      }
   }
}

void Chamber::DrawHitsWithTrack(Event& event, XTcurve& xt, Track& track, int itan)
{
   TCanvas*c1 = new TCanvas("c1-chamber-drawhitswithtrack", "", 700, 700);
   TH2F* h2 = new TH2F("h2", Form("%s Event# %lld", event.GetRootPath(), event.GetEventNumber()), 
         100, -100, 100, 100, 480, 680);
   h2->SetStats(0);
   h2->Draw();

   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = track.GetHit(cid);
      if (!hit.HasHit()) continue;
      //hit.PrintHit(xt);
      int icell = hit.GetCellNumber();
      Line& wire = wiremap_.GetWire(cid, icell);
      Line& tangent = track.GetTangent(itan);
      tangent.DrawLine(kRed);
      TVector3 pA;
      TVector3 pB;
      wire.GetClosestPoints(tangent, pA, pB);
      double zA = pA.Z();
      //if (cid==1) zA=100;
      //if (cid==7) zA=200;
      //printf("--pA--\n");
      //pA.Print();
      //printf("--pB--\n");
      //pB.Print();
      TVector3 pos = wire.GetPosAtZ(zA);

      double hitR = hit.GetHitR(xt);
      if (hitR<0) {
         printf("skip this hit due to negative hitR %f @cid %d icell %d\n", hitR, cid, icell);
         continue;
      }
      TEllipse* e = new TEllipse(pos.X(), pos.Y(), hitR);
      e->SetLineColor(kBlue);
      e->SetFillStyle(4000);
      e->Draw();
   }
}

void Chamber::SetT0(int bd, double t0)
{
   t0_[bd] = t0;
}

double Chamber::GetT0(int bd)
{
   return t0_[bd];
}

double Chamber::GetT0(int cid, int icell)
{
   int bd = wiremap_.GetBoardNumber(cid, icell);
   return t0_[bd];
}
