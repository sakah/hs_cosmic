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
   t_->Branch("trig_number",       &trig_number_,      "trig_number/I");
   t_->Branch("hit_num_hitcells",   hit_num_hitcells_, "hit_num_hitcells[9]/I");
   t_->Branch("hit_icell",          hit_icell_,        "hit_icell[9][15]/I");
   t_->Branch("hit_q",              hit_q_,            "hit_q[9][15]/D");
   t_->Branch("hit_num_hittdcs",    hit_num_hittdcs_,  "hit_num_hittdcs[9][15]/I");
   t_->Branch("hit_drift_time",     hit_drift_time_,   "hit_drit_time[9][15][10]/D");

   t_->Branch("trackfinder_num_tracks", &trackfinder_num_tracks_, "trackfinder_num_tracks/I");

   t_->Branch("track_chi2",         track_chi2_,         "track_chi2[9]/D");
   t_->Branch("track_ndf",          track_ndf_,          "track_ndf[9]/D");
   t_->Branch("track_icell",        track_icell_,        "track_icell[9][9]/D");
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
   trig_number_ = event.GetTriggerNumber();
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
