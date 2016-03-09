#include "layer.h"

Layer::Layer()
{
   cid_ = -1;
   num_cells_ = 0;
}

Layer::Layer(int cid, int num_cells)
{
   cid_ = cid;
   num_cells_ = num_cells;
}

int Layer::GetLayerNumber()
{
   return cid_;
}

int Layer::GetNumCells()
{
   return num_cells_;
}

Cell& Layer::GetCell(int icell)
{
   return cells_[icell];
}

int Layer::GetHitCells(Cell** hit_cells)
{
   num_cells_ = 0;
   for (int icell=0; icell<num_cells_; icell++) {
      Cell& cell = cells_[icell];
      if (cell.GetNumHits()>0) {
         hit_cells[num_cells_] = &cell;
      }
      num_cells_++;
   }
   return num_cells_;
}

void Layer::ClearHits()
{
   for (int icell=0; icell<MAX_CELL; icell++) {
      cells_[icell].ClearHits();
   }
}

void Layer::PrintHits()
{
   for (int icell=0; icell<MAX_CELL; icell++) {
      int num_hits = cells_[icell].GetNumHits();
      if (num_hits>0) {
         cells_[icell].PrintHits();
      }
   }
}

void Layer::DrawHits()
{
   for (int icell=0; icell<MAX_CELL; icell++) {
      cells_[icell].DrawHits();
   }
}
