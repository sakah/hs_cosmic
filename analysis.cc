#include "analysis.h"

Analysis::Analysis()
{
}

void Analysis::OpenRootFile(const char* path)
{
   outroot_.OpenRootFile(path);

   // get basename of root file
   // ../../root_path/run16.root
   // -> name_ = run16
   char* p1 = strrchr(path, '/');
   char* p2 = strrchr(path, '.');
   if (p1==NULL) { p1 = (char*)path; }
   else { p1++; }
   p2--;
   int i=0;
   for (char* p = p1; p<=p2; p++) {
      name_[i++] = *p;
   }
   name_[i] = '\0';
   //printf("name_ => '%s'\n", name_);
}

void Analysis::Loop(int max_events)
{
   BeginOfEvent();
   int num_ents = outroot_.GetEntries();
   if (max_events>num_ents) {
      max_events = num_ents;
   }
   num_events_ = 0;
   for (int iev=0; iev<max_events; iev++) {
      outroot_.GetEntry(iev);
      if (!LoopEvent()) continue;
      //outroot_.PrintEntry();
      num_events_++;
   }
   EndOfEvent();
}

int Analysis::GetNumEvents()
{
   return num_events_;
}

void Analysis::BeginOfEvent()
{
   // impliment in derived class
}

bool Analysis::LoopEvent()
{
   // impliment in derived class
   return true;
}

void Analysis::EndOfEvent()
{
   // impliment in derived class
}

//______________________________

AnaResRVSHitR::AnaResRVSHitR()
{
}

void AnaResRVSHitR::BeginOfEvent()
{
   for (int test_cid=0; test_cid<MAX_LAYER; test_cid++) {
      h2_resR_VS_hitR_[test_cid] = new TH2F(Form("%s_resR_VS_hitR_%d", name_, test_cid), "", 100, 0, 10, 100, -2, 2);
      for (int ihitR=0; ihitR<20; ihitR++) {
         h1_resR_[test_cid][ihitR] = new TH1F(Form("%s_resR_%d_%d", name_, test_cid, ihitR), "", 100, -2, 2);
      }
   }
}

bool AnaResRVSHitR::LoopEvent()
{
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
      double hitR = outroot_.GetTrackHitR(test_cid, test_cid);
      double resR = outroot_.GetTrackResR(test_cid, test_cid);
      h2_resR_VS_hitR_[test_cid]->Fill(hitR, resR);

      int ihitR = 0;
      for (int i=0; i<20; i++) {
         if (hitR>=i*0.5 && hitR<(i+1)*0.5) ihitR=i;
      }
      h1_resR_[test_cid][ihitR]->Fill(resR);
   }
   return true;
}

void AnaResRVSHitR::EndOfEvent()
{
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
      double x[20];
      double y1[20];
      double y2[20];
      for (int ihitR=0; ihitR<20; ihitR++) {
         x[ihitR] = ihitR*0.5;
         TH1F* h1 = h1_resR_[test_cid][ihitR];
         y1[ihitR] = h1->GetMean();
         int imax = h1->GetMaximumBin();
         y2[ihitR] = h1->GetXaxis()->GetBinCenter(imax);
      }
      gr_resR_mean_VS_hitR_[test_cid] = new TGraph(20, x, y1);
      gr_resR_peak_VS_hitR_[test_cid] = new TGraph(20, x, y2);

      gr_resR_mean_VS_hitR_[test_cid]->SetMarkerStyle(20);
      gr_resR_peak_VS_hitR_[test_cid]->SetMarkerStyle(20);
   }
} 

TH2F* AnaResRVSHitR::GetResRVSHitR(int test_cid)
{
   return h2_resR_VS_hitR_[test_cid];
}

TH1F* AnaResRVSHitR::GetResR(int test_cid, int ihitR)
{
   return h1_resR_[test_cid][ihitR];
}

TGraph* AnaResRVSHitR::GetResRVSHitRMean(int test_cid)
{
   return gr_resR_mean_VS_hitR_[test_cid];
}

TGraph* AnaResRVSHitR::GetResRVSHitRPeak(int test_cid)
{
   return gr_resR_peak_VS_hitR_[test_cid];
}

void AnaResRVSHitR::DrawResRVSHitRMean(int test_cid)
{
   GetResRVSHitR(test_cid)->Draw("colz");
   GetResRVSHitRMean(test_cid)->Draw("pl same");
}

void AnaResRVSHitR::DrawResRVSHitRPeak(int test_cid)
{
   GetResRVSHitR(test_cid)->Draw("colz");
   GetResRVSHitRPeak(test_cid)->Draw("pl same");
}
