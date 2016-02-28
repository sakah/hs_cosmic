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
   for (int iev=0; iev<max_events; iev++) {
      outroot_.GetEntry(iev);
      if (!LoopEvent()) continue;
      //outroot_.PrintEntry();
   }
   EndOfEvent();
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
   }
}

bool AnaResRVSHitR::LoopEvent()
{
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
      double hitR = outroot_.GetTrackHitR(test_cid, test_cid);
      double resR = outroot_.GetTrackResR(test_cid, test_cid);
      h2_resR_VS_hitR_[test_cid]->Fill(hitR, resR);
   }
   return true;
}

void AnaResRVSHitR::EndOfEvent()
{
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
      double x[20];
      for (int ihitR=0; ihitR<20; ihitR++) {
         x[ihitR] = ihitR*0.5;
      }
      gr_resR_mean_VS_hitR_[test_cid] = new TGraph(20, x, resR_mean_[test_cid]);
   }
} 

TH2F* AnaResRVSHitR::GetResRVSHitR(int test_cid)
{
   return h2_resR_VS_hitR_[test_cid];
}

TGraph* AnaResRVSHitR::GetResRMeanVSHitR(int test_cid)
{
   return gr_resR_mean_VS_hitR_[test_cid];
}
