#include "chamber.h"

#include "TVector3.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TEllipse.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TText.h"

Chamber::Chamber()
{
}

void Chamber::SetAdcPeakThreshold(int adc_thre)
{
   adc_thre_ = adc_thre;
}

void Chamber::GetEvent(Event& event)
{
   ClearEvent();

   for (int ch=0; ch<MAX_CH; ch++) {
      int cid = wiremap_.GetLayerNumber(ch);
      int icell = wiremap_.GetCellNumber(ch);
     // printf("ch %d cid %d icell %d\n", ch, cid, icell);
      if (cid==-1) continue; // not connected to wire
      int nhits = event.GetTdcNhit(ch);
      double ped = event.GetPedestal(ch);
      //if (nhits>0) {
      //   printf("ch %d cid %d icell %d nhits %d\n", ch, cid, icell, nhits);
      //}

      int jhit=0;
      for (int ihit=0; ihit<nhits; ihit++) {

         // apply adc peak cut
         // Compare adc @ clockNumber with adc @clockNumber+1
         // and get higer one
         int sample = event.GetClockNumberDriftTime(ch, ihit);
         bool over_threshold = false;
         for (int is=sample;  is<=sample+1; is++) {
            int adc = event.GetAdc(ch, is);
            //printf("sample %d adc %d\n", sample, adc);
            if (adc-ped>adc_thre_) {
               over_threshold = true;
               sample = is;
               break;
            }
         }
         if (!over_threshold) {
            continue;
         }
         if (cid==7&& icell==7) {
            printf("cid==7 && icel==7 ch %d sample %d adc %d\n", ch, sample, event.GetAdc(ch, sample));
         }

         int side  = wiremap_.GetLayerSide(cid, icell);
         Hit& hit = hits_[side][cid][icell][jhit];

         hit.SetHitNumber(jhit);
         hit.SetHitFlag(true);
         hit.SetLayerNumber(cid);
         hit.SetCellNumber(icell);
         hit.SetChanNumber(ch);
         hit.SetDriftTime(event.GetDriftTime(ch, ihit));
         hit.SetQ(event.GetQ(ch));
         hit.SetAdc(event.GetAdc(ch, sample));
         hit.SetPedestal(event.GetPedestal(ch));
         hit.SetT0(GetT0(cid, icell));
         hit.SetZ(wiremap_.GetZ0(cid));

         jhit++;
      }
   }
}

void Chamber::ClearEvent()
{
   for (int side=0; side<2; side++) {
      for (int cid=0; cid<MAX_LAYER; cid++) {
         for (int icell=0; icell<MAX_CELL; icell++) {
            for (int ihit=0; ihit<MAX_CELL_HIT; ihit++) {
               hits_[side][cid][icell][ihit].SetHitFlag(false);
            }
         }
      }
   }
}

void Chamber::ReadWireMap(const char* path, double rot_deg)
{
   wiremap_.Read(path, rot_deg);
}

WireMap& Chamber::GetWireMap()
{
   return wiremap_;
}

int Chamber::GetNumHitCells(int side, int cid)
{
   int num_hits = 0;
   for (int icell=0; icell<MAX_CELL; icell++) {
      bool has_hit = false;
      for (int ihit=0; ihit<MAX_CELL_HIT; ihit++) {
         has_hit = hits_[side][cid][icell][ihit].HasHit();
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

int Chamber::GetNumHitsInCell(int side, int cid, int icell)
{
   int num_hits = 0;
   int num_hit_cells = GetNumHitCells(side, cid);
   for (int ihit=0; ihit<num_hit_cells; ihit++) {
      bool has_hit = hits_[side][cid][icell][ihit].HasHit();
      if (has_hit) {
         num_hits++;
      }
   }
   return num_hits;
}

void Chamber::GetHitCellNumberByMaxAdcPeak(int side, int cid, int& max_icell, int& max_ihit)
{
   int num_hit_cells = GetNumHitCells(side, cid);
   if (num_hit_cells==0) {
      return;
   }

   double max_adc_peak = -1000;
   max_icell = -1;
   max_ihit = -1;
   int num=0;
   for (int icell=0; icell<MAX_CELL; icell++) {
      int num_tdc_hits_ov_threshold = GetNumHitsInCell(side, cid, icell);
      if (num_tdc_hits_ov_threshold==0) continue;
      for (int ihit=0; ihit<num_tdc_hits_ov_threshold; ihit++) {
         Hit& hit = hits_[side][cid][icell][ihit];
         int adc = hit.GetAdc();
         double ped = hit.GetPedestal();
         double adc_peak = adc-ped;
         //if (cid==7) printf("cid %d icell %d ihit %d adc %d ped %f adc_peak %lf\n", cid, icell, hit, adc, ped, adc_peak);
         if (adc_peak > max_adc_peak) {
            max_adc_peak = adc_peak;
            max_icell = icell;
            max_ihit = ihit;
         }
      }
   }
   //if (cid==7) printf("max_icell %d max_ihit %d\n", max_icell, max_ihit);
} 

int Chamber::GetHitCellNumber(int side, int cid, int icellhit)
{
   int num_hit_cells = GetNumHitCells(side, cid);
   if (num_hit_cells==0) {
      return -1;
   }

   int num=0;
   for (int icell=0; icell<MAX_CELL; icell++) {
      bool has_hit = hits_[side][cid][icell][0].HasHit();
      if (has_hit) {
         if (num==icellhit) {
            return icell;
         }
         num++;
      }
   }
   return -1;
}

Hit& Chamber::GetHit(int side, int cid, int icell, int ihit)
{
   return hits_[side][cid][icell][ihit];
}

int Chamber::GetNumLayerHits(int side, int cid)
{
   int nhits = 0;
   for (int icell=0; icell<MAX_CELL; icell++) {
      if (wiremap_.GetLayerSide(cid, icell)==side) {
         nhits += GetNumHitsInCell(side, cid, icell);
      }
   }
   return nhits;
}

bool Chamber::isGuardLayer(int side, int cid)
{
   if (side==WireMap::SIDE_TOP) {
      if (cid==19 || cid==7) return true;
   }
   if (side==WireMap::SIDE_BOTTOM) {
      if (cid==19 || cid==10) return true;
   }
   return false;
}

bool Chamber::isLayerUsed(int side, int cid)
{
   return wiremap_.isLayerUsed(side, cid);
}
bool Chamber::hasAllLayerHits()
{
   for (int side=0; side<2; side++) {
      for (int cid=0; cid<MAX_LAYER; cid++) {
         bool used = wiremap_.isLayerUsed(side, cid);
         //printf("### side %d cid %d used %d\n", side, cid, used);
         if (used) {
            int nhits = GetNumLayerHits(side, cid);
            if (nhits==0) return false;
         }
      }
   }
   return true;
}

void Chamber::PrintHits(XTcurve& xt, double adc_peak_thre)
{
   for (int side=0; side<MAX_SIDE; side++) {
      for (int cid=0; cid<MAX_LAYER; cid++) {
         printf("side %d cid %2d num_hit_cells %d\n", side, cid, GetNumHitCells(side, cid));
      }
   }
   for (int side=0; side<MAX_SIDE; side++) {
      for (int cid=0; cid<MAX_LAYER; cid++) {
         for (int icell=0; icell<MAX_CELL; icell++) {
            int nhits =  GetNumHitsInCell(side, cid, icell);
            if (nhits==0) continue;
            //printf("cid %2d icell %2d num_hits_in_cell %d\n", cid, icell, nhits);
            for (int ihit=0; ihit<nhits; ihit++) {
               hits_[side][cid][icell][ihit].PrintHit(xt, adc_peak_thre);
            }
         }
         printf("----\n");
      }
   }
}

void Chamber::DrawHits(Event& event, XTcurve& xt)
{
   //TCanvas*c1 = new TCanvas("c1-chamber-drawhits", "a", 700, 700);
   TH2F* h2 = new TH2F("h2", Form("%s Event# %lld", event.GetRootPath(), event.GetEventNumber()), 
         100, -1000, 1000, 100, -1000, 1000);
   h2->SetStats(0);
   h2->Draw();

   for (int ch=0; ch<MAX_CH; ch++) {
      int cid = wiremap_.GetLayerNumber(ch);
      int icell = wiremap_.GetCellNumber(ch);
      if (cid==-1) continue;
      //printf("### ch %d cid %d icell %d\n", ch, cid, icell);
      TVector3 pos = wiremap_.GetWire(cid, icell).GetPosAtZ(wiremap_.GetZ0(cid));
      //pos.Print();
      TMarker* m = new TMarker(pos.X(), pos.Y(), 20);
      m->SetMarkerSize(0.3);
      m->Draw();
   }

   for (int side=0; side<MAX_SIDE; side++) {
      for (int cid=0; cid<MAX_LAYER; cid++) {
         for (int icell=0; icell<MAX_CELL; icell++) {
            int nhits = GetNumHitsInCell(side, cid, icell);
            if (nhits==0) continue;
            for (int ihit=0; ihit<nhits; ihit++) {
               Hit& hit = hits_[side][cid][icell][ihit];
               //int cid = hit.GetLayerNumber();
               //int icell = hit.GetCellNumber();
               double hitR = hit.GetHitR(xt);
               double hitZ = hit.GetZ();
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
}

void Chamber::DrawTrack(Event& event, XTcurve& xt, Track& track, Line& line, int test_side, int test_cid, char* pdf_dir)
{
   char side_name[32];
   if (test_side==WireMap::SIDE_TOP) {
      strcpy(side_name, "up");
   } else {
      strcpy(side_name, "bottom");
   }

   int ndf = track.GetNDF();
   double chi2 = track.GetChi2();
   double red_chi2 = track.GetRedChi2();

   double ypos_S1 = 1900/2.0; // mm
   double ypos_S2 = -ypos_S1;
   TVector3 pos_s1 = line.GetPosAtY(ypos_S1);
   TVector3 pos_s2 = line.GetPosAtY(ypos_S2);


   TCanvas*c1 = new TCanvas("c1-chamber-drawhitswithtrack", "", 500, 1250);
   TPad* p1 = new TPad("p1", "p1", 0, 0.8, 1, 1);
   TPad* p2 = new TPad("p2", "p2", 0, 0, 1, 0.8);
   
   p1->Draw();
   p2->Draw();

   p1->cd();
   TPaveText* pt = new TPaveText(0, 0, 1, 1);

   pt->AddText(Form("%s", event.GetRootPath()));
   pt->AddText(Form("Event# %lld", event.GetEventNumber()));
   pt->AddText(Form("Trigger# %d", event.GetTriggerNumber()));
   pt->AddText(Form("Test layer %d (%s)", test_cid, side_name));
   pt->AddText(Form("chi2/NDF= %5.2f/%d (%5.2f)", chi2, ndf, red_chi2));
   pt->AddText(Form("S1: x= %6.2f , y= %6.2f , z= %6.2f [mm]", pos_s1.X(),pos_s1.Y(),pos_s1.Z() ));
   pt->AddText(Form("S2: x= %6.2f , y= %6.2f , z= %6.2f [mm]", pos_s2.X(),pos_s2.Y(),pos_s2.Z() ));
   pt->Draw();

   p2->Divide(2,1);

   p2->cd(1);
   drawCDCup(event);
   drawTrack(event, xt, track, line);

   p2->cd(2);
   drawCDCbottom(event);
   drawTrack(event, xt, track, line);

   if (pdf_dir) {
      c1->Print(Form("%s/iev_%04lld_tn_%04d.pdf", pdf_dir, event.GetEventNumber(), event.GetTriggerNumber()));
   }
}

void Chamber::drawCDCup(Event& event)
{
   TH2F* h2 = new TH2F("h2-up", "", 100, -50, 50, 100, 600, 850);
   h2->SetStats(0);
   h2->Draw();

   TPaveText* pt = new TPaveText(0.08793507,0.9052217,0.2552711,0.9356514,"brNDC");
   pt->SetTextSize(0.1);
   pt->SetFillColor(0);
   pt->SetBorderSize(0);
   pt->AddText("Up");
   pt->Draw();
}

void Chamber::drawCDCbottom(Event& event)
{
   TH2F* h2 = new TH2F("h2-bottom", "", 100, -50, 50, 100, -850, -600);
   h2->SetStats(0);
   h2->Draw();

   TPaveText* pt = new TPaveText(0.08793507,0.9052217,0.2552711,0.9356514,"brNDC");
   pt->SetTextSize(0.1);
   pt->SetFillColor(0);
   pt->SetBorderSize(0);
   pt->AddText("Bottom");
   pt->Draw();
}

void Chamber::drawTrack(Event& event, XTcurve& xt, Track& track, Line& line)
{
   for (int side=0; side<MAX_SIDE; side++) {
      for (int cid=0; cid<MAX_LAYER; cid++) {
         Hit& hit = track.GetHit(side, cid);
         if (!hit.HasHit()) continue;
         //hit.PrintHit(xt);
         int icell = hit.GetCellNumber();
         Line& wire = wiremap_.GetWire(cid, icell);
         line.DrawLine(kRed);
         TVector3 pA;
         TVector3 pB;
         wire.GetClosestPoints(line, pA, pB);
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

         TText* txt = new TText(pos.X(), pos.Y(), Form("(%d-%d)", cid, icell));
         txt->SetTextSize(0.04);
         txt->Draw();
      }
   }
}

void Chamber::SetT0(int bd, double t0)
{
   t0_[bd] = t0;
}

void Chamber::SetVelocity(double vel)
{
   vel_ = vel;
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

