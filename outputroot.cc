#include "outputroot.h"
#include <time.h>

OutputROOT::OutputROOT()
{
}

OutputROOT::OutputROOT(const char* path)
{
   SetRootFile(path);
}

void OutputROOT::Clear()
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      hit_num_hitcells_[cid] = 0;
      for (int ihitcell=0; ihitcell<MAX_CELL; ihitcell++) {
         hit_icell_[cid][ihitcell] = -1;
         for (int ihit=0; ihit<MAX_CELL_HIT; ihit++) {
            hit_q_[cid][ihitcell] = -1;
            hit_drift_time_[cid][ihitcell][ihit] = -1e10;
         }
      }
   }
   trackfinder_num_tracks_ = 0;

   for (int test_cid=0; test_cid<MAX_LAYER; test_cid++) {
      track_chi2_[test_cid] = -1;
      track_ndf_[test_cid]  = -1;

      for (int cid=0; cid<MAX_LAYER; cid++) {
         track_icell_[test_cid][cid]        = -1;
         track_q_[test_cid][cid]            = -1;
         track_num_hittdcs_[test_cid][cid]  = 0;
         track_hitT_[test_cid][cid]         = -1e10;
         track_fitT0_[test_cid][cid]        = -1e10;
         track_fitdT_[test_cid][cid]        = -1e10;
         track_hitR_[test_cid][cid]         = -1e10;
         track_fitX_[test_cid][cid]         = -1e10;
         track_resR_[test_cid][cid]         = -1e10;
         track_fitZ_[test_cid][cid]         = -1e10;
      }
   }
}

void OutputROOT::SetRootFile(const char* path)
{
   f_ = new TFile(path, "recreate");
   t_ = new TTree("t", "t");

   t_->Branch("etime",             &etime_,            "etime/I");

   t_->Branch("iev",               &iev_,              "iev/I");
   t_->Branch("hit_num_hitcells",   hit_num_hitcells_, "hit_num_hitcells[9]/I");
   t_->Branch("hit_icell",          hit_icell_,        "hit_icell[9][15]/I");
   t_->Branch("hit_q",              hit_q_,            "hit_q[9][15]/D");
   t_->Branch("hit_num_hittdcs",    hit_num_hittdcs_,  "hit_num_hittdcs[9][15]/I");
   t_->Branch("hit_drift_time",     hit_drift_time_,   "hit_drit_time[9][15][10]/D");

   t_->Branch("trackfinder_num_tracks", &trackfinder_num_tracks_, "trackfinder_num_tracks/I");

   t_->Branch("track_chi2",         track_chi2_,         "track_chi2[9]/D");
   t_->Branch("track_ndf",          track_ndf_,          "track_ndf[9]/D");
   t_->Branch("track_icell",        track_icell_,        "track_icell[9]/D");
   t_->Branch("track_q",            track_q_,            "track_q[9][9]/D");
   t_->Branch("track_num_hittdcs",  track_num_hittdcs_,  "track_num_hittdcs[9][9]/I");
   t_->Branch("track_hitT",         track_hitT_,         "track_hitT[9][9]/D");
   t_->Branch("track_fitT0",        track_fitT0_,        "track_fitT0[9][9]/D");
   t_->Branch("track_fitdT",        track_fitdT_,        "track_fitdT[9][9]/D");
   t_->Branch("track_hitR",         track_hitR_,         "track_hitR[9][9]/D");
   t_->Branch("track_fitX",         track_fitX_,         "track_fitX[9][9]/D");
   t_->Branch("track_resR",         track_resR_,         "track_resR[9][9]/D");
   t_->Branch("track_fitZ",         track_fitZ_,         "track_fitZ[9][9]/D");

}

void OutputROOT::SetHitData(Event& event, Chamber& chamber)
{
   iev_ = event.GetEventNumber();
   for (int cid=0; cid<MAX_LAYER; cid++) {
      int nhits = chamber.GetNumHitCells(cid);
      hit_num_hitcells_[cid] = nhits;
      for (int ihitcell=0; ihitcell<nhits; ihitcell++) {
         int icell = chamber.GetHitCellNumber(cid, ihitcell);
         hit_icell_[cid][ihitcell] = icell;
         int nhits_tdc = chamber.GetNumHitsInCell(cid, icell);
         hit_num_hittdcs_[cid][ihitcell] = nhits_tdc;
         for (int ihit=0; ihit<nhits_tdc; ihit++) {
            Hit& hit = chamber.GetHit(cid, icell, ihit);
            hit_q_[cid][ihitcell] = hit.GetQ();
            hit_drift_time_[cid][ihitcell][ihit] = hit.GetDriftTime();
         }
      }
   }
}

void OutputROOT::SetTrackFinderData(TrackFinder& finder)
{
   trackfinder_num_tracks_ = finder.GetNumTracks();
}

void OutputROOT::SetTrackData(Chamber& chamber, XTcurve& xt, Track& track)
{
   WireMap& wiremap = chamber.GetWireMap();
   Line& fit_line = track.GetFitLine();

   int test_cid = track.GetTestLayerNumber();
   track_chi2_[test_cid] = track.GetChi2OfLine(wiremap, xt, fit_line);
   track_ndf_[test_cid]  = track.GetNDF();

   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = track.GetHit(cid);
      if (!hit.HasHit()) continue;

      int icell = hit.GetCellNumber();
      double hitR = hit.GetHitR(xt);
      double fitX = track.GetXFromLine(wiremap, cid, fit_line);
      track_icell_[test_cid][cid]        = icell;
      track_q_[test_cid][cid]            = hit.GetQ();
      track_num_hittdcs_[test_cid][cid]  = chamber.GetNumHitsInCell(cid, icell);
      track_hitT_[test_cid][cid]         = hit.GetDriftTime();
      track_fitT0_[test_cid][cid]        = hit.GetT0();
      track_fitdT_[test_cid][cid]        = hit.GetDriftTimeFromT0();
      track_hitR_[test_cid][cid]         = hitR;
      track_fitX_[test_cid][cid]         = fitX;
      track_resR_[test_cid][cid]         = TMath::Abs(fitX) - hitR;
      track_fitZ_[test_cid][cid]         = hit.GetZ();
   }
}

void OutputROOT::Fill()
{
   t_->Fill();
   t_->AutoSave();
}

void OutputROOT::Write()
{
   f_->cd();
   t_->Write();
   f_->Close();
}

void OutputROOT::SetElapstedTime(int etime)
{
   etime_ = etime;
}

void OutputROOT::OpenRootFile(const char* path)
{
   f_ = new TFile(path);
   t_ = (TTree*)f_->Get("t");
   if (t_==NULL) {
      fprintf(stderr, "ERROR: tree is not foudn...\n");
      return;
   }
   t_->SetBranchAddress("etime",             &etime_);

   t_->SetBranchAddress("iev",               &iev_);
   t_->SetBranchAddress("hit_num_hitcells",   hit_num_hitcells_);
   t_->SetBranchAddress("hit_icell",          hit_icell_);
   t_->SetBranchAddress("hit_q",              hit_q_);
   t_->SetBranchAddress("hit_num_hittdcs",    hit_num_hittdcs_);
   t_->SetBranchAddress("hit_drift_time",     hit_drift_time_);

   t_->SetBranchAddress("trackfinder_num_tracks", &trackfinder_num_tracks_);

   t_->SetBranchAddress("track_chi2",         track_chi2_);
   t_->SetBranchAddress("track_ndf",          track_ndf_);
   t_->SetBranchAddress("track_icell",        track_icell_);
   t_->SetBranchAddress("track_q",            track_q_);
   t_->SetBranchAddress("track_num_hittdcs",  track_num_hittdcs_);
   t_->SetBranchAddress("track_hitT",         track_hitT_);
   t_->SetBranchAddress("track_fitT0",        track_fitT0_);
   t_->SetBranchAddress("track_fitdT",        track_fitdT_);
   t_->SetBranchAddress("track_hitR",         track_hitR_);
   t_->SetBranchAddress("track_fitX",         track_fitX_);
   t_->SetBranchAddress("track_resR",         track_resR_);
   t_->SetBranchAddress("track_fitZ",         track_fitZ_);
}

int OutputROOT::GetEntries()
{
   return t_->GetEntries();
}

void OutputROOT::GetEntry(int event_number)
{
   t_->GetEntry(event_number);
}

void OutputROOT::PrintEntry()
{
   printf("iev   %d\n",  iev_);
   printf("etime %d\n", etime_);

   for (int cid=0; cid<MAX_LAYER; cid++) {
      printf("cid %d hit_num_hitcells %d\n", cid, hit_num_hitcells_[cid]);
      for (int ihitcell=0; ihitcell<hit_num_hitcells_[cid]; ihitcell++) {
         printf("cid %d ihitcell %d hit_icell        %d\n", cid, ihitcell, hit_icell_[cid][ihitcell]);
         printf("cid %d ihitcell %d hit_q            %f\n", cid, ihitcell, hit_q_[cid][ihitcell]);
         printf("cid %d ihitcell %d hit_num_hittdcs  %d\n", cid, ihitcell, hit_num_hittdcs_[cid][ihitcell]);
         for (int ihittdc=0; ihittdc<hit_num_hittdcs_[cid][ihitcell]; ihittdc++) {
            printf("cid %d ihitcell %d ihittdc %d hit_drift_time %f\n", cid, ihitcell, ihittdc, hit_drift_time_[cid][ihitcell][ihittdc]);
         }
      }
      printf("cid %d trackfinder_num_tracks %d\n", cid, trackfinder_num_tracks_);
   }

   // skip guard layers
   for (int test_cid=1; test_cid<MAX_LAYER-1; test_cid++) {
      printf("test_cid %d track_chi2 %f\n", test_cid, track_chi2_[test_cid]);
      printf("test_cid %d track_ndf  %f\n", test_cid, track_ndf_[test_cid]);
      for (int cid=1; cid<MAX_LAYER-1; cid++) {
         printf("test_cid %d cid %d track_icell       %d\n", test_cid, cid, track_icell_[test_cid][cid]);
         printf("test_cid %d cid %d track_q           %f\n", test_cid, cid, track_q_[test_cid][cid]);
         printf("test_cid %d cid %d track_num_hittdcs %d\n", test_cid, cid, track_num_hittdcs_[test_cid][cid]);
         printf("test_cid %d cid %d track_hitT        %f\n", test_cid, cid, track_hitT_[test_cid][cid]);
         printf("test_cid %d cid %d track_fitT0       %f\n", test_cid, cid, track_fitT0_[test_cid][cid]);
         printf("test_cid %d cid %d track_fitdT       %f\n", test_cid, cid, track_fitdT_[test_cid][cid]);
         printf("test_cid %d cid %d track_hitR        %f\n", test_cid, cid, track_hitR_[test_cid][cid]);
         printf("test_cid %d cid %d track_fitX        %f\n", test_cid, cid, track_fitX_[test_cid][cid]);
         printf("test_cid %d cid %d track_resR        %f\n", test_cid, cid, track_resR_[test_cid][cid]);
         printf("test_cid %d cid %d track_fitZ        %f\n", test_cid, cid, track_fitZ_[test_cid][cid]);
      }
   }
}

int    OutputROOT::GetElapstedTime()
{
   return etime_;
}

int    OutputROOT::GetEventNumber()
{
   return iev_;
}

int    OutputROOT::GetNumHitCells(int cid)
{
   return hit_num_hitcells_[cid];
}

int    OutputROOT::GetHitCellNumber(int cid, int ihitcell)
{
   return hit_icell_[cid][ihitcell];
}

double OutputROOT::GetHitQ(int cid, int ihitcell)
{
   return hit_q_[cid][ihitcell];
}

int    OutputROOT::GetHitNumTdcHits(int cid, int ihitcell)
{
   return hit_num_hittdcs_[cid][ihitcell];
}

double OutputROOT::GetHitDriftTime(int cid, int ihitcell, int ihittdc)
{
   return hit_drift_time_[cid][ihitcell][ihittdc];
}

int    OutputROOT::GetTrackfinderNumTracks()
{
   return trackfinder_num_tracks_;
}

double OutputROOT::GetTrackChi2(int test_cid)
{
   return track_chi2_[test_cid];
}

double OutputROOT::GetTrackNDF(int test_cid)
{
   return track_ndf_[test_cid];
}

int    OutputROOT::GetTrackCellNumber(int test_cid, int cid)
{
   return track_icell_[test_cid][cid];
}

double OutputROOT::GetTrackCellQ(int test_cid, int cid)
{
   return track_q_[test_cid][cid];
}

int    OutputROOT::GetTrackCellNumTdcHits(int test_cid, int cid)
{
   return track_num_hittdcs_[test_cid][cid];
}

double OutputROOT::GetTrackHitT(int test_cid, int cid)
{
   return track_hitT_[test_cid][cid];
}

double OutputROOT::GetTrackFitT0(int test_cid, int cid)
{
   return track_fitT0_[test_cid][cid];
}

double OutputROOT::GetTrackFitdT(int test_cid, int cid)
{
   return track_fitdT_[test_cid][cid];
}

double OutputROOT::GetTrackHitR(int test_cid, int cid)
{
   return track_hitR_[test_cid][cid];
}

double OutputROOT::GetTrackFitX(int test_cid, int cid)
{
   return track_fitX_[test_cid][cid];
}

double OutputROOT::GetTrackResR(int test_cid, int cid)
{
   return track_resR_[test_cid][cid];
}

double OutputROOT::GetTrackFitZ(int test_cid, int cid)
{
   return track_fitZ_[test_cid][cid];
}

