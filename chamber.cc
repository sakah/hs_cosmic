#include "chamber.h"
#include "config.h"
#include "wiremap.h"
#include "xtcurve.h"
#include "inputroot.h"

#include "TVector3.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TEllipse.h"

Chamber::Chamber(Config* config_ptr)
{
   wiremap_ptr_ = config_ptr->GetWireMap_Ptr();
   xtcurve_ptr_ = config_ptr->GetXTcurve_Ptr();
   input_root_ptr_ = config_ptr->GetInputROOT_Ptr();
   num_boards_ = config_ptr->GetT0Boards(t0_boards_);
   ClearHits();
}

void Chamber::ClearHits()
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      layers_[cid].ClearHits();
   }
}

void Chamber::GetEvent()
{
   ClearHits();

   for (int ch=0; ch<MAX_CH; ch++) {
      int cid = wiremap_ptr_->GetLayerNumber(ch);
      int icell = wiremap_ptr_->GetCellNumber(ch);
      if (cid==-1) continue; // not connected to wire
      int bd = wiremap_ptr_->GetBoardNumber(cid, icell);
      int nhits = input_root_ptr_->GetTdcNhit(ch);
      //printf("ch %d cid %d icell %d nhits %d\n", ch, cid, icell, nhits);
      for (int ihit=0; ihit<nhits; ihit++) {
         double drift_time = input_root_ptr_->GetDriftTime(ch, ihit);
         double t0 = t0_boards_[bd]; // initial t0 is set anyway. t0 will be set by trackfinder/trackfitter
         double q = input_root_ptr_->GetQ(ch);
         double z = wiremap_ptr_->GetZRO(); // z will be set by trackfinder/trackfitter
         double fitX = 0; // fitX will be set by trackfinder/trackfitter
         Hit hit(wiremap_ptr_, xtcurve_ptr_, cid, icell, ihit, XTcurve::RIGHT, drift_time, t0, q, z, fitX);
         layers_[cid].GetCell(icell).AddHit(hit);
      }
   }
}

Layer& Chamber::GetLayer(int cid)
{
   return layers_[cid];
}

int Chamber::GetHitLayers(Layer** hit_layers)
{
   int num_hit_layers = 0;
   for (int cid=0; cid<MAX_LAYER; cid++) {
      Cell* hit_cells[MAX_CELL];
      int num_hit_cells = layers_[cid].GetHitCells(hit_cells);
      if (num_hit_cells>0) {
         hit_layers[num_hit_layers] = &layers_[cid];
      }
   }
   return num_hit_layers;
}

void Chamber::PrintHits()
{
   printf("------------\n");
   printf("event_number %lld (%s)\n", input_root_ptr_->GetEventNumber(), input_root_ptr_->GetRootPath());
   printf("------------\n");
   for (int cid=0; cid<MAX_LAYER; cid++) {
      layers_[cid].PrintHits();
      printf("----\n");
   }
}

void Chamber::DrawHits()
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      layers_[cid].DrawHits();
   }
}

